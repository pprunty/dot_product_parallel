CC	:= g++
CFLAGS 	:= -Wall -Wextra -O3 --std=c++17
LDFLAGS := -lm
EXE 	:= dot_product_parallel
CFILES	:= dot_product_parallel.cc
OBJECTS := dot_product_parallel.o

all: ${EXE}

$(EXE): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(EXE) $(OBJECTS) $(LDFLAGS)

dot_product_parallel.o: dot_product_parallel.cc
	$(CC) $(CFLAGS) -c dot_product_parallel.cc


.PHONY: clean
clean:
	rm -f $(EXE) $(OBJECTS)