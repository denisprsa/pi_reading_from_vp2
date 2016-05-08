CC = g++
CFLAGS  = -g -Wall -std=c++11

TARGET = vproweather
all:	$(TARGET)

$(TARGET): main.o WeatherStation.o
	$(CC) main.o WeatherStation.o -o $(TARGET)

main.o: main.cpp main.h WeatherStation.h
	$(CC) $(CFLAGS) -c main.cpp

dhandler.o: WeatherStation.cpp WeatherStation.h
	$(CC) $(CFLAGS) -c WeatherStation.cpp

clean:
	rm -f $(TARGET) *.o

