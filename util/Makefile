CXX=g++
CXXFLAGS=-c -Wall -g -std=c++11
INCDIRS=-Iinc -I../inc
BASESRCDIR=../src
UTILSRCDIR=src
LDFLAGS=-Wall
LDLIBS= -lboost_filesystem -lboost_system
BINDIR=bin
MKDIR=mkdir -p
OBJS=print_header_info direntries

all: output_dirs $(OBJS)

print_header_info: print_header_info.o
	$(CXX) $(LDFLAGS) print_header_info.o -o $(BINDIR)/print_header_info $(LDLIBS)
	\rm print_header_info.o

print_header_info.o: $(UTILSRCDIR)/print_header_info.cpp
	$(CXX) $(CXXFLAGS) $(UTILSRCDIR)/print_header_info.cpp $(INCDIRS)

direntries: direntries.o cfb_types.o
	$(CXX) $(LDFLAGS) direntries.o cfb_types.o -o $(BINDIR)/direntries $(LDLIBS)
	\rm direntries.o cfb_types.o

direntries.o: $(UTILSRCDIR)/direntries.cpp
	$(CXX) $(CXXFLAGS) $(UTILSRCDIR)/direntries.cpp $(INCDIRS)

cfb_types.o: $(BASESRCDIR)/cfb_types.cpp
	$(CXX) $(CXXFLAGS) $(BASESRCDIR)/cfb_types.cpp $(INCDIRS)

output_dirs: $(BINDIR)
	
$(BINDIR):
	$(MKDIR) $(BINDIR)

clean:
	\rm -f *.o $(BINDIR)/print_header_info $(BINDIR)/direntries
	\rm -r $(BINDIR)
	 
