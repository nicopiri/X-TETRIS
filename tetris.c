
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
void multi_player() {
  int tetromino_type, tetromino_rotation, tetromino_type2, tetromino_rotation2;
  int tetrominos_left[7];
  int Blocks[7][4][4][4];
  int virtual_grid[24][22];
  int virtual_grid2[24][22];
  int multigrid[24][22];
  int result;
  int result2;
  int score = 0;
  int score2 = 0;
  int multicount = 0;
  tetromino_type = tetromino_rotation = tetromino_type2 = tetromino_rotation2 =
      0;
  for (int j = 0; j < 7; j++) {
    tetrominos_left[j] = 40;
  }
  for (int i = 0; i < 24; i++) {
    for (int j = 0; j < 22; j++) {
      virtual_grid[i][j] = virtual_grid2[i][j] = -1;
      multigrid[i][j] = -1;
    }
  }
  system("mode con cols=120 lines=30");
  hide_cursor();
  define_array(Blocks);
  draw_map(0);
  draw_map(60);

  gotoxy(86, 16);
  printf("S C O R E  : %d", score2);
  print_score(&score);

  while (1) {
    result = moving_tetromino(Blocks, virtual_grid, tetromino_type,
                              tetromino_rotation, &score, tetrominos_left,
                              0);
    if (result == -1) break;
    score_line(virtual_grid, &score, 0);
    draw_map(0);
    print_score(&score);

    result2 = moving_tetromino(Blocks, virtual_grid2, tetromino_type2,
                               tetromino_rotation2, &score2, tetrominos_left,
                               60);
    if (result2 == -1) break;

    for (int i = 0; i < 24; i++) {
      for (int j = 0; j < 22; j++) {
        if (j == 2 || j == 4)
          multigrid[i][j] = virtual_grid2[i + 2][j + 16];
        else if (j == 6 || j == 8 || j == 10 || j == 12 || j == 14 || j == 16 ||
                 j == 18 || j == 20)
          multigrid[i][j] = virtual_grid2[i + 3][j - 6];
      }
    }
    score_line(multigrid, &score2, 60, &multicount);
    draw_map(60);
    gotoxy(86, 16);
    printf("S C O R E 2: %d", score2);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                            9 + tetromino_type);
    gotoxy(94, 11);
    if (tetrominos_left[tetromino_type2] > 0)
      printf("%d                   ", tetrominos_left[tetromino_type2]);
    else
      printf("0, change block");
  }
}

void single_player() {
  int tetromino_type, tetromino_rotation;
  int tetrominos_left[7];
  int Blocks[7][4][4][4];
  int virtual_grid[24][22];
  int result;
  int score = 0;
  tetromino_type = tetromino_rotation = 0;
  for (int j = 0; j < 7; j++) {
    tetrominos_left[j] = 20;
  }

  for (int i = 0; i < 24; i++)
  {
    for (int j = 0; j < 22; j++) {
      virtual_grid[i][j] = -1;
    }
  }

  system("mode con cols=120 lines=30");
  hide_cursor();
  define_array(Blocks);
  draw_map(0);

  print_score(&score);

  while (1) {
    result = moving_tetromino(Blocks, virtual_grid, tetromino_type,
                              tetromino_rotation, &score, tetrominos_left,
                              0);
    if (result == -1) break;
    int copyscore= score;
    score_line(virtual_grid, &score, 0);

    draw_map(0);
    print_score(&score);

}
}

