CXX=g++
CXXFLAGS=-std=c++11 -g
BIN=balltree

$(BIN):main.cpp
	$(CXX) $(CXXFLAGS) -o $@ $+
