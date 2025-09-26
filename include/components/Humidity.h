#pragma once

#include "Config.h"
#include <Adafruit_HDC302x.h>
#include "Data.h"

class HumiditySensor {
private:

    Adafruit_HDC302x humidity; 

    Error error;

public:

    // Initializes the humid sensor
    void turnOn();

	// Updates the data we have from the sensor
	void updateData();

    // Returns the current error
    Error getError();

};

