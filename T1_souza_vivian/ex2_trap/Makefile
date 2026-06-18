CC = mpicc
CFLAGS = -O2 -Wall
LDFLAGS = -lm

all: mpi_trap_generalized

mpi_trap_generalized: mpi_trap_generalized.c
	$(CC) $(CFLAGS) -o mpi_trap_generalized mpi_trap_generalized.c $(LDFLAGS)

run: all
	mpiexec -n 4 ./mpi_trap_generalized
	
clean:
	rm -f mpi_trap_generalized