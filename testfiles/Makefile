OBJS = test-directorywalker.c test-inodewalker.c test-zonewalker.c test-damageinodemap.c test-fixinodemap.c test-damagezonemap.c test-fixzonemap
OBJS_OUT = directorywalker inodewalker zonewalker damageinodemap fixinodemap damagezonemap fixzonemap
CC = clang
DEBUG = 
FLAGS = -Wall $(DEBUG)

clean:
	rm -f $(OBJS_OUT)

tests:
	$(CC) $(FLAGS) test-directorywalker.c -o directorywalker
	$(CC) $(FLAGS) test-inodewalker.c -o inodewalker
	$(CC) $(FLAGS) test-zonewalker.c -o zonewalker
	$(CC) $(FLAGS) test-damageinodemap.c -o damageinodemap
	$(CC) $(FLAGS) test-fixinodemap.c -o fixinodemap
	$(CC) $(FLAGS) test-damagezonemap.c -o damagezonemap
	$(CC) $(FLAGS) test-fixzonemap.c -o fixzonemap



all: clean tests