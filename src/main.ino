#include "libs_vars.h"                                                 //this header sotres all included libs, global variables and sturctures

TaskHandle_t core0, core1;

void core0_task(void* parameter){                                      //task working on core 0 of ESP32 
  for(;;){
          mpu_get_data();
          wifi_connect();                                              //WiFi handler                                 
          wifi_joysticks_values();                                     //takes joy values from app
          motors_on_off();                                             //turns motors on or off if left joy value < 150
          d_mot_on_off = mot_on_off?1.0:0.0;                           //bool to double
          dtostrf(d_mot_on_off, 0, 1, RemoteXY.text_1);                //show if on or off in application window
          dtostrf(kp_yaw*100, 0, 1, RemoteXY.text_2);                  //show kp value <-- testing purposes
    }  
}

void core1_task(void* parameter){                                      //task working on core 1 of ESP32
  for(;;){
          pid_desired_ypr();                                           //maping function for calcualting desired angles for controlling the drone with app
          //calibration();                                             //function used for calibration of kp ki and kd values manually in app
          pid_compute();                                               //runs pid calculations (for all three angles)
          pid_to_ms();                                                 //maps pid calculations into ms so they can be used as motor speed
          pid_saturation_control();                                    //windup check for integral part of pid controller
          calculate_motor_speed();                                     //sums up values from PID and joysticks
          adjust_motrs_speed();                                        //sends calculculated speed to all motors
    }  
}

void setup() {

  if(mot_calibration == true){                                         //calibrates motor esc so all of them start at same ms value if true
    calibrate_motors();
    }else arm_motors();
  
  wifi_init();
  pid_init();
  mpu_join_i2c_bus();
  if (printMPU == true){                                              //prints Yaw Pitch Roll values to serial monitor if true
    mpu_data_with_serial_init();
    mpu_gyro_offset();
    mpu_work_check_with_serial();
  }else{
     mpu_data_init();
     mpu_gyro_offset();
     mpu_work_check_data();
  }

  xTaskCreatePinnedToCore(
              core0_task,       /* Task function. */
              "core0",          /* String with name of task. */
              10000,            /* Stack size in bytes. */
              NULL,             /* Parameter passed as input of the task */
              1,                /* Priority of the task. */
              &core0,           /* Task handle. */
              1);               /* Core */     

  xTaskCreatePinnedToCore(
              core1_task,       /* Task function. */
              "core1",          /* String with name of task. */
              10000,            /* Stack size in bytes. */
              NULL,             /* Parameter passed as input of the task */
              1,                /* Priority of the task. */
              &core1,           /* Task handle. */
              1);               /* Core */ 

}

void loop() {
  delay(1000); // idle state so it doesnt 'eat' cpu
}
