SRC = main.cpp
OUT = main

CXX = g++
CXXFLAGS = -std=c++23 -g -Wall -Wextra \
		   -Wpedantic -Wshadow 

INSANEFLAGS = -std=c++23 -g -Wall -Wextra \
			  -Wpedantic -Wshadow -Wconversion \
			  -Wsign-conversion -Wcast-align \
			  -Wcast-qual -Wold-style-cast \
			  -Wnon-virtual-dtor -Woverloaded-virtual \
			  -Wnull-dereference -Wdouble-promotion -Wformat=2 \
			  -Wimplicit-fallthrough -Wmisleading-indentation \
			  -Wduplicated-cond -Wduplicated-branches \
			  -Wlogical-op -Wuseless-cast -fanalyzer 

$(OUT): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(OUT) $(SRC)

insanechecks:
	$(CXX) $(INSANEFLAGS) -o $(OUT) $(SRC)

analyze:
	cppcheck --enable=all --inconclusive --std=c++23 --suppress=missingIncludeSystem --inline-suppr --check-level=exhaustive --platform=win64 --error-exitcode=1 $(SRC) 

clean:
	del $(OUT).exe
	
