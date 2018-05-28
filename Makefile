LIBS = -lX11 -pthread

main: Testfile.o unit.o plotx11.o
	$(CXX) $(LIBS) -std=c++11 -o $@ $^
Testfile.o: Testfile.cpp
	$(CXX) -c $<
.PHONY: clean
clean:
	rm -f main Testfile.o 
