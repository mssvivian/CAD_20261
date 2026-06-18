CC = mpicc
CFLAGS = -O2 -Wall
LDFLAGS = -lm

all: hello_gather minmax

hello_gather: hello_gather.c
	$(CC) $(CFLAGS) -o hello_gather hello_gather.c $(LDFLAGS)

minmax: minmax.c
	$(CC) $(CFLAGS) -o minmax minmax.c $(LDFLAGS)

run: all
	mpiexec -n 4 ./hello_gather
	mpiexec -n 4 ./minmax
	
run_hello: hello_gather
	mpiexec -n 4 ./hello_gather

run_minmax: minmax
	mpiexec -n 4 ./minmax

clean:
	rm -f hello_gather minmax