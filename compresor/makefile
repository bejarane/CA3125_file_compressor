##si copian el del telegram va con error de*** missing separator.  Stop.

CC = g++
SOURCE = source
OBJ = FCSR.o
OUT = comprimir.bin
OUTDIR = ..
HEADER = compresor.h
CFLAGS = -I./$(SOURCE) -c -Wall

MKDIR = mkdir -p


edit: $(OBJ)
	$(MKDIR) $(OUTDIR)
	$(CC) -o $(OUTDIR)/$(OUT) $(OBJ)

FCSR.o: FCSR.cpp $(SOURCE)/$(HEADER)
	$(CC) $(CFLAGS) FCSR.cpp

.PHONY: clean
clean:
	rm $(OUTDIR)/$(OUT) $(OBJ)

run:
	$(OUTDIR)/$(OUT)