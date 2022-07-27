CXX = g++-11
CXXFLAGS = -fcoroutines -std=c++20

TARGET = testHash

DATASTRUCTS = HashTable.cpp

all: $(TARGET)

$(TARGET): $(TARGET).cpp
			$(CXX) $(CXXFLAGS) -o $(TARGET) $(TARGET).cpp $(DATASTRUCTS)

.PHONY : clean
clean :
	rm -f *.o *.obj $(TARGET)