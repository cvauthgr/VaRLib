SRC = main.cpp
OUT = main

CXX = g++
CXXFLAGS = -std=c++23 -g -Wall -Wextra -Wpedantic -Wshadow
EXTRAWARNINGS = -std=c++23 -g \
           		-Wall -Wextra -Wpedantic \
                -Wshadow -Wconversion -Wsign-conversion \
                -Wcast-align -Wcast-qual -Wold-style-cast \
                -Wnon-virtual-dtor -Woverloaded-virtual \
        		-Wnull-dereference -Wdouble-promotion \
                -Wformat=2 -Wimplicit-fallthrough \
                -Wmisleading-indentation -Wduplicated-cond \
                -Wduplicated-branches -Wlogical-op \
                -Wuseless-cast \
                -fanalyzer

CHECKER = cppcheck
CHECKERFLAGS =  --enable=all --inconclusive --std=c++23 --suppress=missingIncludeSystem --inline-suppr --check-level=exhaustive 
	         	         

$(OUT) : $(SRC)
	$(CXX) $(CXXFLAGS) -o $(OUT) $(SRC)

analyze :
	$(CHECKER) $(CHECKERFLAGS) $(SRC)

insanechecks : $(SRC)
	$(CXX) $(CXXFLAGS) $(EXTRAWARNINGS) -o $(OUT) $(SRC)
	
delete :
	rm $(OUT).exe
