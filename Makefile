OBJS = main.c world.c economy.c utils.c demographics.c constants.c queries.c government.c
CC = gcc

INCLUDE_PATHS =

LIBRARY_PATHS =

COMPILER_FLAGS = -Wall

LINKER_FLAGS =

OBJ_NAME = main

all : $(OBJS)
	$(CC) $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)