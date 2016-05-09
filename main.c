/****************************************************************************/
/*  vproweather 1.0                                                         */
/*  A program for acquiring data from a Davis Vantage Pro Weather Station.  */
/*                                                                          */
/*  Thanks to Paul Davis for his 'remserial' program, and Aaron Logue's     */
/*  'weastat' program.                                                      */
/*                                                                          */
/* (c)2013 Joe Jaworski    email: jj@joejaworski.com                        */
/* VPROWEATHER is covered under the GNU general license. See the attached   */
/* LICENSE file for details.                                                */
/*                                                                          */
/* This software is provided as-is, without any expressed or implied        */
/* warranties, including, without limitation, the implied warranties of     */
/* merchantibility and fitness for any particular purpose.                  */
/*                                                                          */
/* main.c - main entry and intitialization                                  */
/* tabs every 4 places                                                      */
/*                                                                          */
/*                                                                          */
/****************************************************************************/
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <stdint.h>
#include <stdbool.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include <getopt.h>
#include <time.h>
#include "main.h"
#include "names.h"
#include "dhandler.h"


/* local Data */
static char szttyDevice[255];           /* Serial device string */
static bool bVerbose;                   /* Verbose Mode */
static bool bBKLOn;                     /* Backlite ON */
static bool bBKLOff;                    /* Backlite OFF */
static bool bVer;                       /* Davis firmware version astring */
static bool bModel;                     /* model name/number */
static bool bGetRTD;                    /* Get Real Time Data */
static bool bGetHLD;                    /* Get High Low Data */
static bool bGetGD;                     /* Get graph data */
static bool bGetARH;
static bool bGetTime;                   /* Get time flag */
static bool bSetTime;                   /* Set time flag */
static bool bHTML;                      /* html substitution flag */
static int8_t yDelay;                   /* Read wait time */

static char szSerBuffer[4200];          /* serial read/write buffer */
static char szSWriteErr[] = "Serial Port Write Error";



/* local functions */
static int GetParms(int argc, char *argv[]);
static int WakeUp(int nfd);
static int ReadNextChar(int fdser, char *pChar);
static void Delay(int secs, long microsecs);
static int ReadToBuffer(int fdser, char *pszBuffer, int nBufSize);



