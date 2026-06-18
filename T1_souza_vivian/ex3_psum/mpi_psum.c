#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <mpi.h>

#define N 10000000

int main(int argc, char** argv) {
    int my_rank, comm_sz;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    // Assume que N é divisível por p para simplificar o exercício
    int local_n = N / comm_sz; 
    
    // Todos os processos alocam espaço para o seu pedaço
    double *local_a = (double*) malloc(local_n * sizeof(double));
    double serial_sum = 0.0;
    
    if (my_rank == 0) {
        // Processo 0 aloca o vetor completo e o preenche
        double *a = (double*) malloc(N * sizeof(double));
        srand(time(NULL));
        
        for (int i = 0; i < N; i++) {
            a[i] = ((double)rand() / RAND_MAX); // Valores entre 0 e 1
            serial_sum += a[i]; // Calcula o gabarito
        }
        
        // DISTRIBUIÇÃO: Loop manual de Send (o que o MPI_Scatter faria)
        for (int dest = 1; dest < comm_sz; dest++) {
            MPI_Send(&a[dest * local_n], local_n, MPI_DOUBLE, dest, 0, MPI_COMM_WORLD);
        }
        
        // Processo 0 copia a sua própria parte
        for (int i = 0; i < local_n; i++) {
            local_a[i] = a[i];
        }
        
        free(a); // Já distribuímos, podemos liberar a memória global
    } else {
        // RECEBIMENTO: Trabalhadores recebem seus pedaços
        MPI_Recv(local_a, local_n, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    
    // CÁLCULO LOCAL: Todos os processos somam o seu próprio pedaço
    double local_sum = 0.0;
    for (int i = 0; i < local_n; i++) {
        local_sum += local_a[i];
    }
    
    // COLETA: Loop manual de Send/Recv (o que o MPI_Reduce faria)
    if (my_rank == 0) {
        double global_sum = local_sum;
        for (int src = 1; src < comm_sz; src++) {
            double temp_sum;
            MPI_Recv(&temp_sum, 1, MPI_DOUBLE, src, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            global_sum += temp_sum;
        }
        
        // Verificação do erro relativo
        double error = fabs(global_sum - serial_sum) / fabs(serial_sum);
        printf("Soma Serial = %.10e\n", serial_sum);
        printf("Soma Global = %.10e\n", global_sum);
        printf("Erro Relativo = %.10e\n", error);
        
    } else {
        // Trabalhadores enviam suas somas parciais
        MPI_Send(&local_sum, 1, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);
    }
    
    free(local_a);
    MPI_Finalize();
    return 0;
}