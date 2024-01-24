LIB_DIR=-L$(SYSTEMC_HOME)/lib-linux64
INC_DIR=-I$(SYSTEMC_HOME)/include
LIB=-lsystemc -Wl,-rpath,$(SYSTEMC_HOME)/lib-linux64

.PHONY:default

SRC=DecoupledIO.hpp

default:
	g++ -o pass-pointer.out pass-pointer.cpp $(LIB_DIR) $(INC_DIR) $(LIB) -std=c++20

