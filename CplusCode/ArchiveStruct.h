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
};

// STRUCT FOR ACTUAL DATA (CONVERTED FROM NUMBERS)
struct ARDATA_c_t
{
    string datetime;
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
};

#endif /* ArchiveStruct_h */