/*--------------------------------------------------------------------------
    main
----------------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
    int fdser;                      /* serial device fd */
    struct termios oldtio, newtio;  /* serial device configuration */
    int16_t nCnt;
    struct tm stm;
    time_t tt;
    int16_t i;
    char ch;


    /* Get command line parms */
    if (!GetParms(argc, argv)) {
        printf("vproweather v0.6 18-OCT-2004 written by Joe Jaworski ");
        printf("\nSee http://www.joejaworski.com/weather/ for latest version and info");
        printf("\nUsage: vproweather [Options] Device\n");
        printf("Options:\n");
        printf(" -x, --get-realtime    Get real time weather data.\n");
        printf(" -l, --get-highlow     Get Highs/Lows data.\n");
        printf(" -g, --get-graph       Get graph data.\n");
        printf(" -t, --get-time        Get weather station time.\n");
        printf(" -h, --get-arhive      Get data from last time in arhive.\n");
	    printf(" -s, --set-time        Set weather station time to system time.\n");
        printf(" -o, --bklite-on       Turn backlite on.\n");
        printf(" -f, --bklite-off      Turn backlite off.\n");
        printf(" -r, --version         Query for Davis firmware version string.\n");
        printf(" -m, --model           Query for weather station model name.\n");
        printf(" -d, --delay=num       Cmd Delay in 1/10ths seconds. Default is 10 (1 sec).\n");
        printf(" -v, --verbose         Verbose mode.\n");
        printf(" Device                Serial Device. Required parameter.\n");
        printf("\n");
        printf("Examples:\n");
        printf("vproweather --get-realtime /dev/ttyp0 > rtwdata.txt\n");
        printf("Gets real time data set to file rtwdata.txt from serial device ttyp0\n");
        printf("vproweather --verbose --bklite-on /dev/ttyp0\n");
        printf("Turns the LCD backlite On, illuminating the display.\n");
        exit(2);
    }

    if(bVerbose)
        printf("Opening Serial Port %s...\n", szttyDevice);
    fdser = open(szttyDevice, O_RDWR | O_NOCTTY );
    if (fdser < 0) {
        perror("vproweather: Problem opening serial device, check device name.");
        exit(2);
    }

    tcgetattr(fdser, &oldtio);  /* save previous port settings */

    bzero(&newtio, sizeof(newtio));
    newtio.c_cflag = CRTSCTS | CS8 | CLOCAL | CREAD;
    newtio.c_iflag = IGNBRK | IGNPAR;
    newtio.c_oflag = 0;
    /* set serial device input mode (non-canonical, no echo,...) */
    newtio.c_lflag = 0;
    newtio.c_cc[VTIME]    = yDelay; /* timeout in 1/10 sec intervals */
    newtio.c_cc[VMIN]     = 0;      /* block until char or timeout */
    cfsetospeed (&newtio, B19200);
    cfsetispeed (&newtio, B19200);

    if(tcsetattr(fdser, TCSAFLUSH, &newtio)) {
        perror("vproweather: Problem configuring serial device, check device name.");
        exit(2);
    }

    if(tcflush(fdser, TCIOFLUSH)) {
        perror("vproweather: Problem flushing serial device, check device name.");
        exit(2);
    }


    /* wake up the station */
    if(bVerbose)
        printf("Waking up weather station...\n");
    if(!WakeUp(fdser)) {
        fprintf(stderr, "vproweather: Can't wake up weather station- no response.\n");
        exit(2);
    }


    /* ---------------------------- */
    /* Process Command line actions */
    /* ---------------------------- */

    /* turn on backlite */
    if(bBKLOn) {
        if(bVerbose)
            printf("Turning Backlite ON...\n");
        while(ReadNextChar(fdser, &ch));    /* clear channel and delay */
        while(ReadNextChar(fdser, &ch));    /* clear channel and delay */
        strcpy(szSerBuffer, "LAMPS 1\n");   /* make Davis cmd string */
        if(write(fdser, &szSerBuffer, strlen(szSerBuffer)) != strlen(szSerBuffer))
        {   perror(szSWriteErr);
            exit(2);
        }
        tcdrain(fdser);
    }

    /* turn off backlite */
    if(bBKLOff) {
        if(bVerbose)
            printf("Turning Backlite OFF...\n");
        while(ReadNextChar(fdser, &ch));        /* clear channel and delay */
        strcpy(szSerBuffer, "LAMPS 0\n");   /* make Davis cmd string */
        if(write(fdser, &szSerBuffer, strlen(szSerBuffer)) != strlen(szSerBuffer))
        {   perror(szSWriteErr);
            exit(2);
        }
        tcdrain(fdser);
    }

    /* Get firmware version strings */
    if(bVer) {
        if(bVerbose)
            printf("Getting firmware string...\n");
        while(ReadNextChar(fdser, &ch));        /* clear channel and delay */
        strcpy(szSerBuffer, "VER\n");       /* make Davis cmd string */
        if(write(fdser, &szSerBuffer, strlen(szSerBuffer)) != strlen(szSerBuffer))
        {   perror(szSWriteErr);
            exit(2);
        }
        tcdrain(fdser);
        Delay(1, 0L);
        nCnt = ReadToBuffer(fdser, szSerBuffer, sizeof(szSerBuffer));
        if(bVerbose)
            printf("Got %d characters\n", nCnt);
        *(szSerBuffer+nCnt) = '\0';
        printf("Version Info: %s\n", szSerBuffer);
    }
    if(bModel) {
        if(bVerbose)
            printf("Getting model number code byte...\n");
        while(ReadNextChar(fdser, &ch));        /* clear channel and delay */
        strcpy(szSerBuffer, "WRD\022\115\n");   /* make Davis cmd string */
        if(write(fdser, &szSerBuffer, strlen(szSerBuffer)) != strlen(szSerBuffer))
        {
            perror(szSWriteErr);
            exit(2);
        }

        tcdrain(fdser);

        if(!ReadNextChar(fdser, &ch)) {
            fprintf(stderr, "vproweather: No response to model number query.\n");
            exit(2);
        }
        if(!ReadNextChar(fdser, &ch)) {
            fprintf(stderr, "vproweather: Model number byte not sent.\n");
            exit(2);
        }
        printf("Model: Davis ");
        switch((int)ch) {
            case 0: printf("Wizard III\n");                 break;
            case 1: printf("Wizard II\n");                  break;
            case 2: printf("Monitor\n");                    break;
            case 3: printf("Perception\n");                 break;
            case 4: printf("GroWeather\n");                 break;
            case 5: printf("Energy Environmonitor\n");      break;
            case 6: printf("Health Environmonitor\n");      break;
            case 16: printf("Vantage Pro\n");               break;
            default: printf("Unknown model (%d)\n", (int)ch);   break;
        }
        if(ch != 16)
            printf("Warning: This program may not work correctly with your model.\n");
    }


    /* Get weather station time */
    if(bGetTime) {
        if(bVerbose)
            printf("Getting weather station time...\n");
        while(ReadNextChar(fdser, &ch));    /* clear channel and delay */
        strcpy(szSerBuffer, "GETTIME\n");   /* make Davis cmd string */
        if(write(fdser, &szSerBuffer, strlen(szSerBuffer)) != strlen(szSerBuffer))
        {
            perror(szSWriteErr);
            exit(2);
        }
        nCnt = ReadToBuffer(fdser, szSerBuffer, sizeof(szSerBuffer));
        if(bVerbose) {
            printf("Got %d characters...", nCnt);
            if(nCnt != 9)
                printf("Bad\n");
            else
                printf("Good\n");
        }
        if(nCnt != 9) {
            fprintf(stderr, "vproweather: Didn't get all data. Try changing delay parameter.\n");
            exit(2);
        }
        if((nCnt = CheckCRC(8, szSerBuffer+1))) {   /* check crc */
            fprintf(stderr,"vproweather: CRC failure %d.\n", nCnt);
            exit(2);
        }
        else if (bVerbose)
            printf("CRC verified good on TIME packet.\n");

        PrintTime(szSerBuffer);                 /* ...and to stdout */
    }


    /* Set weather station time */
    if(bSetTime) {
        if(bVerbose)
            printf("Setting weather station time...\n");
        while(ReadNextChar(fdser, &ch));    /* clear channel and delay */
        strcpy(szSerBuffer, "SETTIME\n");   /* make Davis cmd string */
        if(write(fdser, &szSerBuffer, strlen(szSerBuffer)) != strlen(szSerBuffer))
        {
            perror(szSWriteErr);
            exit(2);
        }
        ReadNextChar(fdser, &ch);           /* get ACK */
        if(ch != 0x06) {
            fprintf(stderr, "vproweather: Failed to get ACK.\n");
            exit(2);
        }
        else if(bVerbose)
            printf("Received ACK, sending system time...\n");


        time(&tt);
        stm = *localtime(&tt);              /* get system time */
        stm.tm_mon  += 1;

        szSerBuffer[0] = (char)stm.tm_sec;
        szSerBuffer[1] = (char)stm.tm_min;
        szSerBuffer[2] = (char)stm.tm_hour;
        szSerBuffer[3] = (char)stm.tm_mday;
        szSerBuffer[4] = (char)stm.tm_mon;
        szSerBuffer[5] = (char)stm.tm_year;
        i = CheckCRC(6, szSerBuffer);       /* generate CRC */
        szSerBuffer[6] = HIBYTE(i);         /* send MSB first */
        szSerBuffer[7] = LOBYTE(i);

        if(write(fdser, &szSerBuffer, strlen(szSerBuffer)) != strlen(szSerBuffer))
        {
            perror(szSWriteErr);
            exit(2);
        }
        tcdrain(fdser);
        Delay(1, 0L);

        ReadNextChar(fdser, &ch);           /* get ACK from console */
        if(ch != 0x06) {
            fprintf(stderr, "vproweather: CRC Failed, didn't get ACK (%d)\n", ch);
            exit(2);
        }
        else if(bVerbose)
            printf("Received ACK, CRC good, time is now set.\n");
    }



    /* Get highs/lows data set */
    if(bGetHLD) {
        if(bVerbose)
            printf("Getting highs/lows data set...\n");

        while(ReadNextChar(fdser, &ch));        /* clear channel and delay */
        strcpy(szSerBuffer, "HILOWS\n");        /* make Davis cmd string */
        if(write(fdser, &szSerBuffer, strlen(szSerBuffer)) != strlen(szSerBuffer))
        {
            perror(szSWriteErr);
            exit(2);
        }
        tcdrain(fdser);
        nCnt = ReadToBuffer(fdser, szSerBuffer, sizeof(szSerBuffer));
        if(bVerbose) {
            printf("Got %d characters...", nCnt);
            if(nCnt != 439)
                printf("Bad\n");
            else
                printf("Good\n");
        }
        if(nCnt != 439) {
            fprintf(stderr,"vproweather: Didn't get all data. Try changing delay parameter.\n");
            exit(2);
        }
        if((nCnt = CheckCRC(438, szSerBuffer+1))) { /* check crc */
            fprintf(stderr,"vproweather: CRC failure %d.\n", nCnt);
            exit(2);
        }
        else if (bVerbose)
            printf("CRC verified good on HILOWS packet.\n");

        GetHLData(szSerBuffer);             /* get data to struct */
        PrintHLData();                      /* ...and to stdout */
    }



    /* Get Graph data sets */
    if(bGetGD) {
        if(bVerbose)
            printf("Getting graph data set...\n");

        while(ReadNextChar(fdser, &ch));        /* clear channel and delay */
        strcpy(szSerBuffer, "GETEE\n");         /* make Davis cmd string */
        if(write(fdser, &szSerBuffer, strlen(szSerBuffer)) != strlen(szSerBuffer))
        {
            perror(szSWriteErr);
            exit(2);
        }
        tcdrain(fdser);
        nCnt = ReadToBuffer(fdser, szSerBuffer, sizeof(szSerBuffer));
        if(bVerbose) {
            printf("Got %d characters...", nCnt);
            if(nCnt != 4099)
                printf("Bad\n");
            else
                printf("Good\n");
        }
        if(nCnt != 4099) {
            fprintf(stderr, "vproweather: Didn't get all data. Try changing delay parameter.\n");
            exit(2);
        }
        if((nCnt = CheckCRC(4098, szSerBuffer+1))) {    /* check crc */
            fprintf(stderr, "vproweather: CRC failure %d.\n", nCnt);
            exit(2);
        }
        else if (bVerbose)
            printf("CRC verified good on full EEPROM packet.\n");
    }





    /* Get real time data set (Davis LOOP data) */
    if(bGetRTD) {
        if(bVerbose)
            printf("Getting real time data set...\n");

        while(ReadNextChar(fdser, &ch));        /* clear channel and delay */
        strcpy(szSerBuffer, "LOOP 1\n");        /* make Davis cmd string */
        if(write(fdser, &szSerBuffer, strlen(szSerBuffer)) != strlen(szSerBuffer))
        {
            perror(szSWriteErr);
            exit(2);
        }
        tcdrain(fdser);
        nCnt = ReadToBuffer(fdser, szSerBuffer, sizeof(szSerBuffer));
        if(bVerbose) {
            printf("Got %d characters...", nCnt);
            if(nCnt != 100)
                printf("Bad\n");
            else
                printf("Good\n");
        }
        if(nCnt != 100) {
            fprintf(stderr, "vproweather: Didn't get all data. Try changing delay parameter.\n");
            exit(2);
        }
        if((nCnt = CheckCRC(99, szSerBuffer+1))) {  /* check crc */
            fprintf(stderr, "vproweather: CRC failure %d.\n", nCnt);
            exit(2);
        }
        else if (bVerbose)
            printf("CRC verified good on LOOP packet.\n");

        GetRTData(szSerBuffer);             /* get data to struct */
        PrintRTData();                      /* ...and to stdout */
    }


    if(bGetARH){
	    if(bVerbose)
		    printf("Archive\n");
	    while(ReadNextChar(fdser, &ch));
	    strcpy(szSerBuffer, "DMPAFT\n");
	    if(write(fdser, &szSerBuffer, strlen(szSerBuffer)) != strlen(szSerBuffer)){
		perror(szSWriteErr);
		exit(2);
	    }

	    ReadNextChar(fdser, &ch);

	    if(ch != 0x06) {
		fprintf(stderr, "vproweather: Failed to get ACK.\n");
		exit(2);
	    }
	    else if(bVerbose)
		    printf("Received ACK, sending time for archive ...\n");

	    int day = 9;
	    int month = 5;
	    int year = 2016;
	    int hour = 12;
	    int minute = 0;

	    int vantageDateStamp = day + month*32 + (year-2000)*512;
	    int vantageTimeStamp = (100*hour + minute);
        fprintf(stderr,"vantageDateStamp  %d . \n", (int)vantageDateStamp);

	    char b1 = (vantageDateStamp >> 8) & 0xFF;
	    char b2 = vantageDateStamp & 0xFF;

	    char c1 = (vantageTimeStamp >> 8) & 0xFF;
	    char c2 = vantageTimeStamp & 0xFF;
	    
	    fprintf(stderr,"b2  %d . \n", (int)b2);
	    fprintf(stderr,"b1  %d . \n", (int)b1);
	    
	    fprintf(stderr,"c2  %d . \n", (int)c2);
	    fprintf(stderr,"c1  %d . \n", (int)c1);

	    static char datah[6];
	    datah[0] = b2;
	    datah[1] = b1;
	    datah[2] = c2;
	    datah[3] = c1;
	    i = CheckCRC(4, datah);
        fprintf(stderr,"hi  %d . \n", HIBYTE(i));
        fprintf(stderr,"lo  %d . \n", LOBYTE(i));
	    datah[4] = HIBYTE(i);
	    datah[5] = LOBYTE(i);

	    if(write(fdser, &datah, strlen(datah)) != strlen(datah))
	    {
		    perror(szSWriteErr);
		    exit(2);
	    }
	    tcdrain(fdser);

	   
	    //Delay(1, 0L);
	    //nCnt = ReadToBuffer(fdser, szSerBuffer, sizeof(szSerBuffer));
	    //fprintf(stderr, "Got %d characters...\n", nCnt);

	    

	    
	    ReadNextChar(fdser, &ch);	   
	    fprintf(stderr, "vproweather ACK za preverjanje (%d)\n", ch); 
	    if(ch != 0x06) {
		    fprintf(stderr, "vproweather: Failed to get ACK.\n");
		    exit(2);
	    }
	    else if(bVerbose)
		    printf("Received ACK, CRC OK ...\n");

	    Delay(2, 0L);

	    nCnt = ReadToBuffer(fdser, szSerBuffer, sizeof(szSerBuffer));
	    fprintf(stderr, " prejetih bajtov  %d  \n", nCnt);

	    if((nCnt = CheckCRC(6, szSerBuffer))) {
		    fprintf(stderr, "crc koda za prejem strani:  %d  \n", nCnt);
	    }


	    
	    char pa2 = szSerBuffer[0];
        
        fprintf(stderr, "pa2 %d \n", pa2);
	    char pa1 = szSerBuffer[1];
        
        fprintf(stderr, "pa1 %d \n", pa1);
	    int number = pa2 | pa1 << 8;
	    fprintf(stderr, "page %d \n", number);

	    char za2 = szSerBuffer[2];
        fprintf(stderr, "za2 %d \n", pa2);
	    char za1 = szSerBuffer[3];
        fprintf(stderr, "za1 %d \n", pa1);
	    int number2 = za2 | za1 << 8;
	    fprintf(stderr, "row %d \n", number2);

	    for(int i=0; i<4200; i++){
		    szSerBuffer[i] = 0x00;
	    }

	    //tcdrain(fdser);
	    
	    static char ACKS[1];
	    ACKS[0] = 0x06;
	    fprintf(stderr, " %d  \n", ACKS[0] ); 
	    
	    //tcdrain(fdser);
	    //while(ReadNextChar(fdser, &ch)){fprintf(stderr, " 4  " ); }

	    while(true){

            if(write(fdser, &ACKS, strlen(ACKS) ) != strlen(ACKS)){
                perror(szSWriteErr);
                exit(2);
            }
            tcdrain(fdser);

            nCnt = ReadToBuffer(fdser, szSerBuffer, sizeof(szSerBuffer));
            fprintf(stderr, "vproweather: mora biti nic (%d)\n", nCnt);
            if(nCnt != 267 ){
                fprintf(stderr, "Napaka \n");
                break;
            }
            if((nCnt = CheckCRC(267, szSerBuffer))) {
                fprintf(stderr, "crc koda za prejem strani1:  %d  \n", nCnt);
            }
            if((nCnt = CheckCRC(266, szSerBuffer+1))) {
                fprintf(stderr, "crc koda za prejem strani2:  %d  \n", nCnt);
            }
            GetARData(szSerBuffer);


            break;




        }

    }


    /* all done, exit */
    tcsetattr(fdser, TCSANOW, &oldtio); /* restore previous port settings */
    if(close(fdser)) {
        perror("vproweather: Problem closing serial device, check device name.");
        exit(2);
    }

    exit(0);
}





