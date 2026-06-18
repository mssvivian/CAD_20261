#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

#define N 10000000 

int main(int argc, char** argv) {
    int my_rank, comm_sz;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    int local_n = N / comm_sz; // Assumindo N divisível por p
    double *local_a = (double*) malloc(local_n * sizeof(double));
    
    double serial_min = 2.0, serial_max = -1.0;
    double *global_a = NULL;

    if (my_rank == 0) {
        global_a = (double*) malloc(N * sizeof(double));
        srand(time(NULL));
        
        // Inicializa o array e acha o min/max serial como gabarito
        serial_min = 1.0;
        serial_max = 0.0;
        for (int i = 0; i < N; i++) {
            global_a[i] = (double)rand() / RAND_MAX;
            if (global_a[i] < serial_min) serial_min = global_a[i];
            if (global_a[i] > serial_max) serial_max = global_a[i];
        }
    }

    // Distribui os blocos para os processos
    MPI_Scatter(global_a, local_n, MPI_DOUBLE, 
                local_a, local_n, MPI_DOUBLE, 
                0, MPI_COMM_WORLD);

    // Encontra o min/max LOCAL do pedaço que cada rank recebeu
    double local_min = 2.0;
    double local_max = -1.0;
    for (int i = 0; i < local_n; i++) {
        if (local_a[i] < local_min) local_min = local_a[i];
        if (local_a[i] > local_max) local_max = local_a[i];
    }

    // Redução para o min/max GLOBAL
    double global_min, global_max;
    MPI_Reduce(&local_min, &global_min, 1, MPI_DOUBLE, MPI_MIN, 0, MPI_COMM_WORLD);
    MPI_Reduce(&local_max, &global_max, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if (my_rank == 0) {
        printf("--- Resultado Serial ---\n");
        printf("Min = %.10f | Max = %.10f\n", serial_min, serial_max);
        printf("--- Resultado MPI ---\n");
        printf("Min = %.10f | Max = %.10f\n", global_min, global_max);
        
        free(global_a);
    }

    free(local_a);
    MPI_Finalize();
    return 0;
}