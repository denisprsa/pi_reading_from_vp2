/*
*
*  AUTHOR : DENIS PRSA
*
*
*/

#include <iostream>
#include <stdio.h>      // standard input / output functions
#include <stdlib.h>
#include <string.h>     // string function definitions
#include <unistd.h>     // UNIX standard function definitions
#include <fcntl.h>      // File control definitions
#include <errno.h>      // Error number definitions
#include <termios.h>    // POSIX terminal control definitions
#include <getopt.h>     // OPTIONS IN TERMINAL
#include <vector>
#include <ctype.h>
#include <stdint.h>
#include <sstream>
#include <complex>
#include <fstream>
#include <vector>
#include <math.h>
#include <time.h>
#include "ArchiveStruct.h"
#include "WeatherStation.h"
#include "main.h"
#include "json.hpp"

using json = nlohmann::json;
using namespace std;

//
// FOR MORE DETAILS SEE : http://www.davisnet.com/support/weather/download/VantageSerialProtocolDocs_v261.pdf
//

/*--------------------------------------------------------
// CONSTRUCTOR
// PARAMETERS ::
// serial_port          :: PATH TO SERIAL PORT CONNECTION
// settings_filename    :: SETTINGS ARE STORED IN JSON FILE, SPECIFY PATH
// data_filename        :: FILE NAME FOR STORED DATA (CSV)
// --------------------------------------------------------
*/
WeatherStation::WeatherStation(string serial_port, string settings_filename, string data_filename){
    this->serial_port_path = serial_port.c_str();
    this->settings_filename = settings_filename;
    this->data_filename = data_filename;
    this->yDelay = 10;
    
}



// --------------------------------------------------------
// FUNCTION MENU WITH OPTIONS
// --------------------------------------------------------
void WeatherStation::menu(int argc, char *argv[]){
    
    // VARIABLES FOR READING ARGUMENTS FROM CONSOLE
    extern char *optarg;
    extern int optind, opterr, optopt;
    
    int c;
    
    static struct option longopts[] = {
        { "archive",    required_argument,      0,      'a' },
        { NULL,         0,                      NULL,   0 }
    };
    
    
    if(argc == 1){
        this->showMenu();
        exit(2);
    }
    while ((c = getopt_long(argc, argv, "a:", longopts, NULL )) != EOF) {
        switch (c) {
            
            case 'a':
                
                // OPENING SERIAL PORT
                this->OpenSerialPort();
                
                // WAKING UP WEATHER STATION
                if(this->WakeUpStation() != -1){
                    cout << "Error while waking up weather station! Check connection." << endl;
                    exit(2);
                }
                
                // SEND COMMAND TO READ OUT ARHIVE DATA
                if(write(this->fd, "DMPAFT\n", 7) != 7){
                    cout << "Error while writing to serial port " << endl;
                    exit(2);
                }
                if(!checkACK()){
                    exit(2);
                }
                
                
                // IF NO ERROR THEN GET TIME FROM CONSOLE, OR READ FROM FILE. EXIT IF NO DATETIME PASSED.
                const char *dateTime = this->getDateTime(optarg);
                
                
                // WRITE DATE TIME AND LOW AND HIGH BITS
                if(write(this->fd, dateTime, 6) != 6)
                {
                    cout << "Error while writing to serial port " << endl;
                    exit(2);
                }
                tcdrain(this->fd);
                
                // RESPONSE ACK THAT WAS RECIVED DATE AND TIME
                if(!checkACK()){
                    exit(2);
                }
                
                int num_pack;
                char SerBuffer[4200];
                
                num_pack = this->ReadToBuffer(SerBuffer, sizeof(SerBuffer));
                cout << "Recived packets: " << num_pack << endl;
                
                if((num_pack = this->CheckCRC(6, SerBuffer))) {
                    cout << "Error! CRC code : " << num_pack << endl;
                }
                // GET WHAT PAGES FOR THIS DATE AND WHAT ROW IN PAGE CONTAINS THAT DATE
                int pages = SerBuffer[0] | SerBuffer[1] << 8;
                int row = SerBuffer[2] | SerBuffer[3] << 8;
                
                cout << "Pages: " << pages << " Row: "<< row << endl;
                
                bool date_is_smaller = true;
                bool first_time = true;
                
                // ACTUAL DATA (NOT PROCESSED)
                vector<ARDATA_c_t> vec_data;
                
                while (date_is_smaller){
                    // SEND ACK TO RECIVE NEXT PAGE
                    static char ACKS[1];
                    ACKS[0] = 0x06;
                    
                    if(write(this->fd, &ACKS, strlen(ACKS) ) != strlen(ACKS)){
                        cout << "Error while sending ACK to serial port." << endl;
                    }
                    tcdrain(this->fd);
                    
                    // READ PAGES FROM SERIAL PORT
                    
                    num_pack = ReadToBuffer(SerBuffer, sizeof(SerBuffer));
                    cout << "Received packets was: " << num_pack << endl;
                    if( num_pack != 267 ){
                        cout << "Number of packets was incorrect (pages)!" << endl;
                        cout << "Trying once again ..." << endl;
                        
                        // TODO ::::
                        
                    } else {
                        // CHECK FOR CRC
                        if((num_pack = this->CheckCRC(267, SerBuffer))) {
                            cout << "Error! CRC code : " << num_pack << endl;
                            cout << "Trying once again ..." << endl;
                            
                            // TODO ::::
                        }
                        
                        // IF FIRST TIME CHECK WHAT ROW IN PAGE CONTAINS ACCURATE DATEITME
                        if( first_time ){
                            // COPY BUFFER TO STRUCT
                            if(this->ReadRowFromWeatherStation(vec_data, SerBuffer, row))
                                break;
                            
                            first_time = false;
                        } else {
                            if(this->ReadRowFromWeatherStation(vec_data, SerBuffer, 0))
                                break;
                        }
                    }
                    
                }
                
                // SAVE DATA TO FILE
                cout << vec_data.size() << endl;
                this->SaveDataToFile(vec_data);
                
                
                
                break;
        }
    }
}


