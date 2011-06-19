CC=g++
CFLAG= -Wall -pedantic -I.. -O3

all: MCM_pt MCM

MCM_pt: main_pt.cpp
	$(CC) $(CFLAG) -o MCM_pt main_pt.cpp -pthread

MCM: main.cpp
	$(CC) $(CFLAG) -o MCM main.cpp

clean:
	rm -f *.o *~ core
