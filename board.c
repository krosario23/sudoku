#include "board.h"

//cell constructtor
void init_cell(Cell* c, int x, int y, int val) {
     c->x = x;
     c->y = y;
     c->value = val;
}

//returns cell at given position
static Cell* peek(Board* b, int x, int y) {
     return &b->grid[x][y];
}

static void set_value(Cell* c, int val) {
     c->value = val;
}

static int get_value(Cell* c) {
     return c->value;
}

//initializes every cell to 0 (empty)
Board new_board() {
     Board b;

     for (size_t i = 0; i < GRIDSIZE; i++) {
          for (size_t j = 0; j < GRIDSIZE; j++) {
               init_cell(peek(&b, i, j), i, j, 0);
          }
     }

     return b;
}

//takes a 2D array and copies sudoku grid
void load_board(Board* b, int arr[GRIDSIZE][GRIDSIZE]) {
     for (size_t i = 0; i < GRIDSIZE; i++) {
          for (size_t j = 0; j < GRIDSIZE; j++) {
               init_cell(peek(b, i, j), i, j, arr[i][j]);
          }
     }
}

void print_board(Board* b) {
     printf("   1  2  3   4  5  6   7  8  9  \n");
     for (size_t i = 0; i < GRIDSIZE; i++) {
          if (i % 3 == 0) printf(" +---------+---------+---------+\n");
          printf("%c", (char)i + 65);
          for (size_t j = 0; j < GRIDSIZE; j++) {
               if (j % 3 == 0) printf("|");
               if (!(get_value(peek(b, i, j)) == 0)) {
                    printf(" %i ", get_value(peek(b, i, j)));
               } else {
                    printf("   ");
               }
          }
          printf("|\n");
     }
     printf(" +---------+---------+---------+\n");
}

//returns true if value given can be found in given row, false otherwise
bool check_row(Board* b, int r, int val) {
     for (size_t i = 0; i < GRIDSIZE; i++) {
          if(get_value(peek(b, r, i)) == val) return true;
     }
     return false;
}

//returns true if value given can be found in given column, false otherwise
bool check_column(Board* b, int c, int val) {
     for (size_t i = 0; i < GRIDSIZE; i++) {
          if (get_value(peek(b, i, c)) == val) return true;
     }
     return false;
}

/* this function resets the current (x,y) position back to
   the first cell of the current square */
static void realign(int* x, int* y) {
     *x = *x - *x % 3;
     *y = *y - *y % 3;
}

//returns true if value given can be found in given square, false otherwise
bool check_square(Board* b, int x, int y, int val) {
     //printf("(%d,%d)", x, y);
     realign(&x, &y);
     //printf(" -> (%d, %d)\n", x ,y);
     for (int i = x; i < x + 3; i++) {
          for (int j = y; j < y + 3; j++) {
                //printf("(%d,%d)", i, j);
                if(get_value(peek(b, i, j)) == val) return true;
          }
     }
     return false;
}

//returns true if value given can be placed at given position, false otherwise
bool try_cell(Board* b, int x, int y, int val) {
     return !check_row(b, x, val) &&
            !check_column(b, y, val) &&
            !check_square(b, x, y, val);
}

bool find_empty(Board* b, int* x, int* y) {
     for (int i = *x; i < GRIDSIZE; i++) {
          for (int j = *y; j < GRIDSIZE; j++) {
               if (get_value(peek(b, i, j)) == 0) {
                    *x = i;
                    *y = j;
                    return true;
               }
          }
     }

     for (int i = 0; i <= *x; i++) {
          for (int j = 0; j <= *y; j++) {
               if (get_value(peek(b, i, j)) == 0) {
                    *x = i;
                    *y = j;
                    return true;
               }
          }
     }

     return false;
}

//backtracking algorithm which solves a given board
bool solve(Board* b, int x, int y) {
     //print_board(b);
     //getchar();

     //if there are no empty cells, we are done
     if (!find_empty(b, &x, &y)) return true;

     //otherwise, try placing digits 1-9 in the empty cell
     for (int n = 1; n <= GRIDSIZE; n++) {
          //if we found a digit that's valid in the current spot
          if (try_cell(b, x, y, n)) {
               //set the cell to n for now
               set_value(peek(b, x, y), n);

               //recursively call solve to deal with the rest of the board
               if (solve(b, x, y)) return true;

               //if solve failed, we set this cell back to empty and try the next digit
               set_value(peek(b, x, y), 0);
          }
     }

     /* we tried every number from 1-9 in the mpty spot and none were valid
        the board is unsolvable, time to backtrack */
     return false;
}

static void swap(Cell* a, Cell* b) {
     Cell temp = *a;
     *a = *b;
     *b = temp;
}

/* this function fills the first row with digits 1-9 and shuffles
   we call this when we are filling an empty board so that our first row
   is more random than just the digits in order */
void seed_board(Board* b) {
     for (int i = 0; i < GRIDSIZE; i++) {
          set_value(peek(b, 0, i), i + 1);
     }

     for (int j = 0; j < GRIDSIZE;  j++) {
          swap(peek(b, 0, rand() % GRIDSIZE), peek(b, 0, rand() % GRIDSIZE));
     }
}

//checks if board is empty
static bool is_empty(Board* b) {
     for (int i = 0; i < GRIDSIZE; i++) {
          for(int j = 0; j < GRIDSIZE; j++) {
               if (get_value(peek(b, i, j)) != 0) {
                    return false;
               }
          }
     }
     return true;
}

//takes and empty board and attempts to fill with a valid sudoku solution
bool fill_board(Board* b) {
     assert(is_empty(b));
     seed_board(b);
     print_board(b);
     int count = 0;
     if(solve(b, rand() % GRIDSIZE, rand() % GRIDSIZE)) return true;

     printf("failed to fill given board\n");
     return false;
}