// --------------------------------------------------------
// FUNCTION THAT CHECKS FOR ACK
// --------------------------------------------------------
bool WeatherStation::ReadRowFromWeatherStation(vector<ARDATA_c_t> &data_converted, char *buffer, int row){
    int from = 1;
    
    for(int i = 0; i < 5; i++){
        if(i >= row){
            ARDATA_b_t data;
            memcpy( &data, buffer + from, sizeof( ARDATA_b_t ));
            // OUT OF DATA
            if(data.date == 0xFFFF)
                return true;
            
            ARDATA_c_t data_c = this->ConvertToHumanData(data);
            
            // IF DATE SMALLER FROM CURRENT READED DATA, THEN RETURN END OF DATA READING
           
            int size_data = data_converted.size();
            if(size_data > 0){
                int date1 = (data_converted[data_converted.size()-1].year - 2000) *  data_converted[data_converted.size()-1].month *  data_converted[data_converted.size()-1].day * (data_converted[data_converted.size()-1].hour+1)*  (data_converted[data_converted.size()-1].minutes+1);
                int date2 = (data_c.year -2000 )* data_c.month * data_c.day * (data_c.hour+1) * (data_c.minutes +1);
                if(date1 > date2)
                    return true;
                
            }
            data_converted.push_back(data_c);
            from += 52;
        }
    }
    return false;
}



// --------------------------------------------------------
//
//
// --------------------------------------------------------
void WeatherStation::SaveDataToFile(vector<ARDATA_c_t> data_to_save){
    ofstream out_file;
    // OPEN FILE AND APPEND DATA
    out_file.open(this->data_filename, std::ofstream::out | std::ofstream::app);
    
    for(int i = 0; i < data_to_save.size(); i++){
        string out_data = this->PrepareDataOut(data_to_save[i]);
        cout << out_data << endl;
        out_file << out_data << "\n";
        
    }
    out_file.close();
    
}



