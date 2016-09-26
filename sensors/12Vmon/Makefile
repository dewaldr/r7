#
# Makefile for '12vmon'.
#
# Type 'make' or 'make 12vmon' to create the binary.
# Type 'make clean' to delete all temporaries.
#

# build target specs
CC      = g++
CCFLAGS = -fpermissive -Wall
LFLAGS  =
LIBS    =

default: 12vmon

12vmon: 12vmon.o mcp3008Spi.o
	$(CC) $(LFLAGS) -o 12vmon 12vmon.o mcp3008Spi.o $(LIBS)

12vmon.o: 12vmon.cpp
	$(CC) $(CCFLAGS) -o 12vmon.o -c 12vmon.cpp

mcp3008Spi.o: mcp3008Spi.cpp
	$(CC) $(CCFLAGS) -o mcp3008Spi.o -c mcp3008Spi.cpp

clean:
	@rm -f 12vmon *.o
	@rm -f *~

