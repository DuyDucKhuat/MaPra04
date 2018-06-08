	LIBS = -lX11 -pthread

main: vierGewinnt.cpp Spielbrett.o unit.o plotx11.o
	$(CXX) $(LIBS) -g -std=c++11 -o $@ $^
Spielbrett.o: Spielbrett.cpp
	$(CXX) -g -std=c++11 -c $<
.PHONY: clean
clean:
	rm -f main Spielbrett.o 
	 
