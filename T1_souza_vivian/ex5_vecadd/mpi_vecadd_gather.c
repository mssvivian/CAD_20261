/* mpi_vecadd.c
 * Module 3, Class 6 -- Parallel vector addition with
 * MPI_Scatter / MPI_Gather.
 *
 * Process 0 builds vectors x[N] and y[N], scatters them to all ranks,
 * each rank computes its block of z = x + y, and process 0 gathers the
 * pieces back into a full z[N].
 *
 * Compile: mpicc -O2 -Wall -o mpi_vecadd mpi_vecadd.c
 * Run:     mpiexec -n 4 ./mpi_vecadd
 */

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N 16  /* divisible by every p we plan to run with */

int main(void) {
    int my_rank, comm_sz;
    double *x = NULL, *y = NULL, *z = NULL;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    if (N % comm_sz != 0) {
        if (my_rank == 0)
            fprintf(stderr, "N=%d must be divisible by p=%d\n", N, comm_sz);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    int local_n = N / comm_sz;
    double local_x[local_n], local_y[local_n], local_z[local_n];

    if (my_rank == 0) {
        x = (double *) malloc(N * sizeof(double));
        y = (double *) malloc(N * sizeof(double));
        z = (double *) malloc(N * sizeof(double));
        for (int i = 0; i < N; i++) {
            x[i] = i;
            y[i] = 2.0 * i;
        }
    }

    /* Distribute blocks of x and y. */
    MPI_Scatter(x, local_n, MPI_DOUBLE,
                local_x, local_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Scatter(y, local_n, MPI_DOUBLE,
                local_y, local_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    for (int i = 0; i < local_n; i++)
        local_z[i] = local_x[i] + local_y[i];

    /* Collect the result. */
    MPI_Gather(local_z, local_n, MPI_DOUBLE,
               z, local_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (my_rank == 0) {
        printf("z = x + y:\n");
        for (int i = 0; i < N; i++)
            printf("  z[%2d] = %g\n", i, z[i]);
        free(x); free(y); free(z);
    }

    MPI_Finalize();
    return 0;
}
