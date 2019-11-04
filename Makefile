CXX=g++ 
CXXFLAGS=-Wall -pedantic -Werror -std=c++11 -pipe -Wl,--stack,600000000

all: debug 

debug: CXXFLAGS += -g -ggdb -O0 -DDEBUG
debug: compile

opt: CXXFLAGS+= -march=native -O3 -DNDEBUG
opt: compile

compile:
	$(CXX) $(CXXFLAGS) -o prog.exe *.cpp