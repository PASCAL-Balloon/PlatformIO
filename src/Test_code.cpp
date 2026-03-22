// // This file is just for running whatever test code we want
// #include "Arduino.h"
// #include <Wire.h>
// #include <SPI.h>
// #include <Adafruit_BMP3XX.h>
// #include <SparkFun_u-blox_GNSS_Arduino_Library.h> 
// #include "DFRobot_OxygenSensor.h"
// #include "Data.h"
// #include "Config.h"
// #include "PASCAL.h"
// #include "components/Logger.h"
// #include "components/BMP.h"
// #include "components/Oxygen.h"
// #include "components/GPS.h"
// #include "components/hum.h"
// #include "components/PumpController.h"

// void setup() {
//     pinMode(4, OUTPUT);
//     pinMode(3, OUTPUT);
//     pinMode(LED_BUILTIN, OUTPUT);    
//     initPins();
// 	initComponents();
// 	initLEDs(3000);     
// }

// void loop() {

//     blinky();
    
//     //digitalWrite(4, HIGH);
//     // digitalWrite(4, HIGH); 
//     // digitalWrite(3, HIGH); 
//     // digitalWrite(LED_BUILTIN, HIGH);     
//     // delay(3000);  
//     // //digitalWrite(4, LOW);
//     // digitalWrite(4, LOW);
//     // digitalWrite(3, LOW); 
//     // digitalWrite(LED_BUILTIN, LOW);    
//     // delay(3000);
//     // blinky(); 
// 	// collectData();
// 	// logger.writeTelemetry();
// 	// updateState();
// 	// if (data.state == PASSIVE) controller.sampling();
// 	// if (data.state == INITIALIZATION) displayErrors();

// }

#include "Arduino.h"
#include <ADS1115_WE.h> 

// #include <DHT.h>

// DHT sensor specs
// #define DHTPIN 2
// #define DHTTYPE DHT11
// DHT dht (DHTPIN, DHTTYPE);

// Serial: 204240462
  float sensitivityOX = 380.0 / 1000;             // Sensor's sensitivity (mV/ppb)
  float WEcOX = 0;                                // Corrected WE output in ppb/ppm 
  float WEuOX = 0;                                // Uncorrected raw WE output (analog pin)
  float AEuOX = 0;                                // Uncorrected raw AE output (analog pin)
  float WEoOX = 216;                              // WE Zero (mV)
  float WEeOX = 223;                              // WE Eletronic Zero (mV)
  float AEoOX = 230;                              // Aux Zero (mV)
  float AEeOX = 235;                              // Aux Eletronic Zero (mV)
  
  float OXConcentrationPPB = WEcOX / sensitivityOX;
  float OXConcentrationPPM = OXConcentrationPPB / 1000;

// Correction factors that depends on the temperature
float temperature = 0;      // Temperature value
float hum = 0;         // hum value 
float NtOX = 0;             // nT
float KtOX = 0;             // kT
float KttOX = 0;            // kT'
float KtttOX = 0;           // kT''

void correctionFactors(float temperature){
    if(temperature < -25)                       { NtOX =  0.9; KtOX =  0.5; KttOX =  0.5; KtttOX =    1.0; };
    if(temperature >= -25 && temperature < -15) { NtOX =  0.9; KtOX =  0.5; KttOX =  0.5; KtttOX =    1.0; };
    if(temperature >= -15 && temperature <  -5) { NtOX =  1.0; KtOX =  0.6; KttOX =  0.5; KtttOX =    1.0; };
    if(temperature >=  -5 && temperature <   5) { NtOX =  1.3; KtOX =  0.8; KttOX =  0.6; KtttOX =    1.0; };
    if(temperature >=   5 && temperature <  15) { NtOX =  1.5; KtOX =  0.9; KttOX =  0.6; KtttOX =    1.0; };
    if(temperature >=  15 && temperature <  25) { NtOX =  1.7; KtOX =  1.0; KttOX =  1.0; KtttOX =    1.0; };
    if(temperature >=  25 && temperature <  35) { NtOX =  2.0; KtOX =  1.2; KttOX =  2.8; KtttOX =    8.5; };
    if(temperature >=  35 && temperature <  45) { NtOX =  2.5; KtOX =  1.5; KttOX =  5.0; KtttOX =   23.0; };
    if(temperature >=  45)                      { NtOX =  3.7; KtOX =  2.2; KttOX =  5.3; KtttOX =  103.0; };
}

float equationWEc(float WEc, float WEu, float WEe, float Nt, float AEu, float AEe){
  // Algorithms to correct the WE output for the effects of temperature
  WEc = ((WEu - WEe) - (Nt * (AEu - AEe)));
  //WEc = (WEu - WEe) - Kt * (WEo / AEo) * (AEu - AEe);
  //WEc = (WEu - WEe) - (WEo - AEo) - Ktt * (AEu - AEe);
  //WEc = (WEu - WEe) - WEo - Kttt;

  return WEc;
}


ADS1115_WE adc;


void setup() {
    Serial.begin(9600);
    while (!Serial);
    adc = ADS1115_WE(0x48);
//   dht.begin();
}

void loop() {
  // DHT data (Temperature and hum)
    temperature = 25.0;//dht.readTemperature();
    hum = 49.0;//dht.readhum();
    correctionFactors(temperature);
    
  // OX-B431 (Oxidising - Ozone + Nitrogen Dioxide)
    adc.setCompareChannels(ADS1115_COMP_0_GND);
    WEuOX = adc.getResult_mV() * (5.0 / 1024) * 1000; ; // alternative: getResult_mV for Millivolt

    Serial.println("Just got AEuOX");

    //WEuOX = analogRead(A0) * (5.0 / 1024) * 1000;
    //AEuOX = analogRead(A1) * (5.0 / 1024) * 1000;

    adc.setCompareChannels(ADS1115_COMP_1_GND);
    AEuOX = adc.getResult_mV() * (5.0 / 1024) * 1000;; 

    Serial.println("Just got AEuOX");

   
    WEcOX = equationWEc(WEcOX, WEuOX, WEeOX, NtOX, AEuOX, AEeOX);
    OXConcentrationPPB = WEcOX / sensitivityOX;
    OXConcentrationPPM = OXConcentrationPPB / 1000;

    Serial.println("Just got WEcOX");

  /* Print DHT informations */
    // Temperature and hum
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print(" C, hum: ");
    Serial.print(hum);
    Serial.print("\%");
    Serial.print("\r\n");

  /* Print Gas informations */
    // OX-B431 (Oxidising -> Ozone + Nitrogen Dioxide)
    Serial.print("OX-B431 (Oxidising -> Ozone + Nitrogen Dioxide): ");
    Serial.print("WEu Raw value: ");
    Serial.print(WEuOX);
    Serial.print(", AEu Raw Value: ");
    Serial.print(AEuOX);
    Serial.print(", WEc Raw Value: ");
    Serial.print(WEcOX);
    Serial.println("");
    Serial.print("Concentrations: ");
    Serial.print(OXConcentrationPPB);
    Serial.print(" ppb, ");
    Serial.print(OXConcentrationPPM);
    Serial.print(" ppm");
    Serial.println("\r\n\r\n");

  delay(2000); // Time between measurement
}