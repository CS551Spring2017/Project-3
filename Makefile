OBJS = test-directorywalker.c test-inodewalker.c test-zonewalker.c
OBJS_OUT = directorywalker inodewalker zonewalker
CC = clang
DEBUG = 
FLAGS = -Wall $(DEBUG)

clean:
	rm -f $(OBJS_OUT)

tests:
	$(CC) $(FLAGS) test-directorywalker.c -o directorywalker
	$(CC) $(FLAGS) test-inodewalker.c -o inodewalker
	$(CC) $(FLAGS) test-zonewalker.c -o zonewalker

all: clean tests