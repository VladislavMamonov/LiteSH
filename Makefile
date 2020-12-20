all: bin/LiteSH


bin/LiteSH: build/main.o build/LiteSH.o
	g++ -Wall -g build/main.o build/LiteSH.o -I./home/vladislav/OS/LiteSH/lib/libabout.so -ldl -o bin/LiteSH

build/main.o: src/main.cpp
	g++ -Wall -c -g src/main.cpp -o  build/main.o

build/LiteSH.o: src/LiteSH.cpp
	g++ -Wall -c -g src/LiteSH.cpp -o build/LiteSH.o


clean:
	rm -rf build/*.o bin/LiteSH
