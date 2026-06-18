CC = mpicc
CFLAGS = -O2 -Wall
LDFLAGS = -lm

all: struct_bcast three_bcasts

struct_bcast: student_struct.c
	$(CC) $(CFLAGS) -o student_struct student_struct.c $(LDFLAGS)

three_bcasts: student_three_bcasts.c
	$(CC) $(CFLAGS) -o student_three_bcasts student_three_bcasts.c $(LDFLAGS)

run: all
	mpiexec -n 4 ./student_struct
	mpiexec -n 4 ./student_three_bcasts
	
run_struct: struct_bcast
	mpiexec -n 4 ./student_struct

run_three: three_bcasts
	mpiexec -n 4 ./student_three_bcasts

clean:
	rm -f student_struct student_three_bcasts