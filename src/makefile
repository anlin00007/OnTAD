##Note: in some cases, static linking is neccessary. If it's needed, please add '-static' in line 3####
OnTAD:	main.cpp step1.cpp step2.cpp step3.cpp step4.cpp common.cpp
	g++ -c main.cpp step1.cpp step2.cpp step3.cpp step4.cpp common.cpp
	g++ main.o step1.o step2.o step3.o step4.o common.o -lm -o OnTAD
	rm main.o step1.o step2.o step3.o step4.o common.o 
	./OnTAD ../test/mES_rep2.40Kb.raw.chr19.mat -o ../mES_rep2.40Kb.raw.chr19

clean:
	rm OnTAD
 
