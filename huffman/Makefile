CC=clang
CFLAGS=-Werror -Wall -Wextra -Wconversion -Wdouble-promotion -Wstrict-prototypes -pedantic
OBJS=bitreader.o bitwriter.o 

HEAD=bitreader.h  bitwriter.h pq.h node.h
EXEC=test


.PHONY: clean format scan-build

all: huff dehuff #brtest bwtest nodetest pqtest

huff: huff.o $(OBJS) pq.o node.o
	$(CC) $(CFLAGS) $^ -o $@

dehuff: dehuff.o $(OBJS) pq.o node.o
	$(CC) $(CFLAGS) $^ -o $@

#brtest: brtest.o $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@
#bwtest: bwtest.o $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@
#nodetest: nodetest.o node.o
	$(CC) $(CFLAGS) $^ -o $@

#pqtest:pqtest.o pq.o node.o $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ 

$(EXEC): $(OBJS)
	$(CC) -o $(EXEC) $(OBJS)

%.o: %.c $(HEAD)
	$(CC) $(CFLAGS) -c $< -o $@
	
clean:
	rm -f $(EXEC) *.o *.gch huff dehuff *test

scan-build: clean
	scan-build --use-cc=clang make

format:
	clang-format -i -style=file *.[ch]



