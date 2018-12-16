IDIR=include
BDIR=bin

SUFFIXES += .d
NODEPS:=clean tags svn
SOURCES:=$(shell find src/ -name "*.cpp")
DEPFILES:=$(patsubst %.cpp,%.d,$(SOURCES))

CXX=g++
CXXFLAGS=-I$(IDIR) -Wshadow -Wall -Wextra -DASIO_STANDALONE -std=c++17
OFLAGS=-O3 -march=native

ODIR=obj

OBJS:=$(patsubst %.cpp, obj/%.o, $(shell find src/ -name '*.cpp' -type f -printf "%f "))

fsa-test: $(OBJS)
	$(CXX) -o $(BDIR)/$@ $^ $(CXXFLAGS) $(LIBS)

include Rulefile

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o $(BDIR)/*
