#include <iostream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>
#include <fstream>
#include <algorithm>
#include <vector>
#include <string>

#include "WeatherStation.h"
#include "ArchiveStruct.h"

using namespace std;

int main(int argc, char *argv[])
{

    WeatherStation *vp = new WeatherStation("/home/pi/.wine/dosdevices/com3", "settings.json", "data.csv");
    
    vp->menu(argc, argv);
    
    return 0;
}