CC=g++ -O3 -std=c++14
SRCS=$(wildcard src/*.cpp)

pmlsh:$(SRCS)
	$(CC) $(SRCS) -o pmlsh

clean:
	rm -rf pmlsh

cleancode:
	rm -rf $(wildcard src/*.cpp) $(wildcard src/*.h)