// --------------------------------------------------------
//
//
// --------------------------------------------------------
string WeatherStation::PrepareDataOut(ARDATA_c_t data){
    
    string data_out = "";
    // TIME
    data_out += to_string(data.day) + "." + to_string(data.month) + "." + to_string(data.year) + " ";
    data_out += to_string(data.hour) + ":" + to_string(data.minutes) + ",";
    // TEMPERATURE
    data_out += to_string(floor(data.outside * 100.0) / 10.0) + ",";
    // HUMIDITY OUTSIDE
    data_out += to_string(floor(data.outsideH * 100.0) / 10.0) + ",";
    // DEWPOINT
    double v = data.outsideH*0.01*6.112*exp((17.62*data.outside)/(data.outside+243.12));
    double numerator = 243.12*(log(v)/log(2.718281828459045235) )-440.1;
    double denominator = 19.43-(log(v)/log(2.718281828459045235));
    data_out += to_string(floor(numerator/denominator) / 10.0) + ",";
    // BAROMETER
    data_out += to_string(data.barometer) + ",";
    // WINDSPEED HIGH
    data_out += to_string(data.highWindSpeed) + ",";
    // WINDSPEED AVG
    data_out += to_string(data.avgWindSpeed) + ",";
    // DIRECTION DOMINANT WIND
    data_out += to_string(data.directionDominantWind) + ",";
    // RAIN
    data_out += to_string(data.rainfall) + ",";
    
    
    return data_out;
}



// --------------------------------------------------------
// FUNCTION THAT CONVERTS BITS TO HUMAN READABLE INT,
// DOUBLE, STRINGS
// --------------------------------------------------------
ARDATA_c_t WeatherStation::ConvertToHumanData(ARDATA_b_t data){
    ARDATA_c_t output_data;
    
    // CONVERT DATE
    uint16_t  mask;
    mask = ((1 << 7) - 1) << 9;
    int year = data.date & mask;
    output_data.year =  (year / 512 ) + 2000;
    
    mask = ((1 << 4) - 1) << 5;
    int month = data.date & mask;
    output_data.month = month / 32;
    
    mask = (1 << 5) - 1;
    output_data.day = data.date & mask;
    
    //CONVERT TIME
    output_data.hour = data.time/100;
    output_data.minutes = data.time - ((data.time/100) * 100);
    
    // OUTSIDE TEMP (C)
    output_data.outside = ((data.outside / 10.0) - 32 ) * 5/9;
    // OUTSIDE TEMP HIGH (C)
    output_data.hightOut = ((data.hightOut / 10.0) - 32 ) * 5/9;
    // OUTSIDE TEMP LOW (C)
    output_data.lowOut = ((data.lowOut / 10.0) - 32 ) * 5/9;
    // RAIN ACC (mm)
    output_data.rainfall = data.rainfall * 0.2;
    // HIGHRAINFALL IN THAT TIME (mm)
    output_data.rainfall = data.highRainFall * 0.2;
    // BAROMETER (mbar)
    output_data.barometer = (data.barometer / 1000)* 33.8638815;
    // SOLAR (W)
    output_data.numwisa = data.numwisa;
    // NUMBER OF WIND SAMPLES
    output_data.inside = data.inside;
    // INISDE HUMIDITY
    output_data.insideH = data.insideH;
    // OUTSIDE HUMIDITY
    output_data.outsideH = data.outsideH;
    // AVRAGE WIND SPEED
    output_data.avgWindSpeed = data.avgWindSpeed;
    // HIGH WIND SPEED
    output_data.highWindSpeed = data.highWindSpeed;
    // DIRECTION OF HIGH WIND SPEED
    output_data.directionHighWindSpeed = data.directionHighWindSpeed;
    // DIRECTION OF DOMINANT WIND
    output_data.directionDominantWind = data.directionDominantWind;
    // UV INDEX
    output_data.UVindex = data.UVindex;
    // ET ???
    output_data.ET = data.ET;
    // HIGHEST SOLAR RADIATION
    output_data.highSolarRadiation = data.highSolarRadiation;
    // HIGH UV RADIATION
    output_data.highUVindex = data.highUVindex;
    // FORECAST RULE, THERE ARE 193 RULES IN XXX.H
    // TODO
    output_data.forecastRule = data.forecastRule;
    // LEAF TEMPERATURE 1
    // .
    // .
    // .
    // TODO
    
    // RECORD TYPE
    output_data.recordType = data.recordType;
    
    
    
    
    
    
    string datetime_k = SSTR(output_data.day) + "." + SSTR(output_data.month) + "." +
                        SSTR(output_data.year) + " " + SSTR(output_data.hour) + ":" +
                        SSTR(output_data.minutes) + " T: " + SSTR(output_data.outside);
    
    // RETURN
    cout << datetime_k << endl;
    return output_data;
}



