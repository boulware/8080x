main: main.o cpu.o
	g++ main.o cpu.o -o main

main.o: main.cpp cpu.hpp
	g++ main.cpp -c -o main.o -std=c++11

cpu.o: cpu.cpp
	g++ cpu.cpp -c -o cpu.o -std=c++11