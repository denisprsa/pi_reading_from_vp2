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
        const char * serial_port_path;
        string data_filename, settings_filename;
        struct termios oldsio, newsio;
        int yDelay;
    
        // PRIVATE FUNCTIONS
        char *getDateTime(char *string);
        int getNumberFromChar(char *string, int &row);
        int CheckCRC(int _num, char *_data);
        bool checkACK();
        bool ReadRowFromWeatherStation(vector<ARDATA_c_t> &data, char *buffer, int row);
        ARDATA_c_t ConvertToHumanData(ARDATA_b_t data);
        string PrepareDataOut(ARDATA_c_t data);
        bool PrepareCurrentData(vector<ARDATA_c_t> &vec_data, RTDATA data);
        string toStrMaxDecimals(double value, int decimals);
        string getTimeWeatherStation();
    
    public:
    
        WeatherStation(string serial_port, string setting, string data_filename);
        void OpenSerialPort();
        int WakeUpStation();
        int ReadNextChar(char *pChar);
        int ReadToBuffer( char *pszBuffer, int nBufSize);
        string ReadJSONsettings(string name_of_file);
    
    
        void SaveDataToFile(vector<ARDATA_c_t> data_to_save);
        void menu(int argc, char *argv[]);
        void showMenu();
        void readArchive();
        void readCurrent();
        void readLast15MinuteArhive();
        void setTimeWeatherStation();


};



#endif