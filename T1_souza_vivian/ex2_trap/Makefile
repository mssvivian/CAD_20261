CC = mpicc
CFLAGS = -O2 -Wall
LDFLAGS = -lm

all: mpi_trap_generalized

mpi_trap_generalized: mpi_trap_generalized.c
	$(CC) $(CFLAGS) -o mpi_trap_generalized mpi_trap_generalized.c $(LDFLAGS)

run: all
	echo "0 3.141592653589793 10" | mpiexec -n 4 ./mpi_trap_generalized

run_testes: all
	@echo "a. Execucao com p=3, n=10, f(x) = sin(x) em [0, pi]"
	echo "0 3.141592653589793 10" | mpiexec -n 3 ./mpi_trap_generalized
	@echo ""
	@echo "b. Escalabilidade forte: n=10^7"
	echo "0 3.141592653589793 10000000" | mpiexec -n 1 ./mpi_trap_generalized
	echo "0 3.141592653589793 10000000" | mpiexec -n 2 ./mpi_trap_generalized
	echo "0 3.141592653589793 10000000" | mpiexec -n 4 ./mpi_trap_generalized
	echo "0 3.141592653589793 10000000" | mpiexec -n 8 --oversubscribe ./mpi_trap_generalized
	@echo ""
	@echo "c. Convergencia (bonus)"
	echo "0 3.141592653589793 64" | mpiexec -n 4 ./mpi_trap_generalized
	echo "0 3.141592653589793 256" | mpiexec -n 4 ./mpi_trap_generalized
	echo "0 3.141592653589793 1024" | mpiexec -n 4 ./mpi_trap_generalized
	echo "0 3.141592653589793 4096" | mpiexec -n 4 ./mpi_trap_generalized
	echo "0 3.141592653589793 16384" | mpiexec -n 4 ./mpi_trap_generalized
	
clean:
	rm -f mpi_trap_generalized