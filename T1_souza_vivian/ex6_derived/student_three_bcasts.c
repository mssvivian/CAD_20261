#include <stdio.h>
#include <string.h>
#include <mpi.h>

struct Student { 
    char name[50]; 
    double grade; 
    int id; 
};

int main(int argc, char** argv) {
    int my_rank, comm_sz;
    struct Student student;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    if (my_rank == 0) {
        strcpy(student.name, "Vivian Souza");
        student.grade = 10.0;
        student.id = 123456789;
    }

    // TRÊS chamadas separadas de Bcast!
    MPI_Bcast(student.name, 50, MPI_CHAR, 0, MPI_COMM_WORLD);
    MPI_Bcast(&student.grade, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&student.id, 1, MPI_INT, 0, MPI_COMM_WORLD);

    printf("[3 Bcasts] Rank %d recebeu -> Nome: %s | Nota: %.1f | ID: %d\n", 
           my_rank, student.name, student.grade, student.id);

    MPI_Finalize();
    return 0;
}