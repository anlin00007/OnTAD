##Please specify the local path of gsl in line 3&4####
OnTAD:	main.cpp step1.cpp step2.cpp step3.cpp step4.cpp common.cpp
	g++ -I. -I/PathToGSL/gsl-2.3/include -c main.cpp step1.cpp step2.cpp step3.cpp step4.cpp common.cpp
	g++ -L/PathToGSL/gsl-2.3/lib main.o step1.o step2.o step3.o step4.o common.o -lgsl -lgslcblas -static -lm -o OnTAD
	rm main.o step1.o step2.o step3.o step4.o common.o 

clean:
	rm OnTAD
 
