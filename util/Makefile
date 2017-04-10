CXX=g++
CXXFLAGS=-c -Wall -g
INCDIRS=-Iinc -I../inc
SRCDIR=src
LDFLAGS=-Wall
LDLIBS= -lboost_filesystem -lboost_system
BINDIR=bin
MKDIR=mkdir -p
OBJS=print_header_info direntries

all: output_dirs print_header_info direntries

print_header_info: print_header_info.o
	$(CXX) $(LDFLAGS) print_header_info.o -o $(BINDIR)/print_header_info $(LDLIBS)
	\rm print_header_info.o

print_header_info.o: $(SRCDIR)/print_header_info.cpp
	$(CXX) $(CXXFLAGS) $(SRCDIR)/print_header_info.cpp $(INCDIRS)

direntries: direntries.o
	$(CXX) $(LDFLAGS) direntries.o -o $(BINDIR)/direntries $(LDLIBS)
	\rm direntries.o

direntries.o: $(SRCDIR)/direntries.cpp
	$(CXX) $(CXXFLAGS) $(SRCDIR)/direntries.cpp $(INCDIRS)

output_dirs: $(BINDIR)
	
$(BINDIR):
	$(MKDIR) $(BINDIR)

clean:
	\rm -f *.o $(BINDIR)/print_header_info $(BINDIR)/direntries
	\rm -r $(BINDIR)
	 
