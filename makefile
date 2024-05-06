CC=gcc
CFLAGS=-Wall -g
LDFLAGS=-lpthread

# List of source files
SOURCES=main.c mssv.c sudoku_reader.c
# List of object files
OBJECTS=$(SOURCES:.c=.o)
# The executable program to build
EXECUTABLE=mssv

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

.PHONY: clean