int moving_tetromino(int array[7][4][4][4], int virtual_grid[24][22],
                     int tetromino_type, int tetromino_rotation, int* score,
                     int tetrominos_left[6], int xplus) {
  time_t clock1, clock2;

  int result;
  int x = 8 + xplus, y = 1, can_spawn;
  gotoxy(8, 1);

  int z = 0;
  do {
    for (int i = 0; i <= 3; i++) {
      for (int j = 0; j <= 3; j++) {
        gotoxy(xplus + 28 + 2 * j, 6 + i);
        printf(" ");
      }
    }
    for (int i = 0; i <= 3; i++) {
      for (int j = 0; j <= 3; j++) {
        gotoxy(xplus + 28 + 2 * j, 6 + i);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                                9 + tetromino_type);
        if (array[tetromino_type][0][i][j] == 1)
          printf("%c", 254);
      }
    }
    gotoxy(34 + xplus, 11);
    if (tetrominos_left[tetromino_type] > 0)
      printf("%d                   ", tetrominos_left[tetromino_type]);
    else
      printf("0, change block");

    switch (getKey()) {
      case 32:
        z = -1;
      case 77:
        if (tetromino_type < 6)
          tetromino_type++;
        else
          tetromino_type = 0;
    }

  } while (z == 0);

  if (tetromino_type > 0)
    tetromino_type--;
  else if (tetromino_type == 0)
    tetromino_type = 6;
  tetrominos_left[tetromino_type]--;

  if (descend_feedback(array, virtual_grid, tetromino_type, tetromino_rotation,
                       x, y) == 1)
    can_spawn = 1;
  else
    can_spawn = -1;

  if (can_spawn != 1)
  {
    spawn_tetromino(array[7][4][4][4], x, y, tetromino_rotation,
                    tetromino_type);
    return -1;
  }

  clock1 = clock();
  clock2 = clock();
  if (tetrominos_left[tetromino_type] <= -1) return 1;
  do {
    clock2 = clock();
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        gotoxy(x + j * 2, y + i);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                                9 + tetromino_type);
        if (array[tetromino_type][tetromino_rotation][i][j] == 1)
          printf("%c", 254);
      }
    }
    switch (getKey()) {
      case 32:
        for (int i = 0; i < 4; i++) {
          for (int j = 0; j < 4; j++) {
            gotoxy(x + j * 2, y + i);
            if (array[tetromino_type][tetromino_rotation][i][j] == 1)
              printf("  ");
          }
        }
        if (tetromino_rotation == 0)
          tetromino_rotation = 3;
        else
          tetromino_rotation -= 1;

        result = rotation_feedback(array, virtual_grid, tetromino_type,
                                   tetromino_rotation, x, y);
        if (result == -1)
          tetromino_rotation += 1;
        else if (result == 1) {
        } else
          x = result;

        break;

      case 80:
        for (int i = 0; i < 4; i++) {
          for (int j = 0; j < 4; j++) {
            gotoxy(x + j * 2, y + i);
            if (array[tetromino_type][tetromino_rotation][i][j] == 1)
              printf("  ");
          }
        }
        gotoxy(x, y);
        y++;
        clock1 = clock();
        break;

      case 75:
        for (int i = 0; i < 4; i++) {
          for (int j = 0; j < 4; j++) {
            gotoxy(x + j * 2, y + i);
            if (array[tetromino_type][tetromino_rotation][i][j] == 1)
              printf("  ");
          }
        }
        x -= 2;
        if (sideways_feedback(array, virtual_grid, tetromino_type,
                              tetromino_rotation, x, y, xplus) == 1) {
          break;
        } else {
          x += 2;
          break;
        }

      case 77:
        for (int i = 0; i < 4; i++) {
          for (int j = 0; j < 4; j++) {
            gotoxy(x + j * 2, y + i);
            if (array[tetromino_type][tetromino_rotation][i][j] == 1)
              printf("  ");
          }
        }
        x += 2;
        if (sideways_feedback(array, virtual_grid, tetromino_type,
                              tetromino_rotation, x, y, xplus) == 1) {
          break;
        } else {
          x -= 2;
          break;
        }
    }

    if (clock2 - clock1 > 500) {
      for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
          gotoxy(x + j * 2, y + i);
          if (array[tetromino_type][tetromino_rotation][i][j] == 1)
            printf("  ");
        }
      }
      y++;
      clock1 = clock();
    }
  } while (descend_feedback(array, virtual_grid, tetromino_type,
                            tetromino_rotation, x,
                            y) == 1);

  y--;
  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++) {
      gotoxy(x + j * 2, y + i);
      if (array[tetromino_type][tetromino_rotation][i][j] == 1) {
        virtual_grid[y + i][x + j * 2] = tetromino_type;
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                                9 + tetromino_type);
        printf("%c", 254);
      }
    }
  }
  return 1;
}

void spawn_tetromino(int array[7][4][4][4], int x, int y,
                     int tetromino_rotation, int tetromino_type) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      gotoxy(x + j * 2, y + i);
      SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                              9 + tetromino_type);
      if (array[tetromino_type][tetromino_rotation][i][j] == 1)
        printf("%c", 254);
    }
  }
}

int getKey() {
  if (kbhit()) return getch();
}

