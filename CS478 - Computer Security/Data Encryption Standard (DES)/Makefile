#Brett Pelkey
#Michael Fadem
#2-27-16

#Programming Assignment 2
#CS478
#Computer Security

all : run

run : des
	./des

des : main.o des.o
	g++ -std=c++11 -g -o des des.o main.o

main.o : main.cc des.h
	g++ -std=c++11 -c main.cc

des.o : des.cc des.h
	g++ -std=c++11 -c des.cc

#clean target will get rid of all compiled files and the executable	
clean : 
	rm -f *.o des
