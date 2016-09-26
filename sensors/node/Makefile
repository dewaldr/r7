# Robot7 Sensor Node Makefile

# Arduino.mk variables
ARDUINO_DIR			= /usr/share/arduino
ARDMK_DIR			= /usr
AVR_TOOLS_DIR		= /usr

TARGET				= node
ARDUINO_LIBS		= RFM12B EEPROM

BOARD_TAG			= uno
ARDUINO_PORT		= /dev/ttyUSB0
MONITOR_BAUDRATE	= 115200

include /usr/share/arduino/Arduino.mk

realclean: clean
	rm -f *~
	rm -rf build-uno
