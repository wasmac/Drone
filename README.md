# RTOS: Maintaining drone position in real time constraints

This Arduino sketch provides a **flight controller** for an X quadcopter based on an **DoIt ESP32 Devkit V1** board and the **MPU6050 sensor** along with Android/ iOS application for controlling it.



## Files

Each .ino script consisnt of varius functions for different drone activities eg. mpu_data collects data from mpu6050.
For includes, variables and structures storing I used header file: libs_vars.h

## Requirements
### Arduino IDE ESP32 libraries:
Wifi
RemoteXY
ESP32Servo
I2Cdev
AutoPID
MPU6050_6Axis_MotionApps20
Wire

### Hardware:
DoIt ESP32 Devkit V1
MPU6050
4 ESC
4 Brushless Motors
LiPol Battery
Connectors
Cables

## Board and Pinout

Board used in this project is ==DoIt ESP32 Devkit V1==.
Thanks to dual core and WiFi/ BLE this board was perfect fit for Drone.
> Although small range of antena and the fact that it cant be replaced should be taken into consideration before choosing it.

![Board Pinout](https://photos.app.goo.gl/BrkvT7EPfsxajjn17)




### Hardware Connection

![Connections](https://imgur.com/a/Aexga9W)



## PID controller

Fast graph made using [Mermaid](https://mermaidjs.github.io/). For improvement in the future.


```mermaid
graph LR
    A[Error] --> B(Kp)
    A --> L[Sign]
	A --> C(Kd)
	A --> D(Ki)
	B --> E{PID}
	C --> E
	G[0] --> F
	D --> F((or))
	F --> E
	E --> H(( ))
	H --> K[Sign]
	L --> M[==]
	K --> M
	M --> N[AND]
	N -->|decisive| F
	H --> I(Clamping Saturation Limit)
	I --> J(( ))
	J --> O[=/=]
	H --> O
	O --> N
	J --> P[Actuator]
	P --> R>Output]
	
```


## Android/ iOS App

Application was made using [RemoteXY](http://remotexy.com/en/) framework.

![Application appearance](https://github.com/wasmac/Drone/blob/master/images/App.PNG)

> App made for this project was made using **free** trial version.




## Project status

Currently under **active** development.



