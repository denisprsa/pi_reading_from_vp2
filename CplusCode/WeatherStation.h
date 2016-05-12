//
//  WeatherStation.h
//
//
//  Created by Denis Prša on 05. 05. 16.
//
//

#ifndef WEATHERSTATION_H
#define WEATHERSTATION_H

#include <string.h>
#include <termios.h>
#include "ArchiveStruct.h"
#include <sstream>

#define SSTR( x ) static_cast< std::ostringstream & >( ( std::ostringstream() << std::dec << x ) ).str()

using namespace std;

class WeatherStation{
    private:
        // VARIABLES FOR OPENING PORT
        int fd;
        const char * path;
        struct termios oldsio, newsio;
        int yDelay;
    
        // PRIVATE FUNCTIONS
        char *getDateTime(char *string);
        int getNumberFromChar(char *string, int &row);
        int CheckCRC(int _num, char *_data);
        bool checkACK();
        bool ReadRowFromWeatherStation(vector<ARDATA_c_t> &data, char *buffer, int row);
        ARDATA_c_t ConvertToHumanData(ARDATA_b_t data);
    
    public:
    
        WeatherStation(string path);
        void OpenSerialPort();
        int WakeUpStation();
        int ReadNextChar(char *pChar);
        int ReadToBuffer( char *pszBuffer, int nBufSize);
        string ReadJSONsettings(string name_of_file);
    
        void menu(int argc, char *argv[]);
        void showMenu();
        void readArchive();
        void readCurrent();


};



#endif