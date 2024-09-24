CXX = g++
CXXFLAGS = -std=c++17 -Wall

TARGET = karatsuba_gen
SRC = main.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET) *.o

clean_verilog:
	rm -f karatsuba_multiplier_*.v
