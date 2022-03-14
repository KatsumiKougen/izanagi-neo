CXX=g++
CXXFLAGS=-std=c++17 -Wall -Wextra -pedantic

NCURSES=-lncursesw
OPT=-O
OPTF=-Ofast
OPTS=-Os
OUTPUT=izng-neo
OUTPUT-O=izng-neo-o
OUTPUT-F=izng-neo-f
OUTPUT-S=izng-neo-s

all: opt opt-f opt-s no-opt
opt: izng-neo.cpp
	 $(CXX) izng-neo.cpp $(CXXFLAGS) $(NCURSES) $(OPT) -o $(OUTPUT-O)
opt-f: izng-neo.cpp
	 $(CXX) izng-neo.cpp $(CXXFLAGS) $(NCURSES) $(OPTF) -o $(OUTPUT-F)
opt-s: izng-neo.cpp
	 $(CXX) izng-neo.cpp $(CXXFLAGS) $(NCURSES) $(OPTS) -o $(OUTPUT-S)
no-opt: izng-neo.cpp
	 $(CXX) izng-neo.cpp $(CXXFLAGS) $(NCURSES) -o $(OUTPUT)
