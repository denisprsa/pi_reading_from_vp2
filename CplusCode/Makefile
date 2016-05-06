CC = g++
CFLAGS  = -g -Wall

TARGET = vproweather
all:	$(TARGET)

$(TARGET): main.o VremenskaPostaja.o
	$(CC) main.o VremenskaPostaja.o -o $(TARGET)

main.o: main.cpp main.h VremenskaPostaja.h
	$(CC) $(CFLAGS) -c main.cpp

dhandler.o: VremenskaPostaja.cpp VremenskaPostaja.h
	$(CC) $(CFLAGS) -c VremenskaPostaja.cpp

clean:
	rm -f $(TARGET) *.o

