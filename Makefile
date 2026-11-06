# Para comunicarse por medio de UART -> screen *PUERTO* *baud rate*
# Regularmente puerto es /dev/ttyACM0 y baud rate 9600
MCU = atmega328p
F_CPU = 16000000UL
SRC = main.c motor.c ultrasonico.c
TARGET = main
PROGRAMMER = arduino
PORT = /dev/ttyACM0

CC = avr-gcc
OBJCOPY = avr-objcopy
AVRDUDE = avrdude

CFLAGS = -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Os -o $(TARGET).elf $(SRC)
OBJ_FLAGS = -O ihex -R .eeprom $(TARGET).elf $(TARGET).hex
AVRDUDE_FLAGS = -c $(PROGRAMMER) -p $(MCU) \
				-P $(PORT) -b 115200 \
				-D -U flash:w:$(TARGET).hex:i

compile:
	$(CC) $(CFLAGS)
	$(OBJCOPY) $(OBJ_FLAGS)

flash:
	$(AVRDUDE) $(AVRDUDE_FLAGS)


clean:
	rm -rf $(TARGET).elf $(TARGET).hex

.PHONY: compile flash clean
