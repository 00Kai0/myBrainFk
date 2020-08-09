CC=     gcc
SOURCE= ./
OUTPUT= ./bin/

all:     build
build:  $(SOURCE)bf.c
	if [ ! -d $(OUTPUT) ]; then \
		mkdir $(OUTPUT); \
	fi
	$(CC) $(SOURCE)bf.c -o $(OUTPUT)bf
clean:  
	rm -rf $(OUTPUT)
