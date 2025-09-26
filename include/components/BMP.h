#pragma once 

#include <Adafruit_BMP3XX.h>
#include "Data.h"
#include "Config.h"

class BMP {

    Error error;

public:
    Adafruit_BMP3XX bmp;

    // Initializes the BMP 
    void init();

    // Reads from the BMP and the reading
    void updateAltitude();
    void updatePressure();
    void updateTemperature();

	// Refreshes each of the datapoints
	void updateData();

    // Polls to see if there is an error
    Error getError();

};
