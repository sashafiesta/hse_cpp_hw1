all:
	g++ -O3 -I. -o tests.o LongNumber.cpp tests.cpp
	g++ -O3 -I. -o computepi.o LongNumber.cpp computepi.cpp

test:
	g++ -O3 -I. -o tests.o LongNumber.cpp tests.cpp
	./tests.o
pi:
	g++ -O3 -I. -o computepi.o LongNumber.cpp computepi.cpp
	./computepi.o

clean:
	rm -f ./*.o
