main: main.cpp
	g++-7 -g -std=c++17 -o main main.cpp -I.

clean:
	-rm main
	-rm -r main.dSYM
