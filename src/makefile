##Note: in some cases, static linking is neccessary. If it's needed, please add '-static' in line 3####
OnTAD:	main.cpp step1.cpp step2.cpp step3.cpp step4.cpp common.cpp
	g++ -std=c++11 -c main.cpp step1.cpp step2.cpp step3.cpp step4.cpp common.cpp
	g++ -std=c++11 main.o step1.o step2.o step3.o step4.o common.o -lm -o OnTAD
	rm main.o step1.o step2.o step3.o step4.o common.o 

clean:
	rm OnTAD
 
