ROOTINC = $(shell root-config --cflags) 
ROOTLIBS = $(shell root-config --glibs)
COMPILE_AS_Cpp0X = $(-std=C++14)



all:  MergeFiles GetDeltaTListAndStartTimePerRunJSON

LBnoise.o : LBnoise.cc
	g++ -c -Wall LBnoise.cc $(ROOTINC) -std=c++14

LBName.o : LBName.cc
	g++ -c -Wall LBName.cc $(ROOTINC) -std=c++14

GetTimes.o :  GetDeltaTListAndStartTimePerRunJSON.cc
	g++ -c -Wall GetDeltaTListAndStartTimePerRunJSON.cc -o GetTimes.o $(ROOTINC) -std=c++14
	
LBNoise : LBnoise.o LBName.o
	g++ main.cpp -o LBNoise LBnoise.o LBName.o $(ROOTLIBS) $(ROOTINC) -std=c++14
	
GetDeltaTListAndStartTimePerRunJSON : GetTimes.o LBName.o
	g++ main_gettimes.cpp -o GetTimes GetTimes.o LBName.o $(ROOTLIBS) $(ROOTINC) -std=c++14
	
MergeFiles: MergeRoots.cc
	g++ MergeRoots.cc -o MergeRoots $(ROOTLIBS) $(ROOTINC) -std=c++14

clean:
	rm -rf *.o *~ 
	mv *.txt *.root results
