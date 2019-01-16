##Please specify the local path of gsl in line 4&5####
##Note: in some cases, static linking is neccessary. If it's needed, please add '-static' in line 5####
OnTAD:	main.cpp step1.cpp step2.cpp step3.cpp step4.cpp common.cpp
	g++ -I. -I/PathToGSL/gsl-2.3/include -c main.cpp step1.cpp step2.cpp step3.cpp step4.cpp common.cpp
	g++ -L/PathToGSL/gsl-2.3/lib main.o step1.o step2.o step3.o step4.o common.o -lgsl -lgslcblas -lm -o OnTAD
	rm main.o step1.o step2.o step3.o step4.o common.o 

clean:
	rm OnTAD
 
