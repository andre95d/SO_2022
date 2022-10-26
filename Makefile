DIR_NAME=bin
OBJ=lab02
CC=gcc
FLAGCS=-Wall -lm -g
target :
	mkdir $(DIR_NAME)
	$(CC) $(FLAGCS) -o $(OBJ) -lm main.c
	cp $(OBJ) $(DIR_NAME)

clean:
	rm $(DIR_NAME)/$(OBJ)

distclean: clean
	rm -r $(DIR_NAME)