void draw_map(int x) {
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

  for (int i = 0; i < 23; i++) {
    if (i < 15) {
      gotoxy(0 + x, 1 + i);
      printf("%c", 254);
      gotoxy(22 + x, 1 + i);
      printf("%c", 254);
    }

    if (i % 2 == 0) {
      gotoxy(i + x, 0);
      printf("%c", 254);
      gotoxy(i + x, 16);
      printf("%c", 254);
    }
  }

  for (int i = 1; i < 7; i++) {
    gotoxy(26 + x, 4 + i);
    printf("%c", 254);
    if (i == 1 || i == 6) {
      for (int j = 28; j < 35; j += 2) {
        gotoxy(j + x, 4 + i);
        printf("%c", 254);
      }
    }
    gotoxy(36 + x, 4 + i);
    printf("%c", 254);
  }

  gotoxy(27 + x, 4);
  printf("N  E  X  T");

  gotoxy(26 + x, 11);
  printf("BLOCKS:");
}

void define_array(int array[7][4][4][4]) {
  int copy_array[7][4][4][4] = {
      {{{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},
       {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},
       {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},
       {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}}},

      {{{0, 0, 0, 0}, {0, 1, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}},
       {{0, 1, 0, 0}, {0, 1, 1, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}},
       {{0, 0, 0, 0}, {0, 1, 1, 1}, {0, 0, 1, 0}, {0, 0, 0, 0}},
       {{0, 0, 0, 0}, {0, 0, 1, 0}, {0, 1, 1, 0}, {0, 0, 1, 0}}},

      {{{0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}},
       {{0, 0, 0, 0}, {0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}},
       {{0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}},
       {{0, 0, 0, 0}, {0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}}},

      {
          {{0, 0, 0, 0}, {1, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},
          {{0, 0, 1, 0}, {0, 1, 1, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}},
          {{0, 0, 0, 0}, {1, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},
          {{0, 0, 1, 0}, {0, 1, 1, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}},
      },

      {{{0, 0, 0, 0}, {0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}},
       {{0, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 0, 0}},
       {{0, 0, 0, 0}, {0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}},
       {{0, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 0, 0}}},

      {{{0, 0, 0, 0}, {0, 1, 1, 1}, {0, 1, 0, 0}, {0, 0, 0, 0}},
       {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}},
       {{0, 0, 0, 0}, {0, 0, 1, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}},
       {{0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}}},

      {{{0, 0, 0, 0}, {1, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 0, 0}},
       {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}},
       {{0, 0, 1, 0}, {0, 0, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},
       {{0, 0, 0, 0}, {0, 1, 0, 0}, {0, 1, 1, 1}, {0, 0, 0, 0}}}};

  for (int i = 0; i < 7; i++) {
    for (int j = 0; j < 4; j++) {
      for (int k = 0; k < 4; k++) {
        for (int x = 0; x < 4; x++) {
          array[i][j][k][x] = copy_array[i][j][k][x];
        }
      }
    }
  }
}

int descend_feedback(int array[7][4][4][4], int virtual_grid[24][22],
                     int tetromino_type, int tetromino_rotation, int x,
                     int y) {
  int height;
  int i = 3;
  while (i >= 0) {
    for (int j = 3; j >= 0; j--) {
      if (array[tetromino_type][tetromino_rotation][i][j] == 1) {
        height = i;
        break;
      }
    }
    if (height == i) break;

    i--;
  }

  if (y + height == 16) {
    return 0;
  }
  int k = 0;
  while (k < 4) {
    for (int j = 0; j < 4; j++) {
      if (array[tetromino_type][tetromino_rotation][k][j] == 1 &&
          virtual_grid[y + k][x + j * 2] !=-1)
        return 0;
    }
    k++;
  }

  return 1;
}

int sideways_feedback(int array[7][4][4][4], int virtual_grid[24][22],
                      int tetromino_type, int tetromino_rotation, int x, int y,
                      int xplus) {
  for (int i = 0; i < 4; i++) {
    if (array[tetromino_type][tetromino_rotation][i][0] == 1 && x == 0 + xplus)
      return -1;
    if (array[tetromino_type][tetromino_rotation][i][1] == 1 && x == -2 + xplus)
      return -2;
    if (array[tetromino_type][tetromino_rotation][i][3] == 1 && x == 16 + xplus)
      return -3;
    if (array[tetromino_type][tetromino_rotation][i][2] == 1 && x == 18 + xplus)
      return -4;
    if (array[tetromino_type][tetromino_rotation][i][1] == 1 && x == 20 + xplus)
      return -5;
    for (int j = 0; j < 4; j++) {
      if (array[tetromino_type][tetromino_rotation][i][j] == 1 &&
          virtual_grid[y + i][x + j * 2] != -1)
        return -6;
    }
  }

  return 1;
}

int rotation_feedback(int array[7][4][4][4], int virtual_grid[24][22],
                      int tetromino_type, int tetromino_rotation, int x, int y,
                      int xplus) {

  switch (sideways_feedback(array, virtual_grid, tetromino_type,
                            tetromino_rotation, x, y, xplus)) {
    case -1:
      return xplus + 2;
    case -2:
      return xplus + 2;
    case -3:
      return xplus - 2;
    case -4:
      return xplus - 4;
    case -5:
      return xplus - 4;
    case -6:
      return -1;
  }
  return 1;
}

void score_line(int virtual_grid[24][22], int* score, int xplus,
               int* multicount) {
  int count = 0;

  for (int i = 23; i > 3; i--)
  {
    if (virtual_grid[i][2] != -1 && virtual_grid[i][4] != -1 &&
        virtual_grid[i][6] != -1 && virtual_grid[i][8] != -1 &&
        virtual_grid[i][10] != -1 && virtual_grid[i][12] != -1 &&
        virtual_grid[i][14] != -1 && virtual_grid[i][16] != -1 &&
        virtual_grid[i][18] != -1 &&
        virtual_grid[i][20] != -1)
    {
      count++;
      for (int a = i; a > 3; a--)
      {
        for (int b = 2; b < 22;
             b++)
        {
          gotoxy(b + xplus, a);
          printf("  ");
          virtual_grid[a][b] = virtual_grid[a - 1][b];
          virtual_grid[a - 1][b] = -1;
        }
      }
      i++;
    }
  }

  int count2 = count;
  if (xplus != 0) count = count - *multicount;

  if (count == 1)
    *score += 1;
  else if (count == 2)
    *score += 3;
  else if (count == 3)
    *score += 6;
  else if (count == 4)
    *score += 12;

  *multicount = count2;

  for (int i = 3; i < 23; i++) {
    for (int j = 0; j < 22; j += 2) {
      gotoxy(j + xplus, i);
      if (virtual_grid[i][j] != -1) {
        SetConsoleTextAttribute(
            GetStdHandle(STD_OUTPUT_HANDLE),
            9 + virtual_grid[i][j]);
        printf("%c", 254);
      }
    }
  }
}

void print_score(int* score) {
  gotoxy(26, 16);
  printf("S C O R E :%d", *score);
}

void gotoxy(int x, int y) {
  COORD CursorPos = {x, y};
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), CursorPos);
}

