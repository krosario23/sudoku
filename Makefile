CC=gcc
CFLAGS=-I
DEPS = board.h
OBJ  = board.o sudoku.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

sudoku: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f sudoku $(OBJ)