/*--------------------------------------------------------------------------
    GetParms
    Reads command line parameters.
----------------------------------------------------------------------------*/
int GetParms(int argc, char *argv[])
{
    extern char *optarg;
    extern int optind, opterr, optopt;
    int c, i;

     /* options descriptor */
    static struct option longopts[] = {
        { "bklite-on",      no_argument,    0,  'o' },
        { "bklite-off",     no_argument,    0,  'f' },
        { "get-arhive",     no_argument,    0,  'c' },
       	{ "version",        no_argument,    0,  'r' },
        { "model",          no_argument,    0,  'm' },
        { "get-realtime",   no_argument,    0,  'x' },
        { "get-hilo",       no_argument,    0,  'l' },
        { "get-graph",      no_argument,    0,  'g' },
        { "get-time",       no_argument,    0,  't' },
        { "set-time",       no_argument,    0,  's' },
        { "verbose",        no_argument,    0,  'v' },
        { "delay",          required_argument,  0,  'd' },
        { NULL,             0,              NULL,   0 }
    };



    /* Set command line defaults */
    *szttyDevice = '\0';
    bBKLOn = false;
    bBKLOff = false;
    bVer = false;
    bModel = false;
    bGetRTD = false;
    bGetHLD = false;
    bGetGD = false;
    bHTML = false;
    bVerbose = false;
    bGetARH = false;
    bGetTime = false;
    bSetTime = false;
    yDelay = 10;

    if(argc == 1)
        return 0;               /* no parms at all */

    while ((c = getopt_long(argc, argv, "cofrmxlgvtsb:d:", longopts, NULL )) != EOF) {
        switch (c) {
            case 'o': bBKLOn  = true; break;
            case 'f': bBKLOff = true; break;
            case 'r': bVer    = true; break;
            case 'm': bModel  = true; break;
            case 'x': bGetRTD = true; break;
            case 'l': bGetHLD = true; break;
	        case 'c': bGetARH = true; break;
            case 'g': bGetGD  = true; break;
            case 'v': bVerbose= true; break;
            case 't': bGetTime= true; break;
            case 's': bSetTime= true; break;

            case 'd':
                /* Get delay time */
                i = atoi(optarg);
                if(i < 0 || i > 255) {
                    fprintf(stderr, "vproweather: Illegal delay specified.\n");
                    return 0;
                }
                yDelay = (unsigned char)i;
                break;

            case '?': /* user entered unknown option */
            case ':': /* user entered option without required value */
            case 'h': /* User wants Usage message */
                return 0;

            default:
                break;
        }

        if (optind < argc)          /* get serial device name */
            strcpy(szttyDevice, argv[optind]);
        else {
            fprintf(stderr, "vproweather: No serial device specified\n");
            return 0;
        }
    }
    return -1;
}



