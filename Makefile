
CCNAME = HClustering
CC = g++
CCFLAGS = -W -g3 -std=c++14

PATH_BIN = ./bin
PATH_INC = ./include
PATH_SRC = ./src

all:
	@echo "\n*** Compiling executable "$(CCNAME)" ***"
	@echo "***"
	mkdir -p $(PATH_BIN)
	$(CC) $(DEFLAGS) $(CCFLAGS) -I $(PATH_INC)/ $(PATH_SRC)/vector2.cpp $(PATH_SRC)/main.cpp -o $(PATH_BIN)/$(CCNAME)
	@echo "***"

verbose:
	make all DEFLAGS="-D __VERBOSE__"

clean:
	@echo "\n*** Purging executable "$(CCNAME)" ***"
	@echo "***"
	rm -rv $(PATH_BIN)
	@echo "***"
