void gps_calcChecksum(unsigned char* CK) {
  memset(CK, 0, 2);
  for (int i = 0; i < (int)sizeof(NAV_POSLLH); i++) {
    CK[0] += ((unsigned char*)(&posllh))[i];
    CK[1] += CK[0];
  }
}

bool gps_process() {
  static int fpos = 0;
  static unsigned char checksum[2];
  const int payloadSize = sizeof(NAV_POSLLH);

  while ( Serial.available() ) {
    byte c = Serial.read();
    if ( fpos < 2 ) {
      if ( c == UBX_HEADER[fpos] )
        fpos++;
      else
        fpos = 0;
    }
    else {
      if ( (fpos-2) < payloadSize )
        ((unsigned char*)(&posllh))[fpos-2] = c;

      fpos++;

      if ( fpos == (payloadSize+2) ) {
        gps_calcChecksum(checksum);
      }
      else if ( fpos == (payloadSize+3) ) {
        if ( c != checksum[0] )
          fpos = 0;
      }
      else if ( fpos == (payloadSize+4) ) {
        fpos = 0;
        if ( c == checksum[1] ) {
          return true;
        }
      }
      else if ( fpos > (payloadSize+4) ) {
        fpos = 0;
      }
    }
  }
  return false;
}


void gps_calc_pos(){ // starts in wifi although can be also started in main loop
  if ( gps_process() ) {
    gps_lat = posllh.lat;
    gps_lat = gps_lat/10000000;
    gps_lon = posllh.lon;
    gps_lon = gps_lon/10000000;
  }
}
