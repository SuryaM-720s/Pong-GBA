# Set the compiler and flags
CXX := arm-none-eabi-g++
CXXFLAGS := -std=gnu++14 -O2 -I/Users/surya/devkitPro/butano/include

# Source and output file definitions
SRCS := main.cpp
OUT := game.gba

# Define the build rule
$(OUT): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(OUT) $(SRCS)

# Clean rule to remove generated files
clean:
	rm -f $(OUT)

