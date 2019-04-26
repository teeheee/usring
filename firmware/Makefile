MCU     = atmega8a
F_CPU     = 8000000UL
PROJECT = main

# ----- These configurations are quite likely not to be changed -----

# Binaries
GCC     = avr-gcc
G++     = avr-g++
RM      = rm -f
AVRDUDE = avrdude

# Files
EXT_C   = c
EXT_C++ = cpp
EXT_ASM = asm

# ----- No changes should be necessary below this line -----

OBJECTS = \
	$(patsubst %.$(EXT_C),%.o,$(wildcard *.$(EXT_C))) \
	$(patsubst %.$(EXT_C++),%.o,$(wildcard *.$(EXT_C++))) \
	$(patsubst %.$(EXT_ASM),%.o,$(wildcard *.$(EXT_ASM)))


CFLAGS = $(INC)
CFLAGS += -Os -std=gnu99
CFLAGS += -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -Wall -Wstrict-prototypes
CFLAGS += -DF_CPU=$(F_CPU)
CFLAGS += -mmcu=$(MCU)

C++FLAGS = $(INC)
C++FLAGS += -Os
C++FLAGS += -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
C++FLAGS += -Wall
C++FLAGS += -DF_CPU=$(F_CPU)
C++FLAGS += -mmcu=$(MCU)

ASMFLAGS = $(INC)
ASMFLAGS += -Os
ASMFLAGS += -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
ASMFLAGS += -Wall -Wstrict-prototypes
ASMFLAGS += -DF_CPU=$(F_CPU)
ASMFLAGS += -x assembler-with-cpp
ASMFLAGS += -mmcu=$(MCU)



default: $(PROJECT).elf
	avr-objcopy -j .text -j .data -O ihex $(PROJECT).elf $(PROJECT).hex

%.elf: $(OBJECTS)
	$(GCC) $(CFLAGS) $(OBJECTS) --output $@ $(LDFLAGS)


%.o : %.$(EXT_C)
	$(GCC) $< $(CFLAGS) -c -o $@

%.o : %.$(EXT_C++)
	$(G++) $< $(C++FLAGS) -c -o $@

%.o : %.$(EXT_ASM)
	$(G++) $< $(ASMFLAGS) -c -o $@

clean:
	$(RM) $(PROJECT).elf $(OBJECTS) $(PROJECT).hex

help:
	@echo "usage:"
	@echo "  make <target>"
	@echo ""
	@echo "targets:"
	@echo "  clean     Remove any non-source files"
	@echo "  config    Shows the current configuration"
	@echo "  help      Shows this help"
	@echo "  show-mcu  Show list of all possible MCUs"

config:
	@echo "configuration:"
	@echo ""
	@echo "Binaries for:"
	@echo "  C compiler:   $(GCC)"
	@echo "  C++ compiler: $(G++)"
	@echo "  Programmer:   $(AVRDUDE)"
	@echo "  remove file:  $(RM)"
	@echo ""
	@echo "Hardware settings:"
	@echo "  MCU: $(MCU)"
	@echo "  OSC: $(OSC)"
	@echo ""
	@echo "Defaults:"
	@echo "  C-files:   *.$(EXT_C)"
	@echo "  C++-files: *.$(EXT_C++)"
	@echo "  ASM-files: *.$(EXT_ASM)"

show-mcu:
	$(G++) --help=target

program:
	avrdude -pm8 -Pusb -cavrispmkII  -B10 -u -U flash:w:$(PROJECT).hex

AS7-tempate:
	rm -f usring-2.1-AS7-template.zip
	zip usring-2.1-AS7-template.zip MyTemplate.vstemplate __TemplateIcon.ico usring.cppproj main.cpp sensor.h sensor.c usring.cpp usring.h twislave.h twislave.c uart.h uart.c


fuse:
	avrdude -pm8 -Pusb -cavrispmkII -u -U lfuse:w:0xe4:m -U hfuse:w:0xd9:m -B10
