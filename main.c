#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) {
  int rank, size, n, m, t, *matrix, *vector, *result, *tempbuf;

  MPI_Init(&argc, &argv);

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (size == 1) {
    // do sequential algorithm
  } else {
    if (rank == 0) {
      // master
      printf("what is the number of rows of the matrix: ");
      scanf("%d", &n);

      printf("what is the number of rows of the matrix: ");
      scanf("%d", &m);

      matrix = malloc(sizeof(int) * n * m);
      vector = malloc(sizeof(int) * m);
      result = malloc(sizeof(int) * m);

      for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
          printf("enter the element at matrix[%d, %d]: ", i, j);
          scanf("%d", &t);

          matrix[m * i + j] = t;
        }
      }

      for (int i = 0; i < m; i++) {
        printf("enter the element at vector[%d]: ", i);
        scanf("%d", &t);

        vector[i] = t;
      }

      while (1) {
      }

    } else {
      // slaves
    }
  }

  MPI_Finalize();

  return 0;
}
