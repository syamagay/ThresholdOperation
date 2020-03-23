cc      =       g++
CFLAGS  =       -O4 -Wall -I/usr/local/include
PROGRAM =       run_analysis
PROGRAM2=       Core_NoiseDist
OBJS    =       tune_analysis.o Plotting_single.o GayaStyle.o
OBJS2   =       Core_NoiseDist.o GayaStyle.o
ROOTCFLAGS      =       $(shell root-config --cflags)
ROOTLIBS        =       $(shell root-config --libs)
ROOTGLIBS       =       $(shell root-config --glibs)

CXXFLAGS        =       $(ROOTCFLAGS) -Wall -fPIC
CXXLIBS         =       $(ROOTLIBS)

all:    $(PROGRAM) ${PROGRAM2}

$(PROGRAM):     $(OBJS)
		$(cc) $(CXXLIBS) $(OBJS) -o $(PROGRAM)
$(PROGRAM2):    $(OBJS2)
		$(cc) $(CXXLIBS) $(OBJS2) -o $(PROGRAM2)

clean:	
	-rm $(OBJS)
