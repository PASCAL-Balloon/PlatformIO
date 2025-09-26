// #include <SparkFun_u-blox_GNSS_v3.h> //http://librarymanager/All#SparkFun_u-blox_GNSS_v3
#include <SparkFun_u-blox_GNSS_Arduino_Library.h> 

#include "Timers.h"
#include "components/GPS.h"
#include "Config.h"
#include "Data.h"
#include "PASCAL.h"



void GPS::init() {
	Wire.setSDA(12);
	Wire.setSCL(13);
	Wire.begin();

    // Initialization
	// delay(3000); // This delay is not needed with new error handling
    if (!gps.begin()) {
		data.error = data.error > GPS_ERROR ? data.error : GPS_ERROR;
		error = GPS_ERROR;	
    } else {
		error = NO_ERROR;
		data.error = data.error == GPS_ERROR ?  NO_ERROR : data.error;
	}

    // Updating settings
    gps.setI2COutput(COM_TYPE_UBX);
    gps.setNavigationFrequency(5);
    if (gps.setDynamicModel(DYN_MODEL_AIRBORNE4g) == false) {
		data.error = data.error > GPS_ERROR ? data.error : GPS_ERROR;	
	}
    gps.saveConfiguration();

	// TODO remove this block
	// Not needed due to updated error logging
	// Waiting for a satellite lock
	// long startTime = millis();
    // while (gps.getSIV() < 3) {
    //     Serial.print("Waiting for lock, SIV: ");
	// 	Serial.println(gps.getSIV());
		
	// 	// Limiting the wait to 3 seconds
	// 	if (millis() - startTime > 3000) break;
    // }

}

void GPS::update() {

	// Checking for new data
	gps.checkUblox();
	if (gps.getPVT() == false) {
		return;  
	}
	
	data.gpsData.pos = { 
		((double)gps.getLongitude() * pow(10, -7)),
		((double)gps.getLatitude() * pow(10, -7)),
		gps.getAltitude() / 1000.0
	};

  	data.gpsData.SIV = gps.getSIV();
	data.gpsData.time = {
		(int)gps.getYear(),
		(int)gps.getMonth(),
		(int)gps.getDay(),
		(int)gps.getHour(),
		(int)gps.getMinute(),
		(int)gps.getSecond(),
	};
  
  	tick.reset();
}

int GPS::getSIV() {
	if (tick.isComplete()) {
		update();
	}
    return data.gpsData.SIV;
}

void GPS::updateData() {
	if (tick.isComplete()) {
		update();
	}
}

Error GPS::getError() {
	// Updates warning if we don't have the larger gps error
	if (error != GPS_ERROR && getSIV() < 3) {
		error = LOW_SIV_WARNING;
		data.error = data.error > LOW_SIV_WARNING ? data.error : LOW_SIV_WARNING;
	}
	return error;
}