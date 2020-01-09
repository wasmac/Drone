bool pid_sign_check(int x, int y){ //if signs are opposite returns true
  if(((x^ y) < 0)){
    return 0;
    }else return 1;
}

void pid_init(){
  pid_setpoint_yaw = 0;
  pid_setpoint_pitch = 0;
  pid_setpoint_roll = 0;

  pid_yaw.setTimeStep(1);
  pid_pitch.setTimeStep(1);
  pid_roll.setTimeStep(1);

}

void pid_desired_ypr(){ // for controll
  desired_yaw = map(wifi_joystick_1_x, 1000, 2000, -180, 180);
  desired_pitch = map(wifi_joystick_2_y, 1000, 2000, -30, 30);
  desired_roll = map(wifi_joystick_2_x, 1000, 2000, -30, 30);
  
}

void pid_compute(){
  pid_desired_ypr();
  
  pid_setpoint_yaw = desired_yaw;
  pid_setpoint_pitch = desired_pitch;
  pid_setpoint_roll = desired_roll;
  
  pid_input_yaw = ypr[0]* 180/PI;
  pid_input_pitch = ypr[1]* 180/PI;
  pid_input_roll = ypr[2]* 180/PI;

  pid_yaw.setGains(kp_yaw, ki_yaw, kd_yaw); // new code
  pid_pitch.setGains(kp_pitch, ki_pitch, kd_pitch);
  pid_roll.setGains(kp_roll, ki_roll, kd_roll);

  pid_yaw.run();
  pid_pitch.run();
  pid_roll.run();




  
  //  stop(); -- might be worf tryin
}

void pid_to_ms(){
  pid_final_yaw = map(pid_output_yaw, -180, 180, -300, 300);
  pid_final_pitch = map(pid_output_pitch, -30, 30, -300, 300);
  pid_final_roll = map(pid_output_roll, -30, 30, -300, 300);
}


void pid_reset(){       //resets i and d values -- does not stop the calculations
  pid_yaw.reset();
  pid_pitch.reset();
  pid_roll.reset();  
  
}


bool pid_windup_yaw(){
  if(pid_output_yaw >= 178)   return 1;
  if(pid_output_yaw <= -178)  return 1;
  else return 0;
}

bool pid_windup_pitch(){
  if(pid_output_pitch >= 28)  return 1;
  if(pid_output_pitch <= -28) return 1;
  else return 0;  
}

bool pid_windup_roll(){
  if(pid_output_roll >= 28)  return 1;
  if(pid_output_roll <= -28) return 1;
  else return 0;  
}

bool pid_output_error_sign_check_yaw(){
  pid_error_yaw = pid_setpoint_yaw - pid_input_yaw;
  if(pid_sign_check(pid_output_yaw, pid_error_yaw) == 1) return 1;
  else return 0;
  
}

bool pid_output_error_sign_check_pitch(){
  pid_error_pitch = pid_setpoint_pitch - pid_input_pitch;
  if(pid_sign_check(pid_output_pitch, pid_error_pitch) == 1) return 1;
  else return 0;
  
}

bool pid_output_error_sign_check_roll(){
  pid_error_roll = pid_setpoint_roll - pid_input_roll;
  if(pid_sign_check(pid_output_roll, pid_error_roll) == 1) return 1;
  else return 0;
  
}

void pid_saturation_control_yaw(){
  if(pid_windup_yaw() == pid_output_error_sign_check_yaw()) ki_yaw = 0;
  else ki_yaw = 0.36;
}

void pid_saturation_control_pitch(){
  if(pid_windup_pitch() == pid_output_error_sign_check_pitch()) ki_pitch = 0;
  else ki_pitch = 0.36;
}

void pid_saturation_control_roll(){
  if(pid_windup_roll() == pid_output_error_sign_check_roll()) ki_roll = 0;
  else ki_roll = 0.36;
}

void pid_saturation_control(){
  pid_saturation_control_yaw();
  pid_saturation_control_pitch();
  pid_saturation_control_roll();
}
