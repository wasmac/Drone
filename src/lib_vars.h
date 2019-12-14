#define REMOTEXY_MODE__ESP8266WIFI_LIB_POINT
#if defined(ESP8266)
  #include <ESP8266WiFi.h>
#else
  #include <WiFi.h>
#endif
#include <RemoteXY.h>
#include <ESP32Servo.h>
#include <I2Cdev.h>
//#include <PID_v1.h>
#include <AutoPID.h>
#include "MPU6050_6Axis_MotionApps20.h"
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

/////////////////////////////////////////////////////////////
///                                                       ///
///                     IMU PART                          ///
///                                                       ///
/////////////////////////////////////////////////////////////

MPU6050 mpu;
Quaternion q; 
VectorFloat gravity;
#define INTERRUPT_PIN 18
#define LED_PIN 2
uint8_t mpuIntStatus;  
uint8_t devStatus;    
uint16_t packetSize;  
uint16_t fifoCount; 
uint8_t fifoBuffer[64]; 
float ypr[3];
volatile bool mpuInterrupt = false; //initiall state of mpu int pin <-- used for yaw drift compensation
bool blinkState = false;            
bool dmpReady = false;  
bool printMPU = true;               //set true to print Yaw Pitch and Roll values in Serial Monitor

/////////////////////////////////////////////////////////////
///                                                       ///
///                     PID PART                          ///
///                                                       ///
/////////////////////////////////////////////////////////////

#define pid_min_yaw -180
#define pid_max_yaw 180
#define pid_min_pitch_roll -30
#define pid_max_pitch_roll 30
double kp_yaw = 0.216, kp_pitch = 0.216, kp_roll = 0.216;
double ki_yaw = 0.36, ki_pitch = 0.36, ki_roll = 0.36;
double kd_yaw = 0.09, kd_pitch = 0.09, kd_roll = 0.09;
double desired_yaw = 0.0;
double desired_roll = 0.0;
double desired_pitch = 0.0;
double pid_setpoint_yaw;
double pid_setpoint_pitch;
double pid_setpoint_roll;
double pid_input_yaw;
double pid_input_pitch;
double pid_input_roll;
double pid_output_yaw;
double pid_output_pitch;
double pid_output_roll;
double pid_error_yaw;
double pid_error_pitch;
double pid_error_roll;
int pid_final_yaw = 1000;
int pid_final_pitch = 1000;
int pid_final_roll = 1000;
AutoPID pid_yaw(&pid_input_yaw, &pid_setpoint_yaw, &pid_output_yaw, pid_min_yaw, pid_max_yaw, kp_yaw, ki_yaw, kd_yaw);
AutoPID pid_pitch(&pid_input_pitch, &pid_setpoint_pitch, &pid_output_pitch, pid_min_pitch_roll, pid_max_pitch_roll, kp_pitch, ki_pitch, kd_pitch);
AutoPID pid_roll(&pid_input_roll, &pid_setpoint_roll, &pid_output_roll, pid_min_pitch_roll, pid_max_pitch_roll, kp_roll, ki_roll, kd_roll);

/////////////////////////////////////////////////////////////
///                                                       ///
///                     WiFi PART                         ///
///                                                       ///
/////////////////////////////////////////////////////////////

#define REMOTEXY_WIFI_SSID "RTDv0.1"
#define REMOTEXY_WIFI_PASSWORD "maciej333"
#define REMOTEXY_SERVER_PORT 6377
#define MOT_LED 4
Servo mot_R_F, mot_R_B, mot_L_F, mot_L_B;
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =
  { 255,4,0,22,0,64,0,8,41,0,
  5,32,0,22,41,41,24,42,31,5,
  32,59,22,41,41,24,42,31,67,4,
  2,11,20,5,24,41,11,67,4,23,
  11,20,5,32,41,11,129,0,2,4,
  29,6,8,83,116,97,114,116,32,32,
  32,32,84,101,115,116,32,118,97,108,
  0 };
struct {
  int8_t joystick_1_x;
  int8_t joystick_1_y;
  int8_t joystick_2_x;
  int8_t joystick_2_y; 
  char text_1[11];
  char text_2[11];
  uint8_t connect_flag;
} RemoteXY;
#pragma pack(pop)
int wifi_joystick_1_x = 1000;
int wifi_joystick_1_y = 1000;
int wifi_joystick_2_x = 1500; //was 1000
int wifi_joystick_2_y = 1500; //was 1000

/////////////////////////////////////////////////////////////
///                                                       ///
///                     Motors PART                       ///
///                                                       ///
/////////////////////////////////////////////////////////////

#define MIN_PULSE_LENGTH 1000 
#define MAX_PULSE_LENGTH 2000 
double d_mot_on_off;
int calib_x = 1500;
int calib_y = 1500;
int calib_z = 1500;
int mot_R_F_speed = 1000;
int mot_R_B_speed = 1000;
int mot_L_F_speed = 1000;
int mot_L_B_speed = 1000;
bool mot_on_off = false;
bool mot_flag = false;
bool mot_calibration = true;  //to calibrate manually set to true
bool calib_flag = false;
bool calib_flag_2 = false;

/////////////////////////////////////////////////////////////
///                                                       ///
///                     GPS PART                          ///
///                                                       ///
/////////////////////////////////////////////////////////////

const unsigned char UBX_HEADER[] = { 0xB5, 0x62 };
struct NAV_POSLLH {
  unsigned char cls;
  unsigned char id;
  unsigned short len;
  unsigned long iTOW;
  long lon;
  long lat;
  long height;
  long hMSL;
  unsigned long hAcc;
  unsigned long vAcc;
};
NAV_POSLLH posllh;
double gps_lat, gps_lon;
