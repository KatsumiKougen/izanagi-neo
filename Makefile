CXX=g++
CXXFLAGS=-std=c++17 -Wall -Wextra -pedantic
IZNEO=izng-neo.cpp
IZHR=izng-neo-highres.cpp
IZMAX=izng-neo-hrmax.cpp
IZHRC=izng-neo-hrcol.cpp
IZMAXC=izng-neo-hrmaxcol.cpp

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
OUTPUTMAX=izng-neo-hrmax
OUTPUTMAX-O=izng-neo-hrmax-o
OUTPUTMAX-F=izng-neo-hrmax-f
OUTPUTMAX-S=izng-neo-hrmax-s
OUTPUTHRC=izng-neo-hrcol
OUTPUTHRC-O=izng-neo-hrcol-o
OUTPUTHRC-F=izng-neo-hrcol-f
OUTPUTHRC-S=izng-neo-hrcol-s
OUTPUTMAXC=izng-neo-hrmaxcol
OUTPUTMAXC-O=izng-neo-hrmaxcol-o
OUTPUTMAXC-F=izng-neo-hrmaxcol-f
OUTPUTMAXC-S=izng-neo-hrmaxcol-s

all: all-color all-highres all-max
all-color: opt opt-f opt-s no-opt
all-highres: hr-opt hr-opt-f hr-opt-s hr-no-opt
all-max: max-opt max-opt-f max-opt-s max-no-opt
all-hrcol: hrc-opt hrc-opt-f hrc-opt-s hrc-no-opt
all-max: maxc-opt maxc-opt-f maxc-opt-s maxc-no-opt

opt: izng-neo.cpp
	$(CXX) $(IZNEO) $(CXXFLAGS) $(NCURSES) $(OPT) -o $(OUTPUT-O)
opt-f: izng-neo.cpp
	$(CXX) $(IZNEO) $(CXXFLAGS) $(NCURSES) $(OPTF) -o $(OUTPUT-F)
opt-s: izng-neo.cpp
	$(CXX) $(IZNEO) $(CXXFLAGS) $(NCURSES) $(OPTS) -o $(OUTPUT-S)
no-opt: izng-neo.cpp
	$(CXX) $(IZNEO) $(CXXFLAGS) $(NCURSES) -o $(OUTPUT)

hr-opt: izng-neo-highres.cpp
	$(CXX) $(IZHR) $(CXXFLAGS) $(NCURSES) $(OPT) -o $(OUTPUTHR-O)
hr-opt-f: izng-neo-highres.cpp
	$(CXX) $(IZHR) $(CXXFLAGS) $(NCURSES) $(OPTF) -o $(OUTPUTHR-F)
hr-opt-s: izng-neo-highres.cpp
	$(CXX) $(IZHR) $(CXXFLAGS) $(NCURSES) $(OPTS) -o $(OUTPUTHR-S)
hr-no-opt: izng-neo-highres.cpp
	$(CXX) $(IZHR) $(CXXFLAGS) $(NCURSES) -o $(OUTPUTHR)

max-opt: izng-neo-hrmax.cpp
	$(CXX) $(IZMAX) $(CXXFLAGS) $(NCURSES) $(OPT) -o $(OUTPUTMAX-O)
max-opt-f: izng-neo-hrmax.cpp
	$(CXX) $(IZMAX) $(CXXFLAGS) $(NCURSES) $(OPTF) -o $(OUTPUTMAX-F)
max-opt-s: izng-neo-hrmax.cpp
	$(CXX) $(IZMAX) $(CXXFLAGS) $(NCURSES) $(OPTS) -o $(OUTPUTMAX-S)
max-no-opt: izng-neo-hrmax.cpp
	$(CXX) $(IZMAX) $(CXXFLAGS) $(NCURSES) -o $(OUTPUTMAX)

hrc-opt: izng-neo-hrcol.cpp
	$(CXX) $(IZHRC) $(CXXFLAGS) $(NCURSES) $(OPT) -o $(OUTPUTHRC-O)
hrc-opt-f: izng-neo-hrcol.cpp
	$(CXX) $(IZHRC) $(CXXFLAGS) $(NCURSES) $(OPTF) -o $(OUTPUTHRC-F)
hrc-opt-s: izng-neo-hrcol.cpp
	$(CXX) $(IZHRC) $(CXXFLAGS) $(NCURSES) $(OPTS) -o $(OUTPUTHRC-S)
hrc-no-opt: izng-neo-hrcol.cpp
	$(CXX) $(IZHRC) $(CXXFLAGS) $(NCURSES) -o $(OUTPUTHRC)

maxc-opt: izng-neo-hrmaxcol.cpp
	$(CXX) $(IZMAXC) $(CXXFLAGS) $(NCURSES) $(OPT) -o $(OUTPUTMAXC-O)
maxc-opt-f: izng-neo-hrmaxcol.cpp
	$(CXX) $(IZMAXC) $(CXXFLAGS) $(NCURSES) $(OPTF) -o $(OUTPUTMAXC-F)
maxc-opt-s: izng-neo-hrmaxcol.cpp
	$(CXX) $(IZMAXC) $(CXXFLAGS) $(NCURSES) $(OPTS) -o $(OUTPUTMAXC-S)
maxc-no-opt: izng-neo-hrmaxcol.cpp
	$(CXX) $(IZMAXC) $(CXXFLAGS) $(NCURSES) -o $(OUTPUTMAXC)
