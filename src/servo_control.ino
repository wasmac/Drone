void calibrate_motors(){ //select the motors
  pinMode(MOT_LED, OUTPUT);
  digitalWrite(MOT_LED, LOW);
  mot_R_F.attach(33, MIN_PULSE_LENGTH, MAX_PULSE_LENGTH);
  mot_R_B.attach(25, MIN_PULSE_LENGTH, MAX_PULSE_LENGTH); //12
  mot_L_F.attach(32, MIN_PULSE_LENGTH, MAX_PULSE_LENGTH);
  mot_L_B.attach(26, MIN_PULSE_LENGTH, MAX_PULSE_LENGTH);
  delay(50); //set motors to programing mode before connecting the battery
  mot_R_F.writeMicroseconds(MAX_PULSE_LENGTH);
  mot_R_B.writeMicroseconds(MAX_PULSE_LENGTH);
  mot_L_F.writeMicroseconds(MAX_PULSE_LENGTH);
  mot_L_B.writeMicroseconds(MAX_PULSE_LENGTH);
  delay(2000);
  digitalWrite(MOT_LED, HIGH);
  delay(7000); // 7 sec delay- time for battery connection
  digitalWrite(MOT_LED, LOW);
  mot_R_F.writeMicroseconds(MIN_PULSE_LENGTH);
  mot_R_B.writeMicroseconds(MIN_PULSE_LENGTH);
  mot_L_F.writeMicroseconds(MIN_PULSE_LENGTH);
  mot_L_B.writeMicroseconds(MIN_PULSE_LENGTH);  
  //motors should be calibrated and all of them should start at same time.
  }


void arm_motors(){
  mot_R_F.attach(13, MIN_PULSE_LENGTH, MAX_PULSE_LENGTH);
  delay(5);
  mot_R_F.writeMicroseconds(1000);
  
  mot_R_B.attach(12, MIN_PULSE_LENGTH, MAX_PULSE_LENGTH);
  delay(5);
  mot_R_B.writeMicroseconds(1000);
  
  mot_L_F.attach(14, MIN_PULSE_LENGTH, MAX_PULSE_LENGTH);
  delay(5);
  mot_L_F.writeMicroseconds(1000);
  
  mot_L_B.attach(27, MIN_PULSE_LENGTH, MAX_PULSE_LENGTH);
  delay(5);
  mot_L_B.writeMicroseconds(1000);
  delay(100);
  }

void motors_on_off(){
  if(wifi_joystick_1_y <= 150 && mot_flag == false){
    mot_on_off = !mot_on_off;
    mot_flag = true;
    } 
  if(wifi_joystick_1_y > 150 && mot_flag == true){
    mot_flag = false;
    }
  }

void calculate_motor_speed(){
  if(mot_on_off == true){
    mot_R_F_speed = wifi_joystick_1_y - pid_final_roll + pid_final_pitch + pid_final_yaw;
    if(mot_R_F_speed > 2000) mot_R_F_speed = 2000;
    if(mot_R_F_speed<1000) mot_R_F_speed = 1000;
    mot_R_B_speed = wifi_joystick_1_y - pid_final_roll - pid_final_pitch + pid_final_yaw; //1
    if(mot_R_B_speed > 2000) mot_R_B_speed = 2000;
    if(mot_R_B_speed<1000) mot_R_B_speed = 1000;
    mot_L_F_speed = wifi_joystick_1_y + pid_final_roll + pid_final_pitch + pid_final_yaw;
    if(mot_L_F_speed > 2000) mot_L_F_speed = 2000;
    if(mot_L_F_speed<1000) mot_L_F_speed = 1000;
    mot_L_B_speed = wifi_joystick_1_y + pid_final_roll - pid_final_pitch - pid_final_yaw ; 
    if(mot_L_B_speed > 2000) mot_L_B_speed = 2000;
    if(mot_L_B_speed<1000) mot_L_B_speed = 1000;
    }


  if(mot_on_off == false){
    mot_R_F_speed = 1000;
    mot_R_B_speed = 1000;
    mot_L_F_speed = 1000;
    mot_L_B_speed = 1000;

    pid_reset(); //reset pid values 
    }  
  }

void adjust_motrs_speed(){
  mot_R_F.writeMicroseconds(mot_R_F_speed);
  mot_R_B.writeMicroseconds(mot_R_B_speed);
  mot_L_F.writeMicroseconds(mot_L_F_speed);
  mot_L_B.writeMicroseconds(mot_L_B_speed);
  }
