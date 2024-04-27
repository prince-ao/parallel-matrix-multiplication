parallel matrix multiplication using mpi
---

How to run?
---
mpicc main.c -o main
mpirun -np 8 ./main < same_input.txt
