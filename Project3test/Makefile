OBJS = test0_directorywalker.c test1_inodebitmapwalker.c test2_zonebitmapwalker.c test3_damageinodebitmap.c test4_repairinodebitmap.c test5_damagezonebitmap.c test6_repairzonebitmap
OBJS_OUT = directorywalker inodewalker zonewalker damageinodemap fixinodemap damagezonemap fixzonemap
CC = clang
DEBUG = 
FLAGS = -Wall $(DEBUG)

clean:
	rm -f $(OBJS_OUT)

tests:
	$(CC) $(FLAGS) test0_directorywalker.c -o directorywalker
	$(CC) $(FLAGS) test1_inodebitmapwalker.c -o inodewalker
	$(CC) $(FLAGS) test2_zonebitmapwalker.c -o zonewalker
	$(CC) $(FLAGS) test3_damageinodebitmap.c -o damageinodemap
	$(CC) $(FLAGS) test4_repairinodebitmap.c -o fixinodemap
	$(CC) $(FLAGS) test5_damagezonebitmap.c -o damagezonemap
	$(CC) $(FLAGS) test6_repairzonebitmap.c -o fixzonemap



all: clean tests