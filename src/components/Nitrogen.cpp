#include "components/Nitrogen.h"
#include "PASCAL.h"

// TODO Check over the logic for the ADC/update for the new sensor
float NO2Sensor::readChannel(ADS1115_MUX channel) {
  float voltage = 0.0;
  adc.setCompareChannels(channel);
  voltage = adc.getResult_mV(); // alternative: getResult_mV for Millivolt
  return voltage;
}

NO2Sensor::NO2Sensor() {
    adc = ADS1115_WE(0x48);
}

void NO2Sensor::init() {
    config.WEOffset;
    config.AuxOffset;
    config.sensitivity;
    config.temperatureMultiplier;

    if(!adc.init()){
       data.error = data.error > NO2_ERROR ? data.error : NO2_ERROR;	
       logger.writeError("ADC Initialization Error");
    }

   
  adc.setVoltageRange_mV(ADS1115_RANGE_6144);
  adc.setCompareChannels(ADS1115_COMP_0_GND);
  adc.setMeasureMode(ADS1115_CONTINUOUS); 
}


//Right now we have it set so it read pin 0 and 1 and uses the differences to calculate
// the NO2 concentration. This will probably updated later
void NO2Sensor::updateData() {
    adc.setCompareChannels(ADS1115_COMP_0_GND);
    float WE = adc.getResult_mV(); // alternative: getResult_mV for Millivolt

    adc.setCompareChannels(ADS1115_COMP_1_GND);
    float Aux = adc.getResult_mV(); 

    data.WE_real = WE; // mV
    data.Aux_real = Aux;

    // data.WE_real = WE - config.WEOffset;   // mV
    // data.Aux_real = config.temperatureMultiplier * (Aux - config.AuxOffset);  // mV
    // data.atmoData.no2 = (data.WE_real - data.Aux_real)/(config.sensitivity);

    data.atmoData.no2 = (data.WE_real - data.Aux_real)/(config.sensitivity);
}


void NO2Sensor::getRawData() {

    adc.setCompareChannels(ADS1115_COMP_0_GND);
    data.pin0 = adc.getResult_mV();

    adc.setCompareChannels(ADS1115_COMP_1_GND);
    data.pin1 = adc.getResult_mV(); 

    adc.setCompareChannels(ADS1115_COMP_2_GND);
    data.pin2 = adc.getResult_mV(); 

    adc.setCompareChannels(ADS1115_COMP_3_GND);
    data.pin3 = adc.getResult_mV(); 
   
    Serial.println(String(data.pin0) + ", " + String(data.pin1)+  ", " + String(data.pin2)+  ", " + String(data.pin3));
    logger.write(String(data.pin0) + "," + String(data.pin1)+  "," + String(data.pin2)+  "," + String(data.pin3));
}