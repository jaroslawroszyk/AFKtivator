CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra
LIBS = -lX11 -lXtst

TARGET = clicker

$(TARGET): main.cpp
	$(CXX) $(CXXFLAGS) $< -o $@ $(LIBS)

clean:
	rm -f $(TARGET)
