CXX=g++
CXXFLAGS=-std=c++11
BIN=balltree

$(BIN):main.cpp
	$(CXX) $(CXXFLAGS) -o $@ $+
