cc      =       g++
CFLAGS  =       -O4 -Wall -I/usr/local/include
PROGRAM =       bin/run_analysis
PROGRAM2=       bin/Core_NoiseDist
INCLUDE =       -I ./include

OBJ_DIR =       ./obj
SRC_DIR =       ./src
OBJS    =       $(OBJ_DIR)/Plotting_single.o $(OBJ_DIR)/GayaStyle.o #$(OBJ_DIR)/tune_evaluation_via.o
#OBJS    =       $(wildcard $(OBJ_DIR)/*.o) 

ROOTCFLAGS      =       $(shell root-config --cflags)
ROOTLIBS        =       $(shell root-config --libs)
ROOTGLIBS       =       $(shell root-config --glibs)

CXXFLAGS        =       $(ROOTCFLAGS) -Wall -fPIC
CXXLIBS         =       $(ROOTLIBS)

all:    $(PROGRAM) ${PROGRAM2}
	-mkdir -p ./bin

$(PROGRAM):     $(OBJS) $(OBJ_DIR)/main/tune_analysis.o
		$(cc) $(CXXLIBS) $(OBJS) $(OBJ_DIR)/main/tune_analysis.o $(INCLUDE) -o $(PROGRAM)

$(PROGRAM2):    $(OBJS) $(OBJ_DIR)/main/Core_NoiseDist.o
		$(cc) $(CXXLIBS) $(OBJS) $(OBJ_DIR)/main/Core_NoiseDist.o $(INCLUDE) -o $(PROGRAM2)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	-mkdir -p $(OBJ_DIR)
	$(cc) $(CXXFLAGS) $(CXXLIBS) $(INCLUDE) -o $@ -c $<

$(OBJ_DIR)/main/%.o : main/%.cpp
	-mkdir -p $(OBJ_DIR)/main
	$(cc) $(CXXFLAGS) $(CXXLIBS) $(INCLUDE) -o $@ -c $<


clean:	
	-rm $(OBJS)
	-rm $(wildcard $(OBJ_DIR)/main/*.o)
