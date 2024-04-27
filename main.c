#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  MPI_Init(&argc, &argv);

  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  int n = 4, *matrix, *vector, *local_row, *result;

  if (rank == 0) {
    printf("Enter the size of the matrix/vector (n x n, where 1 < n < 5): ");
    scanf("%d", &n);
    if (n <= 1 || n >= 5) {
      MPI_Abort(MPI_COMM_WORLD, 1);
    }

    matrix = (int *)malloc(n * n * sizeof(int));
    vector = (int *)malloc(n * sizeof(int));
    result = (int *)malloc(n * sizeof(int));

    printf("Enter the elements of the %d x %d matrix:\n", n, n);
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        scanf("%d", &matrix[i * n + j]);
      }
    }

    printf("Enter the elements of the vector of size %d:\n", n);
    for (int i = 0; i < n; i++) {
      scanf("%d", &vector[i]);
    }
  }

  MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

  local_row = (int *)malloc(n * sizeof(int));

  if (rank != 0) {
    vector = (int *)malloc(n * sizeof(int));
  }

  MPI_Bcast(vector, n, MPI_INT, 0, MPI_COMM_WORLD);

  MPI_Scatter(matrix, n, MPI_INT, local_row, n, MPI_INT, 0, MPI_COMM_WORLD);

  int local_sum = 0;
  for (int i = 0; i < n; i++) {
    local_sum += local_row[i] * vector[i];
  }

  MPI_Gather(&local_sum, 1, MPI_INT, result, 1, MPI_INT, 0, MPI_COMM_WORLD);

  if (rank == 0) {
    printf("Result of matrix-vector multiplication is:\n");
    for (int i = 0; i < n; i++) {
      printf("%d\n", result[i]);
    }
    free(matrix);
    free(vector);
    free(result);
  }

  free(local_row);

  MPI_Finalize();
  return 0;
}
