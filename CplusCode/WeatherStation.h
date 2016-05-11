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
        bool ReadRowFromWeatherStation(vectord<ARDATA_t> &data, char *buffer);
    
    public:
        WeatherStation(string path);
        void OpenSerialPort();
        int WakeUpStation();
        int ReadNextChar(char *pChar);
        int ReadToBuffer( char *pszBuffer, int nBufSize);
    
    
        void menu(int argc, char *argv[]);
        void showMenu();
        void readArchive();
        void readCurrent();


};



#endif