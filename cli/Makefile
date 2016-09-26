#
# Makefile for 'r7gpio'.
#
# Type 'make' or 'make r7gpio' to create the binary.
# Type 'make clean' to delete all temporaries.
#

# build target specs
CC		= g++
CFLAGS	= -O3 
OUT_DIR	= release_build
LIBS	= -lwiringPi -lpthread

# first target entry is the target invoked when typing 'make'
default: r7gpio

r7gpio: $(OUT_DIR)/r7gpio.cpp.o
	@echo -n 'Linking r7gpio... '
	@$(CC) $(CFLAGS) -o r7gpio $(OUT_DIR)/r7gpio.cpp.o $(LIBS)
	@echo Done.

$(OUT_DIR)/r7gpio.cpp.o: r7gpio.cpp
	@echo -n 'Compiling r7gpio.cpp... '
	@$(CC) $(CFLAGS) -o $(OUT_DIR)/r7gpio.cpp.o -c r7gpio.cpp
	@echo Done.

clean:
	@echo -n 'Removing all temporary files... '
	@rm -f r7gpio $(OUT_DIR)/*.o
	@rm -f *~
	@echo Done.

