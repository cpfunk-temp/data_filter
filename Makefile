main: main.o find_objects.o data_filter.o
	gcc -g -o main main.o find_objects.o data_filter.o -lm -fno-strict-aliasing -Wall

main.o: main.c
	gcc -g main.c -c -lm -fno-strict-aliasing -Wall

find_objects.o: find_objects.c
	gcc -g find_objects.c -c -lm -fno-strict-aliasing -Wall

data_filter.o: data_filter.c
	gcc -g data_filter.c -c -lm -fno-strict-aliasing -Wall