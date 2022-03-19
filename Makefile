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
OUTPUTHR=izng-neo-highres
OUTPUTHR-O=izng-neo-highres-o
OUTPUTHR-F=izng-neo-highres-f
OUTPUTHR-S=izng-neo-highres-s

all: all-color all-highres
all-color: opt opt-f opt-s no-opt
all-highres: hr-opt hr-opt-f hr-opt-s hr-no-opt
opt: izng-neo.cpp
	 $(CXX) izng-neo.cpp $(CXXFLAGS) $(NCURSES) $(OPT) -o $(OUTPUT-O)
opt-f: izng-neo.cpp
	 $(CXX) izng-neo.cpp $(CXXFLAGS) $(NCURSES) $(OPTF) -o $(OUTPUT-F)
opt-s: izng-neo.cpp
	 $(CXX) izng-neo.cpp $(CXXFLAGS) $(NCURSES) $(OPTS) -o $(OUTPUT-S)
no-opt: izng-neo.cpp
	 $(CXX) izng-neo.cpp $(CXXFLAGS) $(NCURSES) -o $(OUTPUT)
hr-opt: izng-neo.cpp
	 $(CXX) izng-neo.cpp $(CXXFLAGS) $(NCURSES) $(OPT) -o $(OUTPUTHR-O)
hr-opt-f: izng-neo.cpp
	 $(CXX) izng-neo.cpp $(CXXFLAGS) $(NCURSES) $(OPTF) -o $(OUTPUTHR-F)
hr-opt-s: izng-neo.cpp
	 $(CXX) izng-neo.cpp $(CXXFLAGS) $(NCURSES) $(OPTS) -o $(OUTPUTHR-S)
hr-no-opt: izng-neo.cpp
	 $(CXX) izng-neo.cpp $(CXXFLAGS) $(NCURSES) -o $(OUTPUTHR)
