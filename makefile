CC = gcc
CFLAGS = -Wall -pedantic
OBJECTS = driver.o ReadStat.o ReadProc.o

537ps: $(OBJECTS)
	$(CC) $(CFLAGS) -o 537ps $(OBJECTS)

driver.o: driver.c ReadProc.h ReadStat.h
	$(CC) $(CFLAGS) -c driver.c

ReadStat.o: ReadStat.c ReadStat.h
	$(CC) $(CFLAGS) -c ReadStat.c

ReadProc.o: ReadProc.c ReadProc.h
	$(CC) $(CFLAGS) -c ReadProc.c

clean:
	rm -f $(OBJECTS) 537ps
