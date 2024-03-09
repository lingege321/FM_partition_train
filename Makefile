
all: Lab4

CXX = g++

LINKFLAGS = -pedantic -Wall -fomit-frame-pointer -funroll-all-loops -O3

Lab4: main.o fmNet.o fmNode.o fmPart.o fmBucket.o  parser.o
	$(CXX) $(LINKFLAGS) main.o fmNet.o fmNode.o fmPart.o fmBucket.o  parser.o -o Lab4

main.o: main.cpp
	$(CXX) $(LINKFLAGS) -DCOMPILETIME="\"`date`\"" main.cpp -c

fmNode.o: fmNode.cpp fmDM.h
	$(CXX) $(LINKFLAGS) fmNode.cpp -c

fmNet.o: fmNet.cpp fmDM.h
	$(CXX) $(LINKFLAGS) fmNet.cpp -c

fmPart.o: fmPart.cpp fmDM.h
	$(CXX) $(LINKFLAGS) fmPart.cpp -c

fmBucket.o: fmBucket.cpp fmDM.h
	$(CXX) $(LINKFLAGS) fmBucket.cpp -c

parser.o: parser.cpp parser.h
	$(CXX) $(LINKFLAGS) parser.cpp -c

clean:
	rm -rf *.o *.gch Lab4






