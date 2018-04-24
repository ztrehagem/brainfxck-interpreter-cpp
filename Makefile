default: interpreter

interpreter: interpreter.cpp
	g++ interpreter.cpp -std=c++11 -o interpreter
