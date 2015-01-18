CC=gcc
CFLAGS=-O2 -g -Wall -pedantic -pg
LDFLAGS=-lprintCapture -L.
LIBS=-lpthread -lcrypto

SUFFIXES=.c .o

OBJECTS=libprintCapture.o printCapture.o

PROGS=libprintCapture.a printCapture

all: $(PROGS)

.c.o:
	$(CC) $(CFLAGS) -c $<

printCapture: printCapture.o
	$(CC) $(CFLAGS) printCapture.o $(LDFLAGS) -o printCapture $(LIBS)

libprintCapture.a: libprintCapture.o
	ar rv libprintCapture.a $?
	ranlib libprintCapture.a
	@echo "Library compiled"
	@echo ""

clean:
	rm -f $(OBJECTS) $(PROGS) *~ gmon.out *.capture
