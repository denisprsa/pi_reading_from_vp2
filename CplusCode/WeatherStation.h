#ifndef WEATHERSTATION_H
#define WEATHERSTATION_H

#include <string.h>
#include <termios.h>
using namespace std;

class WeatherStation{
    private:

        // varables for openning serial port
        int fd;
        const char * path;
        struct termios oldsio, newsio;
        int yDelay;
    
    public:
        WeatherStation(string path);
        void OpenSerialPort();
        int WakeUpStation();
        int ReadNextChar(char *pChar);
    
        void menu(int argc, char *argv[]);
        void showMenu();
        void readArchive();
        void readCurrent();


};



#endif