void hide_cursor(void) {
  HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_CURSOR_INFO info;
  info.dwSize = 100;
  info.bVisible = FALSE;
  SetConsoleCursorInfo(consoleHandle, &info);
}

void main(void) {
  system("mode con cols=120 lines=30");
  hide_cursor();
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
  gotoxy(1, 1);
  printf("===============================================");
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
  gotoxy(1, 2);
  printf("X   X     TTTTT  EEEEE  TTTTT  RRR    I    SS  ");
  gotoxy(1, 3);
  printf(" X X        T    E        T    R  R   I   S  S ");
  gotoxy(1, 4);
  printf("  X   ==    T    EEEE     T    R R    I    S   ");
  gotoxy(1, 5);
  printf(" X X        T    E        T    R  R   I   S  S ");
  gotoxy(1, 6);
  printf("X   X       T    EEEEE    T    R   R  I    SS  ");
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
  gotoxy(1, 7);
  printf("===============================================");

  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
  gotoxy(1, 10);
  printf("HOLD S FOR SINGLEPLAYER MODE");
  gotoxy(1, 12);
  printf("HOLD M FOR MULTIPLAYER MODE");
  gotoxy(1, 14);
  printf("HOLD E TO EXIT THE GAME");

  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
  gotoxy(40, 10);
  printf("RULES:");
  gotoxy(40, 12);
  printf("When the tetromino is blinking it's your turn");
  gotoxy(40, 14);
  printf("SPACEBAR to spawn and rotate");
  gotoxy(40, 16);
  printf("ARROWKEYS to move and change the type ");

  while (1) {
    if (getKey() == 's' || getKey() == 'S')
      single_player();
    else if (getKey() == 'm' || getKey() == 'M')
      multi_player();
    else if (getKey() == 'e' || getKey() == 'E')
      return;
  }

  return;
}
