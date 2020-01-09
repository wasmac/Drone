void calibration(){ // to calibrate turn off pid_desired_ypr()
  calib_x = wifi_joystick_2_x;
  calib_y = wifi_joystick_2_y;
  calib_z = wifi_joystick_1_x;

  if(calib_x >1850 && calib_flag == false){ 
    //ki_yaw += 0.001;
    ki_pitch += 0.001;
    ki_roll += 0.001;
    calib_pid_gain = ki_pitch;
    calib_flag = true;
    }

  if(calib_x<1250 &&  calib_flag == false){ 
    //ki_yaw -= 0.001;
    ki_pitch -= 0.001;
    ki_roll -= 0.001;
    calib_pid_gain = ki_pitch;
    calib_flag = true;
    } 

  if(calib_y>1850 && calib_flag == false){ 
    //kp_yaw += 0.01;
    kp_pitch += 0.01;
    kp_roll += 0.01;
    calib_pid_gain = kp_pitch;
    calib_flag = true;
    }

  if(calib_y<1250 && calib_flag == false){ 
    //kp_yaw -= 0.01;
    kp_pitch -= 0.01;
    kp_roll -= 0.01;
    calib_pid_gain = kp_pitch;
    calib_flag = true;
    }

  if(calib_z>1850 && calib_flag_2 == false){ 
    //kd_yaw += 0.01;
    kd_pitch += 100;
    kd_roll += 100;
    calib_pid_gain = kd_pitch;
    calib_flag_2 = true;
    }

  if(calib_z<1250 && calib_flag_2 == false){ 
    //kd_yaw -= 0.01;
    kd_pitch -= 100;
    kd_roll -= 100;
    calib_pid_gain = kd_pitch;
    calib_flag_2 = true;
    }

  if(kp_yaw < 0) kp_yaw = 0.0;
  if(kp_pitch < 0) kp_pitch = 0.0;
  if(kp_roll < 0) kp_roll = 0.0;
  if(ki_yaw < 0) ki_yaw = 0.0;
  if(ki_pitch < 0) ki_pitch = 0.0;
  if(ki_roll < 0) ki_roll = 0.0;
  if(kd_yaw < 0) kd_yaw = 0.0;
  if(kd_pitch < 0) kd_pitch = 0.0;
  if(kd_roll < 0) kd_roll = 0.0;
  
  if(calib_x == 1500 && calib_y == 1500 && calib_flag == true) calib_flag = false;
  if(calib_z == 1500 && calib_flag_2 == true) calib_flag_2 = false;
    
}

void show_calib(){
  Serial.print("kp ki kd\t");
  Serial.print(kp_yaw);
  Serial.print("\t");
  Serial.print(ki_yaw);
  Serial.print("\t");
  Serial.println(kd_yaw);
  }