/*--------------------------------------------------------------------------
    WakeUp
    Wakes up the weather station per the Davis specs.
----------------------------------------------------------------------------*/
int WakeUp(int nfd)
{
    char ch;
    int i;

    for(i = 0; i < 3; i++)
    {
        ch = '\r';
        /* send wake up char */
        if(write(nfd, &ch, 1) != 1)
        {
            perror(szSWriteErr);
            exit(2);
        }
        if(ReadNextChar(nfd, &ch))      /* read a char */
        {
            if(bVerbose)
                printf("Weather station woke up after %d retries\n", i+1);
            return -1;                  /* okay, its awake now */
        }
    }

    return 0;                           /* failed to wake up */
}




/*--------------------------------------------------------------------------
    ReadNextChar
    Reads the next character from the serial device. Returns zero if no
    character was available.
----------------------------------------------------------------------------*/
int ReadNextChar(int nfd, char *pChar)
{
    int nResult;

    nResult = read(nfd, pChar, 1);
    if(nResult == -1) {
        perror("vproweather: Problem reading serial device.");
        exit(2);
    }
    return nResult;
}




/*--------------------------------------------------------------------------
    Delay
    Delays by the number of seconds and microseconds.
----------------------------------------------------------------------------*/
void Delay(int secs, long microsecs)
{
    static struct timeval t1;

    t1.tv_sec = (long)secs;
    t1.tv_usec = microsecs;
    if ( select(0, (fd_set *)0, (fd_set *)0, (fd_set *)0, &t1)  < 0 )
         perror("Internal error: error in select()");
    return;
}




/*--------------------------------------------------------------------------
    ReadToBuffer
    Reads data to a buffer until no more characters are available. If
    the buffer overflows, returns -1. Otherwise, returns the number of
    characters read.
----------------------------------------------------------------------------*/
int ReadToBuffer(int nfd, char *pszBuffer, int nBufSize)
{
    int nPos = 0;               /* current character position */
    char *pBuf = pszBuffer;

    while(nPos < nBufSize) {
        if(!ReadNextChar(nfd, pBuf++))
            return nPos;        /* no character available */
        ++nPos;
    }
    return -1;                  /* buffer overflow */
}


