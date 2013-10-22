SOURCES=Driver.cpp SudokuMatrix.cpp
Target= SudokuSolver

OBJS=$(SOURCES:.cpp=.o)

CPPFLAGS = -I. 
CXXFLAGS = -O3 -Wall -ffast-math

all: all_linux


%.o: %.cpp
	@echo "Compiling " $@
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@ 

all_linux: $(OBJS)
	$(warning Building...)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(OBJS) -o $(Target)

clean: 
	$(warning Cleaning...)
	@$(RM) -rf $(OBJS)
	@$(RM) $(Target)

.PHONY: all_linux clean

