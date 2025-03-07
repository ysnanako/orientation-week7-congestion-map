#MAKEFILE

SRCS       := $(wildcard ./src/*.cpp)
OBJS       := $(patsubst ./src/%.cpp, ./obj/%.o, $(SRCS))

################################################################################

TARGET      = FLUTE_test

CXX         = g++ 
CXXFLAGS    = -O3 -Wall -g -pthread
LIB_FLUTE = -L ./flute-3.1 -lflute
INC_FLUTE = -I ./flute-3.1

# NVCC        = nvcc
# NVCC_FLAGS  = -I ./src/

#################################################################################

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ $(LIB_FLUTE) -o $@

./obj/%.o: ./src/%.cpp
	$(CXX) $(CXXFLAGS) -c $(INC_FLUTE) $< -o $@

clean:
	rm -f $(TARGET) ./obj/*.o core *~

rmlog:
	rm *.log
