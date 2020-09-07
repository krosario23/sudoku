#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>

#define GRIDSIZE 9

typedef struct cell_t {
     int value;
     int x;
     int y;
} Cell;

typedef struct board_t {
     Cell grid[GRIDSIZE][GRIDSIZE];
} Board;

Board new_board();
Cell  new_cell();


void  init_cell(Cell* c, int x, int y, int val);
void  load_board(Board* b, int arr[GRIDSIZE][GRIDSIZE]);
void  print_board(Board* b);
bool  check_row(Board* b, int r, int val);
bool  check_column(Board* b, int c, int val);
bool  check_square(Board* b, int x, int y, int val);
bool  try_cell(Board* b, int x, int y, int val);
bool  solve(Board* b, int x, int y);
void  seed_board(Board* b);
bool  fill_board(Board* b);
