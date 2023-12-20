LIB_DIR=-L$(SYSTEMC_HOME)/lib-linux64
INC_DIR=-I$(SYSTEMC_HOME)/include
LIB=-lsystemc -Wl,-rpath,$(SYSTEMC_HOME)/lib-linux64

.PHONY:one many

SRC=DecoupledIO.hpp

one:
	g++ -o one.out one2one.cpp $(LIB_DIR) $(INC_DIR) $(LIB) -std=c++20

many:
	g++ -o many.out one2many.cpp $(LIB_DIR) $(INC_DIR) $(LIB) -std=c++20
