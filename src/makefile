##Please specify the local path of gsl in line 3&4####
OnTAD:	main.cpp step1.cpp step2.cpp step3.cpp step4.cpp common.cpp
	g++ -c main.cpp step1.cpp step2.cpp step3.cpp step4.cpp common.cpp
	g++ main.o step1.o step2.o step3.o step4.o common.o -lm -static -o OnTAD
	rm main.o step1.o step2.o step3.o step4.o common.o 

clean:
	rm OnTAD
 