// --------------------------------------------------------
// FUNCTION THAT CHECKS FOR ACK
// --------------------------------------------------------
bool WeatherStation::checkACK(){
    char ch = 0x00;
    this->ReadNextChar(&ch);
    cout << ch << endl;
    if(ch != 0x06){
        cout << "Error ! No ACK recevied." << endl;
        return false;
    }
    return true;
}


// --------------------------------------------------------
// FUNCTION THAT GETS TIME FROM FILE OR CONSOLE
// --------------------------------------------------------
char *WeatherStation::getDateTime(char *_string){
    
    // IF NO ERROR THEN GET TIME FROM CONSOLE, OR READ FROM FILE. EXIT IF NO DATETIME PASSED.
    int counter = 3;
    int year;
    int month;
    int day;
    int hour;
    int minute;
    char *datah = new char[6];
    int16_t i;
    
    if(_string[0] == 'D' && _string[1] == ':' && _string[2] == ':'){
        
        //  CONVERT CONSOLE PARAMETERS
        while (_string[counter] != '\0') {
            if(_string[counter] == 'y'){
                year = this->getNumberFromChar(_string, counter);
            } else if (optarg[counter] == 'm'){
                month = this->getNumberFromChar(_string, counter);
            } else if (optarg[counter] == 'd'){
                day = this->getNumberFromChar(_string, counter);
            } else if (optarg[counter] == 'h'){
                hour = this->getNumberFromChar(_string, counter);
            } else if (optarg[counter] == 'i'){
                minute = this->getNumberFromChar(_string, counter);
            }
            counter ++;
        }
        
        int vantageDateStamp = day + month*32 + (year-2000)*512;
        int vantageTimeStamp = (100*hour + minute);
        
        char b1 = (vantageDateStamp >> 8) & 0xFF;
        char b2 = vantageDateStamp & 0xFF;
        
        char c1 = (vantageTimeStamp >> 8) & 0xFF;
        char c2 = vantageTimeStamp & 0xFF;
        
        
        datah[0] = b2;
        datah[1] = b1;
        datah[2] = c2;
        datah[3] = c1;
        i = this->CheckCRC(4, datah);
        datah[4] = HIBYTE(i);
        datah[5] = LOBYTE(i);
        return datah;

    } else if (_string[0] == 'F' && _string[1] == ':' && _string[2] == ':'){
        
    } else {
        cout << "No datetime or file passed. Exiting!" << endl;
        exit(2);
    }
    
    return new char[1];
}




// --------------------------------------------------------
// FUNCTION THAT CONVERTS NUMBER INT FROM CHAR
// --------------------------------------------------------
int WeatherStation::getNumberFromChar(char *_string, int &row){
    
    if( !isdigit(_string[row+1]) ){
        cout << "Invalid datetime!" << endl;
        exit(2);
    }
    string _number = "";
    do {
        _number.push_back(_string[row+1]);
        row ++;
    } while(isdigit(_string[row+1]));
    
    int result;
    stringstream(_number) >> result;
    return result;
}

// --------------------------------------------------------
// FUNCTION THAT SHOWS MENU WITH OPTIONS
// --------------------------------------------------------
void WeatherStation::showMenu(){
    cout << endl;
    cout << "------------------------------------------------------------------------------------" << endl;
    cout << "| Sowtware that reads data from Davis Vantage Pro 2 with Raspbery Pi" << endl;
    cout << "| Author : Denis Prsa, http:://vremezizki.si/" << endl;
    cout << "|" << endl;
    cout << "| Parameters: " << endl;
    cout << "|  --a  required parameter : date ( D::y2016m5d5h10i20) OR ( F::filename.txt ) " << endl;
    cout << "|       ( read data from archive if new data stored )" << endl;
    cout << "|       ( y = year, m = month, d = day, h = hour, i = minute )" << endl;
    cout << "------------------------------------------------------------------------------------" << endl;
    cout << endl;
}

