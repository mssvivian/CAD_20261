CC = mpicc
CFLAGS = -O2 -Wall
LDFLAGS = -lm

all: gather allgather

gather: mpi_vecadd_gather.c
	$(CC) $(CFLAGS) -o mpi_vecadd_gather mpi_vecadd_gather.c $(LDFLAGS)

allgather: mpi_vecadd_allgather.c
	$(CC) $(CFLAGS) -o mpi_vecadd_allgather mpi_vecadd_allgather.c $(LDFLAGS)

run: all
	mpiexec -n 4 ./mpi_vecadd_gather
	mpiexec -n 4 ./mpi_vecadd_allgather

run_gather: gather
	mpiexec -n 4 ./mpi_vecadd_gather

run_allgather: allgather
	mpiexec -n 4 ./mpi_vecadd_allgather

clean:
	rm -f mpi_vecadd_gather mpi_vecadd_allgather