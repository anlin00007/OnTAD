##Note: in some cases, static linking is neccessary. If it's needed, please add '-static' in line 3####
OnTAD:	main.cpp step1.cpp step2.cpp step3.cpp step4.cpp common.cpp straw.cpp
	g++ -std=c++11 main.cpp step1.cpp step2.cpp step3.cpp step4.cpp common.cpp straw.cpp -lm -lcurl -lz -o OnTAD
	./OnTAD ../test/mES_rep2.40Kb.raw.chr19.mat -o ../mES_rep2.40Kb.raw.chr19 -bedout chr19 61342430 40000 

clean:
	rm -f OnTAD
 
