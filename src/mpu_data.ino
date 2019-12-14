void mpu_dmp_ready() { //dmpDataReady()
  mpuInterrupt = true;
}

void mpu_join_i2c_bus(){
  #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
      Wire.begin();
      Wire.setClock(400000);
  #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
      Fastwire::setup(400, true);
  #endif
  }

void mpu_data_with_serial_init(){
  Serial.begin(115200);
  while (!Serial);
  Serial.println(F("Initializing I2C devices..."));
  mpu.initialize();
  pinMode(INTERRUPT_PIN, INPUT);
  Serial.println(F("Testing device connections..."));
  Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));
  delay(5);
  Serial.println(F("Initializing DMP..."));
  devStatus = mpu.dmpInitialize();
  }

void mpu_work_check_with_serial(){
  if (devStatus == 0) {
      mpu.CalibrateAccel(6);
      mpu.CalibrateGyro(6);
      mpu.PrintActiveOffsets();
      Serial.println(F("Enabling DMP..."));
      mpu.setDMPEnabled(true);
      Serial.print(F("Enabling interrupt detection (Arduino external interrupt "));
      Serial.print(digitalPinToInterrupt(INTERRUPT_PIN));
      Serial.println(F(")..."));
      attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), mpu_dmp_ready, RISING);
      mpuIntStatus = mpu.getIntStatus();
      Serial.println(F("DMP ready! Waiting for first interrupt..."));
      dmpReady = true;
      packetSize = mpu.dmpGetFIFOPacketSize();
  } else {
      Serial.print(F("DMP Initialization failed (code "));
      Serial.print(devStatus);
      Serial.println(F(")"));
  }
  pinMode(LED_PIN, OUTPUT);
  }

void mpu_data_init(){
  mpu.initialize();
  pinMode(INTERRUPT_PIN, INPUT);
  delay(5);
  devStatus = mpu.dmpInitialize();
  }
void mpu_work_check_data(){
  if (devStatus == 0) {
    // Calibration Time: generate offsets and calibrate our MPU6050
    mpu.CalibrateAccel(6);
    mpu.CalibrateGyro(6);
    mpu.PrintActiveOffsets();
    mpu.setDMPEnabled(true);
    attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), mpu_dmp_ready, RISING);
    mpuIntStatus = mpu.getIntStatus();
    dmpReady = true;
    packetSize = mpu.dmpGetFIFOPacketSize();
}

  pinMode(LED_PIN, OUTPUT);
  }

void mpu_gyro_offset(){
  mpu.setXGyroOffset(220);
  mpu.setYGyroOffset(76);
  mpu.setZGyroOffset(-85);
  mpu.setZAccelOffset(1688);
  }

void mpu_get_data(){
  if (!dmpReady) return;
  while (!mpuInterrupt && fifoCount < packetSize) {
      if (mpuInterrupt && fifoCount < packetSize) {
        fifoCount = mpu.getFIFOCount();
      }  
  }
  mpuInterrupt = false;
  mpuIntStatus = mpu.getIntStatus();
  fifoCount = mpu.getFIFOCount();
  if(fifoCount < packetSize){}
  else if ((mpuIntStatus & _BV(MPU6050_INTERRUPT_FIFO_OFLOW_BIT)) || fifoCount >= 1024) {
    mpu.resetFIFO();
  } else if (mpuIntStatus & _BV(MPU6050_INTERRUPT_DMP_INT_BIT)) {
      while(fifoCount >= packetSize){
        mpu.getFIFOBytes(fifoBuffer, packetSize);
        fifoCount -= packetSize;
        }
      mpu.dmpGetQuaternion(&q, fifoBuffer);
      mpu.dmpGetGravity(&gravity, &q);
      mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
      if(printMPU == true){
        Serial.print("ypr\t");
        Serial.print(ypr[0] * 180/PI);
        Serial.print("\t");
        Serial.print(ypr[1] * 180/PI);
        Serial.print("\t");
        Serial.println(ypr[2] * 180/PI);
        }
      blinkState = !blinkState;
      digitalWrite(LED_PIN, blinkState);
    }
  }