// --------------------------------------------------------
// FUNCTION FOR OPENING PORT
// --------------------------------------------------------
void WeatherStation::OpenSerialPort(){

    //open port
    this->fd = open(this->serial_port_path, O_RDWR | O_NOCTTY );

    //check if opening failed
    if(this->fd == -1){
        cout << "error while opening port : Unable to open  " <<  this->serial_port_path << "."<< endl;
        return;
    }

    // save previous state
    tcgetattr(this->fd, &this->oldsio);
    // set new state
    bzero(&this->newsio, sizeof(this->newsio));

    // set setings for opening serial port
    this->newsio.c_cflag = CRTSCTS | CS8 | CLOCAL | CREAD;
    this->newsio.c_iflag = IGNBRK | IGNPAR;
    this->newsio.c_oflag = 0;
    this->newsio.c_lflag = 0;
    this->newsio.c_cc[VTIME]    = this->yDelay; /* timeout in 1/10 sec intervals */
    this->newsio.c_cc[VMIN]     = 0;      /* block until char or timeout */
    cfsetospeed (&this->newsio, B19200);
    cfsetispeed (&this->newsio, B19200);

    if(tcsetattr(this->fd, TCSAFLUSH, &this->newsio)) {
        cout << "error while opening port : Problem configuring serial device, check device name." << endl;
        exit(2);
    }

    if(tcflush(this->fd, TCIOFLUSH)) {
        cout << "error while opening port : Problem flushing serial device, check device name." << endl;
        exit(2);
    }

}


// --------------------------------------------------------
// FUNCTION FOR WAKING UP STATION
// --------------------------------------------------------
int WeatherStation::WakeUpStation(){
    char ch;
    int i;

    // FROM DOC : If the console has not woken up after 3 attempts, then signal a connection error
    for(i = 0; i < 3; i++)
    {
        // FROM DOC : Send a Line Feed character, ‘\n’ (decimal 10, hex 0x0A).
        ch = '\n';
        if(write(this->fd, &ch, 1) != 1)
        {
            cout <<  "reading from weather station: Problem reading serial device (one char).";
            exit(2);
        }
        
        // CALL FUNCTION TO READ CHAR FROM SERIAL PORT
        // FROM DOC : Listen for a returned response of Line Feed and Carriage Return characters, (‘\n\r’).
        if(this->ReadNextChar(&ch))
        {
            // IF THERE IS RESPONSE, IT'S GONNA GE  ‘\n\r’ SO NO NEED TO CLEAR '\r'
            this->ReadNextChar(&ch);
            cout << "Weather station woke up after " << i+1 << " retries." << endl;
            return -1;
        }
    }

    return 0;
}

// --------------------------------------------------------
// FUNCTION FOR READING NEXT CHAR FROM WEATHER STATION
// --------------------------------------------------------
int WeatherStation::ReadNextChar( char *pChar ){
    int result;

    result = read(this->fd, pChar, 1);
    if(result == -1) {
        cout <<  "reading from weather station: Problem reading serial device (one char).";
        exit(2);
    }
    // RETURN 1 CHARACTER SO == TRUE
    return result;
}


