//
//  ArchiveStruct.h
//  
//
//  Created by Denis Prša on 10. 05. 16.
//
//

#ifndef ArchiveStruct_h
#define ArchiveStruct_h

#include <string.h>

using namespace std;

// STRUCT FOR COPYING FROM BUFFER
struct ARDATA_b_t
{
    uint16_t date;
    uint16_t time;
    uint16_t outside;
    uint16_t hightOut;
    uint16_t lowOut;
    uint16_t rainfall;
    uint16_t highRainFall;
    uint16_t barometer;
    uint16_t solar;
    uint16_t numwisa;
    uint16_t inside;
    uint8_t insideH;
    uint8_t outsideH;
    uint8_t avgWindSpeed;
    uint8_t highWindSpeed;
    uint8_t directionHighWindSpeed;
    uint8_t directionDominantWind;
    uint8_t UVindex;
    uint8_t ET;
    uint16_t highSolarRadiation;
    uint8_t highUVindex;
    uint8_t forecastRule;
    uint8_t leafTemp1;
    uint8_t leafTemp2;
    uint8_t leafWetnesses1;
    uint8_t leafWetnesses2;
    uint8_t soilTemperature1;
    uint8_t soilTemperature2;
    uint8_t soilTemperature3;
    uint8_t soilTemperature4;
    uint8_t recordType;
    uint8_t extraHumidity1;
    
};

// STRUCT FOR ACTUAL DATA (CONVERTED FROM NUMBERS)
struct ARDATA_c_t
{
    int day;
    int month;
    int year;
    int hour;
    int minutes;
    double outside;
    double hightOut;
    double lowOut;
    double rainfall;
    double highRainFall;
    double barometer;
    double numwisa;
    double inside;
    double insideH;
    double outsideH;
    double avgWindSpeed;
    double highWindSpeed;
    int directionHighWindSpeed;
    int directionDominantWind;
    int UVindex;
    double ET;
    double highSolarRadiation;
    double highUVindex;
    string forecastRule;
    double leafTemp1;
    double leafTemp2;
    int leafWetnesses1;
    int leafWetnesses2;
    double soilTemperature1;
    double soilTemperature2;
    double soilTemperature3;
    double soilTemperature4;
    uint8_t recordType;
    double extraHumidity1;
    double extraHumidity2;
    double extraTemperature1;
    double extraTemperature2;
    double extraTemperature3;
    double soil;
};

#endif /* ArchiveStruct_h */
