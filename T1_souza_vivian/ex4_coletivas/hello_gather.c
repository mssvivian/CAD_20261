#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

#define MAX_STRING 100

int main(int argc, char** argv) {
    int my_rank, comm_sz;
    char my_greeting[MAX_STRING];
    char* all_greetings = NULL;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    // Cada processo cria sua própria mensagem
    snprintf(my_greeting, MAX_STRING, "Greetings from process %d of %d!", my_rank, comm_sz);

    // O processo raiz aloca memória para receber todas as mensagens contíguas
    if (my_rank == 0) {
        all_greetings = (char*) malloc(comm_sz * MAX_STRING * sizeof(char));
    }

    // Coleta todas as mensagens no buffer 'all_greetings' do processo 0
    MPI_Gather(my_greeting, MAX_STRING, MPI_CHAR, 
               all_greetings, MAX_STRING, MPI_CHAR, 
               0, MPI_COMM_WORLD);

    // O processo 0 imprime tudo em ordem
    if (my_rank == 0) {
        for (int i = 0; i < comm_sz; i++) {
            // Acessa o offset correto para cada string
            printf("%s\n", &all_greetings[i * MAX_STRING]);
        }
        free(all_greetings);
    }

    MPI_Finalize();
    return 0;
}