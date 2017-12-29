CC = gcc
OPTIONS = -Wall
TARGET = glossary

#compile:
#	gcc -c main.c dictionary.c
#link:
#	gcc main.o dictionary.o

#all: compile link
$(TARGET): main.o dictionary.o
	$(CC) $(OPTIONS) -o $(TARGET) main.o dictionary.o
dictionary.o: dictionary.c
	$(CC) $(OPTIONS) -c dictionary.c
main.o: main.c
	$(CC) $(OPTIONS) -c main.c
clean:
	rm ./*.o $(TARGET)
