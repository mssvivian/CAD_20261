CC = mpicc
CFLAGS = -O2 -Wall
LDFLAGS = -lm

all: mpi_psum

mpi_psum: mpi_psum.c
	$(CC) $(CFLAGS) -o mpi_psum mpi_psum.c $(LDFLAGS)

run: mpi_psum
	mpiexec -n 4 ./mpi_psum

clean:
	rm -f mpi_psum