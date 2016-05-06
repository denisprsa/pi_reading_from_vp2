/*
*
*  AUTHOR : DENIS PRSA
*
*
*/

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <termios.h>
#include <fstream>
#include "VremenskaPostaja.h"


using namespace std;


VremenskaPostaja::VremenskaPostaja(int year, int month, int day){
    this->m_year = year;
    this->m_month = month;
    this->m_day = day;
}

int VremenskaPostaja::getYear(){
    return this->m_year;
}


//
// FUNCTION FOR OPENING PORT
//
void VremenskaPostaja::OpenSerialPort(string path){

    //open port
    const char * c = path.c_str();
    this->fd = open(c, O_RDWR | O_NOCTTY );

    // save previous state
    tcgetattr(this->fd, &this->oldsio);
    // set new state
    bzero(&this->newsio, sizeof(this->newsio));

    this->newsio.c_cflag = CRTSCTS | CS8 | CLOCAL | CREAD;
    this->newsio.c_iflag = IGNBRK | IGNPAR;
    this->newsio.c_oflag = 0;




}