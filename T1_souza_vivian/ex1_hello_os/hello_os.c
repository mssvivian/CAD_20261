#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sched.h>
#include <mpi.h>

#define MAX_STRING 100

int main(int argc, char** argv) {
    char greeting[MAX_STRING];
    int comm_sz, my_rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    // Introspecção do SO
    int pid = getpid();
    int cpu = sched_getcpu();

    if (my_rank != 0) {
        // Trabalhadores formatam a string e enviam ao rank 0
        snprintf(greeting, MAX_STRING, "Hello from rank %d/%d -- PID = %d, CPU = %d", 
                 my_rank, comm_sz, pid, cpu);
        MPI_Send(greeting, strlen(greeting) + 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    } else {
        // Rank 0 imprime sua própria mensagem primeiro
        printf("Hello from rank %d/%d -- PID = %d, CPU = %d\n", my_rank, comm_sz, pid, cpu);
        
        // Em seguida, recebe e imprime as mensagens dos trabalhadores em ordem
        for (int q = 1; q < comm_sz; q++) {
            MPI_Recv(greeting, MAX_STRING, MPI_CHAR, q, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("%s\n", greeting);
        }
    }

    MPI_Finalize();
    return 0;
}