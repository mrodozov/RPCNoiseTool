ROOTINC = $(shell root-config --cflags) 
ROOTLIBS = $(shell root-config --glibs)
COMPILE_AS_Cpp0X = $(-std=C++14)

all: LBNoise MergeFiles

LBnoise.o : LBnoise.cc
	g++ -c -Wall LBnoise.cc $(ROOTINC)

LBName.o : LBName.cc
	g++ -c -Wall LBName.cc $(ROOTINC)

LBNoise : LBnoise.o LBName.o
	g++ main.cpp -o LBNoise LBnoise.o LBName.o $(ROOTLIBS) $(ROOTINC)
	
MergeFiles: MergeRoots.cc
	g++ MergeRoots.cc -o MergeRoots $(ROOTLIBS) $(ROOTINC) $(COMPILE_AS_Cpp0X)

clean:
	rm -rf *.o *~ 
	mv *.txt *.root results
