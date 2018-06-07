LIBS = -lX11 -pthread

main: vierGewinnt.cpp Spielbrett.o unit.o plotx11.o
	$(CXX) $(LIBS) -std=c++11 -o $@ $^
Spielbrett.o: Spielbrett.cpp
	$(CXX) -std=c++11 -c $<
.PHONY: clean
clean:
	rm -f main Spielbrett.o 
	 
