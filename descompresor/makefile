##si copian el del telegram va con error de*** missing separator.  Stop.

CC = g++
SOURCE = source
OBJ = FDCSR.o
OUT = descomprimir.bin
OUTDIR = ..
HEADER = decompresor.h
CFLAGS = -I./$(SOURCE) -c -Wall

MKDIR = mkdir -p


edit: $(OBJ)
	$(MKDIR) $(OUTDIR)
	$(CC) -o $(OUTDIR)/$(OUT) $(OBJ)

FDCSR.o: FDCSR.cpp $(SOURCE)/$(HEADER)
	$(CC) $(CFLAGS) FDCSR.cpp

.PHONY: clean
clean:
	rm $(OUTDIR)/$(OUT) $(OBJ)

run:
	$(OUTDIR)/$(OUT)