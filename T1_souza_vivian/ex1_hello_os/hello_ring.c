#include <stdio.h>
#include <string.h>
#include <mpi.h>

#define MAX_STRING 100

int main(int argc, char** argv) {
    char send_greeting[MAX_STRING];
    char recv_greeting[MAX_STRING];
    int comm_sz, my_rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    // Cada processo faz a sua própria saudação
    snprintf(send_greeting, MAX_STRING, "Greeting from rank %d of %d!", my_rank, comm_sz);

    // Cálculo do vizinho da direita (destino) e da esquerda (origem)
    int dest = (my_rank + 1) % comm_sz;
    int src  = (my_rank - 1 + comm_sz) % comm_sz;

    // Comunicação em Anel usando MPI_Sendrecv para evitar deadlock
    MPI_Sendrecv(send_greeting, strlen(send_greeting) + 1, MPI_CHAR, dest, 0,
                 recv_greeting, MAX_STRING, MPI_CHAR, src, 0,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    // O processo 0 coleta e imprime todas as saudações que circularam
    if (my_rank == 0) {
        printf("Rank 0 recebeu a mensagem: %s (do Rank %d)\n", recv_greeting, src);
        
        for (int i = 1; i < comm_sz; i++) {
            // Recebe a mensagem que os outros ranks receberam em seus respectivos turnos
            MPI_Recv(recv_greeting, MAX_STRING, MPI_CHAR, i, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("Rank 0 coletou do Rank %d a mensagem: %s\n", i, recv_greeting); 
        }
    } else {
        // Trabalhadores enviam a mensagem que receberam no anel diretamente para o processo 0
        MPI_Send(recv_greeting, strlen(recv_greeting) + 1, MPI_CHAR, 0, 1, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}