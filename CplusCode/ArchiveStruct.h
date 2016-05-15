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

struct RTDATA
{
    // ID
    char _l;
    char _o;
    char _o1;
    uint8_t     bar_trend;
    uint8_t     packet_type;
    uint16_t    not_used;
    uint16_t    barometer;
    uint16_t    inside_temp;
    uint8_t     inside_hum;
    uint16_t    outside_temp;
    uint8_t     wind_speed;
    uint8_t     not_used_1;
    uint16_t    wind_dir;
    uint16_t    min_10_wind_avg;
    uint16_t    min_2_wind_avg;
    uint16_t    min_10_wind_gust;
    uint16_t    wind_direc_10_min_gust;
    uint16_t    not_used_2;
    uint16_t    not_used_3;
    uint16_t    dew_point;
    uint8_t     not_used_4;
    uint8_t     outside_hum;
    uint8_t     not_used_5;
    uint16_t    heat_index;
    uint16_t    wind_chill;
    uint16_t    thsw_index;
    uint16_t    rain_rate;
    uint8_t     uv;
    uint16_t    solar_radiation;
    uint16_t    storm_rain;
    uint16_t    start_date_of_storm;
    uint16_t    daly_rain;
    uint16_t    last_15_min_rain;
    uint16_t    last_hour_rain;
    uint16_t    daly_et;
    uint16_t    last_24_h_rain;
    uint8_t     barometric_reduction;
    uint16_t    barometric_offset;
    uint16_t    barometric_calibration_number;
    uint16_t    barometric_sensor_raw_reading;
    uint16_t    absolute_barometric_pressure;
    uint16_t    altimeter_settings;
    
    uint8_t     not_used_6;
    uint8_t     not_used_7;
    
    uint8_t     next_10_min_wind_speed_grapf_pointer;
    uint8_t     next_15_min_wind_speed_grapf_pointer;
    uint8_t     next_1_h_wind_speed_grapf_pointer;
    uint8_t     next_1_d_wind_speed_grapf_pointer;
    uint8_t     next_1_min_rain_pointer;
    uint8_t     next_1_min_rain_storm_pointer;
    uint8_t     index_to_minute_within_hour;
    uint8_t     next_monthly_rain;
    uint8_t     next_yearly_rain;
    uint8_t     next_sesonal_rain;
    
    uint16_t    not_used_8;
    uint16_t    not_used_9;
    uint16_t    not_used_10;
    uint16_t    not_used_11;
    uint16_t    not_used_12;
    uint16_t    not_used_13;
    
    
    uint8_t     lf;
    uint8_t     cr;
    uint16_t    crc;
    
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
