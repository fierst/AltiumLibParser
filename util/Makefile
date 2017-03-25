CXX=g++
CXXFLAGS=-c -Wall
INCDIRS=-Iinc -I../inc
SRCDIR=src
LDFLAGS=-Wall
LDLIBS= -lboost_filesystem -lboost_system

OBJS=print_header_info.o

all: print_header_info

print_header_info: print_header_info.o
	$(CXX) $(LDFLAGS) print_header_info.o -o print_header_info $(LDLIBS)
	rm $(OBJS)

print_header_info.o: src/print_header_info.cpp inc/print_header_info.hpp
	$(CXX) $(CXXFLAGS) $(SRCDIR)/print_header_info.cpp $(INCDIRS)

clean:
	\rm -f *.o print_header_info
	 
