#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>

#define GRIDSIZE 9
#define CELLCOUNT GRIDSIZE * GRIDSIZE

typedef struct board_t {
     int grid[GRIDSIZE][GRIDSIZE];
     int puzzle[GRIDSIZE * GRIDSIZE];
     int solution[GRIDSIZE * GRIDSIZE];
} Board;

Board new_board();

int*   peek(Board* b, int x, int y);
void   set_value(Board* b, int x, int y, int val);
int    get_value(Board* b, int x, int y);
bool   load_board(Board* b, const char* arr);
bool   is_empty(Board* b, int x, int y);
int    count_filled(Board* b);
void   to_string(Board* b);
void   print_board(Board* b);
void   show_solution(Board* b);
bool   check_row(Board* b, int r, int val);
bool   check_column(Board* b, int c, int val);
bool   check_square(Board* b, int x, int y, int val);
bool   try_cell(Board* b, int x, int y, int val);
bool   solve(Board* b, int x, int y);
void   seed_board(Board* b);
bool   fill_board(Board* b);
Board  gen_board();
void   list_boards(Board* b, int count);
bool   remove_board(Board* list, int count, int idx);
Board  choose_board(Board* list, int count);
bool   guess(Board* b, int pos, int val);
