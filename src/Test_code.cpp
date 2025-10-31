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
    pinMode(4, OUTPUT);
    pinMode(3, OUTPUT);
    pinMode(LED_BUILTIN, OUTPUT);         
}

void loop() {
    //digitalWrite(4, HIGH);
    digitalWrite(4, HIGH); 
    digitalWrite(3, HIGH); 
    digitalWrite(LED_BUILTIN, HIGH);     
    delay(3000);  
    //digitalWrite(4, LOW);
    digitalWrite(4, LOW);
     digitalWrite(3, LOW); 
    digitalWrite(LED_BUILTIN, LOW);    
    delay(3000);
}

