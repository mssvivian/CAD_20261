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

    z = (double *) malloc(N * sizeof(double));

    if (my_rank == 0) {
        x = (double *) malloc(N * sizeof(double));
        y = (double *) malloc(N * sizeof(double));
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
    MPI_Allgather(local_z, local_n, MPI_DOUBLE,
                   z, local_n, MPI_DOUBLE, MPI_COMM_WORLD);

    if (my_rank == comm_sz - 1) {
        printf("Rank %d confirmando o recebimento completo. z = x + y:\n", my_rank);
        for (int i = 0; i < N; i++)
            printf("  z[%2d] = %g\n", i, z[i]);
    }

    if (my_rank == 0) {
        free(x); 
        free(y); 
    }
    free(z);

    MPI_Finalize();
    return 0;
}
