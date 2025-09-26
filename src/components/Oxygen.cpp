
#include "components/Oxygen.h"
#include "PASCAL.h"

void OxygenSensor::init() {
    if(!oxygen.begin(ADDRESS_3)){
        data.error = data.error > O2_ERROR ? data.error : O2_ERROR;
        error = O2_ERROR;
    } else {
        data.error = data.error == O2_ERROR ? O2_ERROR : data.error;
        error = NO_ERROR;
    }
}

void OxygenSensor::updateData() {
    data.atmoData.oxygen = oxygen.getOxygenData(collectionNumber);
}

Error OxygenSensor::getError() {
    return error;
}