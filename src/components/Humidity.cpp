
#include <Adafruit_HDC302x.h>
#include "components/Humidity.h"
#include "PASCAL.h"

void HumiditySensor::turnOn() {
    if(!humidity.begin()){
		data.error = data.error > HUMID_ERROR ? data.error : HUMID_ERROR;	
		logger.writeError("Humidity Sensor Initialization Error");
	}
	
	// If we switch to this it should make everything way faster
	//I COMMENTED IT OUT
	//humidity.setAutoMode(AUTO_MEASUREMENT_0_5MPS_LP0);
}

void HumiditySensor::updateData() {

	double humid;
	double temp;
  	
	// humidity.readTemperatureHumidityOnDemand(temp, humid, TRIGGERMODE_LP0);
	// Changing to this can greatly increase loop speed
	//humidity.readAutoTempRH(temp, humid);
	humidity.readTemperatureHumidityOnDemand(temp, humid, TRIGGERMODE_LP0);


	data.atmoData.humidity = humid;
	data.atmoData.humiditySensorTemperature = temp;
}
