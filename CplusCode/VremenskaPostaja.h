#ifndef VREMENSKAPOSTAJA_H
#define VREMENSKAPOSTAJA_H

#include <string.h>
#include <termios.h>
using namespace std;

class VremenskaPostaja{
    private:

        // varables for openning serial port
        int fd;
        struct termios oldsio, newsio;

        int m_year;
        int m_month;
        int m_day;

    public:
        VremenskaPostaja(int year, int month, int day);

        // function opens serial port
        void OpenSerialPort(string path);

        void SetDate(int year, int month, int day);

        int getYear();


};



#endif