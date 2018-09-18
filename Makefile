INST_DIR = $(HOME)/bin
BIN  = combine_agrid_files_ascii

cflags = -O -Wall 
CFLAGS = $(DBG) $(cflags)
CC = gcc

DBG = 
FOBJS = $(BIN).c 

$(BIN) : $(FOBJS)
	$(CC) $(CFLAGS) $(FOBJS) -o $(BIN)

install :: $(BIN)
	install -s $(BIN) $(INST_DIR)

clean :: 
	rm $(BIN) 
