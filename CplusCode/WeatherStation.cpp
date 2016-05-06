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
#include "WeatherStation.h"


using namespace std;

//
// FOR MORE DETAILS SEE : http://www.davisnet.com/support/weather/download/VantageSerialProtocolDocs_v261.pdf
//


// CONSTRUCTOR
WeatherStation::WeatherStation(string path){
    this->path = path.c_str();
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
                
                // IF NO ERROR THEN GET TIME FROM CONSOLE, OR READ FROM FILE. EXIT IF NO DATETIME PASSED.
                cout << optarg << endl;
                
                break;
        }
    }
}

// --------------------------------------------------------
// FUNCTION THAT SHOWS MENU WITH OPTIONS
// --------------------------------------------------------
void WeatherStation::showMenu(){
    cout << endl;
    cout << "------------------------------------------------------------------------------------" << endl;
    cout << "| Sowtware that reads from Davis Vantage Pro 2 with Raspbery Pi" << endl;
    cout << "| Author : Denis Prsa, http:://vremezizki.si/" << endl;
    cout << "|" << endl;
    cout << "| Parameters: " << endl;
    cout << "|  --a  required parameter : date ( D::d2016m5d5h10m20 ) OR ( F::filename.txt ) " << endl;
    cout << "|       ( read data from archive if new data stored )" << endl;
    cout << "------------------------------------------------------------------------------------" << endl;
    cout << endl;
}



// --------------------------------------------------------
// FUNCTION FOR OPENING PORT
// --------------------------------------------------------
void WeatherStation::OpenSerialPort(){

    //open port
    this->fd = open(this->path, O_RDWR | O_NOCTTY );

    //check if opening failed
    if(this->fd == -1){
        cout << "error while opening port : Unable to open  " <<  path << "."<< endl;
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
            // IF THERE IS RESPONSE, IT'S GONNA GE ANYWAY ‘\n\r’ SO NO NEED TO CHECK THAT
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



