CXX = g++
LINKER = g++
CXXFLAGS = -Wall -std=c++0x -O3 -DNDEBUG

SOURCES:= bloom_filters.cpp

TARGET = bloom_filters

OBJECTS = $(patsubst %.cpp,%.o,$(SOURCES))

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET) : $(OBJECTS)
	$(LINKER) -o $(TARGET) $(OBJECTS)

all:
	$(OBJECTS) $(TARGET)

clean:
	-rm -f *.o $(TARGET)

default:
	all
