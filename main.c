#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

enum MESSAGE_TYPES {
  RECV_CHUNK,
  SEND_CHUNK,
  DONE_CHUNK,
  SEND_CHUNK_NUMBER,
  NO_CHUNK
};

enum CHUNK_STATE { AVAILABLE, DONE };

int main(int argc, char **argv) {
  int rank, size, n, m, t, recv_flag, done_flag,
      chunks = 0, *matrix, *vector, *result, *tempbuf, *chunk, snd_data[2];

  MPI_Init(&argc, &argv);

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (size == 1) {
    // do sequential algorithm
  } else {
    if (rank == 0) {
      printf("what is the number of rows of the matrix: ");
      scanf("%d", &n);

      printf("what is the number of rows of the matrix: ");
      scanf("%d", &m);

      matrix = malloc(sizeof(int) * n * m);
      vector = malloc(sizeof(int) * m);
      result = malloc(sizeof(int) * m);
      chunk = malloc(sizeof(int) * n);

      for (int i = 0; i < n; i++) {
        chunk[i] = AVAILABLE;
      }

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
        for (int i = 1; i < size; i++) {
          MPI_Iprobe(i, RECV_CHUNK, MPI_COMM_WORLD, &recv_flag, NULL);
          MPI_Iprobe(i, DONE_CHUNK, MPI_COMM_WORLD, &done_flag, NULL);

          if (recv_flag) {
            if (chunks < n) {
              MPI_Send(matrix + (chunks * n), m, MPI_INT, i, SEND_CHUNK,
                       MPI_COMM_WORLD);
              MPI_Send(&chunks, 1, MPI_INT, i, SEND_CHUNK_NUMBER,
                       MPI_COMM_WORLD);
              chunks++;
            } else {
              MPI_Send(&recv_flag, 1, MPI_INT, i, NO_CHUNK, MPI_COMM_WORLD);
            }
          }

          if (done_flag) {
            MPI_Recv(snd_data, 2, MPI_INT, i, DONE_CHUNK, MPI_COMM_WORLD, NULL);
            chunk[snd_data[1]] = DONE;
            result[snd_data[1]] = snd_data[0];
          }
        }

        int done = 1;

        for (int i = 0; i < n; i++) {
          if (chunk[i] != DONE) {
            done = 0;
            break;
          }
        }

        if (done) {
          break;
        }
      }

      printf("the result of multiplying the matrix:\n");
      for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
          printf("%d  ", matrix[m * i + j]);
        }
        printf("\n\n");
      }

      printf("with vector:\n");
      for (int i = 0; i < m; i++) {
        printf("%d\n", vector[i]);
      }

      printf("is the vector:\n");
      for (int i = 0; i < n; i++) {
        printf("%d\n", vector[i]);
      }
    } else {
    }
  }

  MPI_Finalize();

  return 0;
}