// --------------------------------------------------------
// FUNCTION THAT CHECKS FOR CRC CODE
// --------------------------------------------------------
int WeatherStation::CheckCRC( int _num, char *_data ){
    int i;
    uint16_t wCRC = 0;
    uint8_t y;
    static uint16_t crc_table[256] = {
        0x0,     0x1021,  0x2042,  0x3063,  0x4084,  0x50a5,  0x60c6,  0x70e7,
        0x8108,  0x9129,  0xa14a,  0xb16b,  0xc18c,  0xd1ad,  0xe1ce,  0xf1ef,
        0x1231,  0x210,   0x3273,  0x2252,  0x52b5,  0x4294,  0x72f7,  0x62d6,
        0x9339,  0x8318,  0xb37b,  0xa35a,  0xd3bd,  0xc39c,  0xf3ff,  0xe3de,
        0x2462,  0x3443,  0x420,   0x1401,  0x64e6,  0x74c7,  0x44a4,  0x5485,
        0xa56a,  0xb54b,  0x8528,  0x9509,  0xe5ee,  0xf5cf,  0xc5ac,  0xd58d,
        0x3653,  0x2672,  0x1611,  0x630,   0x76d7,  0x66f6,  0x5695,  0x46b4,
        0xb75b,  0xa77a,  0x9719,  0x8738,  0xf7df,  0xe7fe,  0xd79d,  0xc7bc,
        0x48c4,  0x58e5,  0x6886,  0x78a7,  0x840,   0x1861,  0x2802,  0x3823,
        0xc9cc,  0xd9ed,  0xe98e,  0xf9af,  0x8948,  0x9969,  0xa90a,  0xb92b,
        0x5af5,  0x4ad4,  0x7ab7,  0x6a96,  0x1a71,  0xa50,   0x3a33,  0x2a12,
        0xdbfd,  0xcbdc,  0xfbbf,  0xeb9e,  0x9b79,  0x8b58,  0xbb3b,  0xab1a,
        0x6ca6,  0x7c87,  0x4ce4,  0x5cc5,  0x2c22,  0x3c03,  0xc60,   0x1c41,
        0xedae,  0xfd8f,  0xcdec,  0xddcd,  0xad2a,  0xbd0b,  0x8d68,  0x9d49,
        0x7e97,  0x6eb6,  0x5ed5,  0x4ef4,  0x3e13,  0x2e32,  0x1e51,  0xe70,
        0xff9f,  0xefbe,  0xdfdd,  0xcffc,  0xbf1b,  0xaf3a,  0x9f59,  0x8f78,
        0x9188,  0x81a9,  0xb1ca,  0xa1eb,  0xd10c,  0xc12d,  0xf14e,  0xe16f,
        0x1080,  0xa1,    0x30c2,  0x20e3,  0x5004,  0x4025,  0x7046,  0x6067,
        0x83b9,  0x9398,  0xa3fb,  0xb3da,  0xc33d,  0xd31c,  0xe37f,  0xf35e,
        0x2b1,   0x1290,  0x22f3,  0x32d2,  0x4235,  0x5214,  0x6277,  0x7256,
        0xb5ea,  0xa5cb,  0x95a8,  0x8589,  0xf56e,  0xe54f,  0xd52c,  0xc50d,
        0x34e2,  0x24c3,  0x14a0,  0x481,   0x7466,  0x6447,  0x5424,  0x4405,
        0xa7db,  0xb7fa,  0x8799,  0x97b8,  0xe75f,  0xf77e,  0xc71d,  0xd73c,
        0x26d3,  0x36f2,  0x691,   0x16b0,  0x6657,  0x7676,  0x4615,  0x5634,
        0xd94c,  0xc96d,  0xf90e,  0xe92f,  0x99c8,  0x89e9,  0xb98a,  0xa9ab,
        0x5844,  0x4865,  0x7806,  0x6827,  0x18c0,  0x8e1,   0x3882,  0x28a3,
        0xcb7d,  0xdb5c,  0xeb3f,  0xfb1e,  0x8bf9,  0x9bd8,  0xabbb,  0xbb9a,
        0x4a75,  0x5a54,  0x6a37,  0x7a16,  0xaf1,   0x1ad0,  0x2ab3,  0x3a92,
        0xfd2e,  0xed0f,  0xdd6c,  0xcd4d,  0xbdaa,  0xad8b,  0x9de8,  0x8dc9,
        0x7c26,  0x6c07,  0x5c64,  0x4c45,  0x3ca2,  0x2c83,  0x1ce0,  0xcc1,
        0xef1f,  0xff3e,  0xcf5d,  0xdf7c,  0xaf9b,  0xbfba,  0x8fd9,  0x9ff8,
        0x6e17,  0x7e36,  0x4e55,  0x5e74,  0x2e93,  0x3eb2,  0xed1,   0x1ef0,
    };

    
    for( i = 0; i < _num; i++) {
        y = *(_data)++;
        wCRC = crc_table[(wCRC >> 8) ^ y] ^ (wCRC << 8);
    }
    
    return wCRC;
}



// --------------------------------------------------------
// FUNCTION THAT CHECKS FOR CRC CODE
// --------------------------------------------------------
int WeatherStation::ReadToBuffer( char *pszBuffer, int nBufSize)
{
    int nPos = 0;               /* current character position */
    char *pBuf = pszBuffer;
    
    while(nPos < nBufSize) {
        if(!ReadNextChar(pBuf++))
            return nPos;        /* no character available */
        ++nPos;
    }
    return -1;                  /* buffer overflow */
}



// --------------------------------------------------------
// FUNCTION THAT CHECKS FOR CRC CODE
// --------------------------------------------------------
string WeatherStation::ReadJSONsettings(string name_of_file){
    // OPEN FILE
    ifstream file(name_of_file);
    string line, all_lines = "";
    while (getline(file, line))
    {
        all_lines.append( line );
    }
    return all_lines;
}



