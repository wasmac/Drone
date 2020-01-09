void wifi_init(){
  RemoteXY_Init();
}

void wifi_connect(){
  RemoteXY_Handler();
}

void wifi_joysticks_values(){
  wifi_joystick_1_x = map(RemoteXY.joystick_1_x, -100, 100, 1000, 2000);
  wifi_joystick_1_y = map(RemoteXY.joystick_1_y, -100, 100, 0, 2000); // because throttle middle point should be 0 for testing purposes
  wifi_joystick_2_x = map(RemoteXY.joystick_2_x, -100, 100, 1000, 2000);
  wifi_joystick_2_y = map(RemoteXY.joystick_2_y, -100, 100, 1000, 2000);
}

void wifi_geo_position(){
  gps_calc_pos();
  dtostrf(gps_lat, 0, 1, RemoteXY.text_1); // txt1 lat
  dtostrf(gps_lon, 0, 1, RemoteXY.text_2); // txt2 lon
}
