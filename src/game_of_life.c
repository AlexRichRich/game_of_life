#include <stdio.h>

#include <stdlib.h>

#include <unistd.h>

#include <fcntl.h>

void redraw(const int * m,
  const int * n, int ** pointer_array, int ** pointer_array2);
void map_rendering(const int * m,
  const int * n, int ** pointer_array);
void first_scan(const int * m,
  const int * n, int ** pointer_array, int *status);
void backup(const int * m,
  const int * n, int ** pointer_array, int ** pointer_array2);
int check_neighbours(int x_coor, int y_coor, const int * m, const int * n, int ** pointer_array);

int main() {
  const int m = 25, n = 80;
  int status = 0;
  int ** pointer_array = (int ** ) malloc(m * sizeof(int * ));
  for (int i = 0; i < m; i++)
    pointer_array[i] = (int * ) malloc(n * sizeof(int));
  int ** pointer_array2 = (int ** ) malloc(m * sizeof(int * ));
  for (int i = 0; i < m; i++)
    pointer_array2[i] = (int * ) malloc(n * sizeof(int));
  first_scan(& m, & n, pointer_array, &status);
  stdin = freopen("/dev/tty", "r", stdin);
  system("stty cbreak");
  if (status == 0) {
  redraw(& m, & n, pointer_array, pointer_array2);
  } else {
    printf("Это не так работает, я обиделась\n");
  }
  for (int i = 0; i < m; i++) {
    free(pointer_array[i]);
    free(pointer_array2[i]);
  }
  free(pointer_array);
  free(pointer_array2);
  return 0;
}

void first_scan(const int * m,
  const int * n, int ** pointer_array, int *status) {
  for (int i = 0; i < * m; i++)
    for (int j = 0; j < * n; j++)
      if (scanf("%d", & pointer_array[i][j]) != 1 || pointer_array[i][j] > 1 || pointer_array[i][j] < 0) {
        *status = 1;
      }
}

void redraw(const int * m,
  const int * n, int ** pointer_array, int ** pointer_array2) {
    int speed = 500000;
    while (1) {
    map_rendering(m, n, pointer_array);
    for (int i = 0; i < * m; i++) {
      for (int j = 0; j < * n; j++) {
        int count = check_neighbours(i, j, m, n, pointer_array);
        if (count < 2 && pointer_array[i][j] == 1) {
          pointer_array2[i][j] = 0;
        }
        if (count >= 2 && count <= 3 && pointer_array[i][j] == 1) {
          pointer_array2[i][j] = 1;
        }
        if (count > 3 && pointer_array[i][j] == 1) {
          pointer_array2[i][j] = 0;
        }
        if (count == 3) {
          pointer_array2[i][j] = 1;
        }
      }
    }
    backup(m, n, pointer_array, pointer_array2);
  int old = fcntl(0, F_GETFL);
  fcntl(0, F_SETFL, old | O_NONBLOCK);
  char change;
  change = getchar();
  fcntl(0, F_SETFL, old);
  if (speed <= 1000000)
    if (change == 's') speed += 50000;
  if (speed >= 200000)
    if (change == 'w') {speed -= 50000;}
  if (change == 'x') break;
  usleep(speed);
    }
}

void map_rendering(const int * m,
  const int * n, int ** pointer_array) {
  printf("\e[H\e[2J\e[3J");
  for (int i = 0; i < * m; i++) {
    for (int j = 0; j < * n; j++) {
      if (pointer_array[i][j] == 0) {
        printf(" ");
      } else {
        printf("*");
      }
    }
    printf("\n");
  }
}

int check_neighbours(int x_coor, int y_coor, const int * m, const int * n, int ** pointer_array) {
        int count = 0;
        if (pointer_array[((x_coor - 1) + * m) % * m][(y_coor + * n) % * n] == 1) count++;
        if (pointer_array[((x_coor + 1) + * m) % * m][(y_coor + * n) % * n] == 1) count++;
        if (pointer_array[((x_coor - 1) + * m) % * m][((y_coor - 1) + * n) % * n] == 1) count++;
        if (pointer_array[((x_coor + 1) + * m) % * m][((y_coor + 1) + * n) % * n] == 1) count++;
        if (pointer_array[((x_coor) + * m) % * m][((y_coor + 1) + * n) % * n] == 1) count++;
        if (pointer_array[((x_coor) + * m) % * m][((y_coor - 1) + * n) % * n] == 1) count++;
        if (pointer_array[((x_coor + 1) + * m) % * m][((y_coor - 1) + * n) % * n] == 1) count++;
        if (pointer_array[((x_coor - 1) + * m) % * m][((y_coor + 1) + * n) % * n] == 1) count++;
        return count;
}

void backup(const int * m,
  const int * n, int ** pointer_array, int ** pointer_array2) {
      for (int i = 0; i < * m; i++) {
      for (int j = 0; j < * n; j++) {
        pointer_array[i][j] = pointer_array2[i][j];
      }
    }
}
