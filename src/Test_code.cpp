// This file is just for running whatever test code we want
#include "Arduino.h"
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP3XX.h>
#include <SparkFun_u-blox_GNSS_Arduino_Library.h> 
#include "DFRobot_OxygenSensor.h"
#include "Data.h"
#include "Config.h"
#include "PASCAL.h"
#include "components/Logger.h"
#include "components/BMP.h"
#include "components/Oxygen.h"
#include "components/GPS.h"
#include "components/Humidity.h"
#include "components/PumpController.h"

void setup() {   
    initPins();
	initComponents();
    logger.write("Pin 0, Pin 1, Pin 2, Pin 3");
}

void loop() {

  
    no2.getRawData();
    //Serial.println(String(data.pin0) + ", " + String(data.pin1)+  ", " + String(data.pin2)+  ", " + String(data.pin3));
    logger.write(String(data.pin0) + "," + String(data.pin1)+  "," + String(data.pin2)+  "," + String(data.pin3));
}

