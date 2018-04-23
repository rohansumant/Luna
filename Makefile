main: main.cpp
	g++ -g -std=c++14 -o main main.cpp -I.

clean:
	-rm main
	-rm -r main.dSYM
