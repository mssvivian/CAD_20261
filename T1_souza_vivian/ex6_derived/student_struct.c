#include <stdio.h>
#include <string.h>
#include <stddef.h> // Necessário para o offsetof()
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

    MPI_Datatype student_type;
    int block_lengths[3] = {50, 1, 1};
    MPI_Aint displacements[3];
    MPI_Datatype types[3] = {MPI_CHAR, MPI_DOUBLE, MPI_INT};

    displacements[0] = offsetof(struct Student, name);
    displacements[1] = offsetof(struct Student, grade);
    displacements[2] = offsetof(struct Student, id);

    MPI_Type_create_struct(3, block_lengths, displacements, types, &student_type);
    MPI_Type_commit(&student_type);

    if (my_rank == 0) {
        strcpy(student.name, "Vivian Souza");
        student.grade = 10.0;
        student.id = 123456789;
    }

    MPI_Bcast(&student, 1, student_type, 0, MPI_COMM_WORLD);

    printf("[Struct Bcast] Rank %d recebeu -> Nome: %s | Nota: %.1f | ID: %d\n", 
           my_rank, student.name, student.grade, student.id);

    MPI_Type_free(&student_type);
    MPI_Finalize();
    return 0;
}