CC = mpicc
CFLAGS = -O2 -Wall
LDFLAGS = -lm

all: hello_os hello_ring

hello_os: hello_os.c
	$(CC) $(CFLAGS) -o hello_os hello_os.c $(LDFLAGS)

hello_ring: hello_ring.c
	$(CC) $(CFLAGS) -o hello_ring hello_ring.c $(LDFLAGS)

run: all
	mpiexec -n 4 ./hello_os
	mpiexec -n 4 ./hello_ring
	
run_os: hello_os
	mpiexec -n 4 ./hello_os

run_ring: hello_ring
	mpiexec -n 4 ./hello_ring

clean:
	rm -f hello_os hello_ring