#include "board.h"

//returns cell at given position
int* peek(Board* b, int x, int y) {
     return &b->grid[x][y];
}

void set_value(Board* b, int x, int y, int val) {
     b->grid[x][y] = val;
     b->puzzle[x * GRIDSIZE + y] = val;
}

int get_value(Board* b, int x, int y) {
     return b->grid[x][y];
}

//initializes every cell to 0 (empty)
Board new_board() {
     Board b;
     for (size_t i = 0; i < GRIDSIZE; i++) {
          for (size_t j = 0; j < GRIDSIZE; j++) {
               set_value(&b, i, j, 0);
               b.solution[i * GRIDSIZE + j] = 0;
          }
     }

     return b;
}

//takes a 2D array and copies sudoku grid
bool load_board(Board* b, const char* arr) {
     for (size_t i = 0; i < GRIDSIZE; i++) {
          for (size_t j = 0; j < GRIDSIZE; j++) {
               if (arr[i * GRIDSIZE + j] < '0' || arr[i * GRIDSIZE + j] > '9') {
                    return false;
               }
               set_value(b, i, j, arr[i * GRIDSIZE + j]);
          }
     }

     for (int i = 0; i < CELLCOUNT; i++) {
          b->solution[i] = arr[i];
     }

     return true;
}

bool is_empty(Board* b, int x, int y) {
     return get_value(b, x, y) == 0;
}

int count_filled(Board* b) {
     int count = 0;
     for (int i = 0; i < GRIDSIZE; i++) {
          for (int j = 0; j < GRIDSIZE; j++) {
               if (!is_empty(b, i , j)) {
                    count++;
               }
          }
     }
     return count;
}

void to_string(Board* b) {
     printf("puzzle: ");
     for (int i = 0; i < GRIDSIZE; i++) {
          for (int j = 0; j < GRIDSIZE; j++) {
               printf("%d", b->puzzle[i * GRIDSIZE + j]);
          }
     }
     printf("\nsolution: ");
     for (int i = 0; i < GRIDSIZE; i++) {
          for (int j = 0; j < GRIDSIZE; j++) {
               printf("%d", b->solution[i * GRIDSIZE + j]);
          }
     }
}

void print_board(Board* b) {
     system("clear");
     printf("   1  2  3   4  5  6   7  8  9  \n");
     for (size_t i = 0; i < GRIDSIZE; i++) {
          if (i % 3 == 0) printf(" +---------+---------+---------+\n");
          printf("%c", (char)i + 65);
          for (size_t j = 0; j < GRIDSIZE; j++) {
               if (j % 3 == 0) printf("|");
               if (!is_empty(b, i, j)) {
                    printf(" %i ", get_value(b, i, j));
               } else {
                    printf("   ");
               }
          }
          printf("|\n");
     }
     printf(" +---------+---------+---------+\n");
     //printf("filled: %d\n", count_filled(b));
}

void show_solution(Board* b) {
     printf("   1  2  3   4  5  6   7  8  9  \n");
     for (size_t i = 0; i < GRIDSIZE; i++) {
          if (i % 3 == 0) printf(" +---------+---------+---------+\n");
          printf("%c", (char)i + 65);
          for (size_t j = 0; j < GRIDSIZE; j++) {
               if (j % 3 == 0) printf("|");
               if (b->solution[i * GRIDSIZE + j] != 0) {
                    printf(" %i ", b->solution[i * GRIDSIZE + j]);
               } else {
                    printf("   ");
               }
          }
          printf("|\n");
     }
     printf(" +---------+---------+---------+\n");
     printf("filled: %d\n", count_filled(b));
}

//returns true if value given can be found in given row, false otherwise
bool check_row(Board* b, int r, int val) {
     for (size_t i = 0; i < GRIDSIZE; i++) {
          if(get_value(b, r, i) == val) return true;
     }
     return false;
}

//returns true if value given can be found in given column, false otherwise
bool check_column(Board* b, int c, int val) {
     for (size_t i = 0; i < GRIDSIZE; i++) {
          if (get_value(b, i, c) == val) return true;
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
                if(get_value(b, i, j) == val) return true;
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
     for (int i = 0; i <= *x; i++) {
          for (int j = 0; j <= *y; j++) {
               if (is_empty(b, i, j)) {
                    *x = i;
                    *y = j;
                    return true;
               }
          }
     }

     for (int i = *x; i < GRIDSIZE; i++) {
          for (int j = *y; j < GRIDSIZE; j++) {
               if (is_empty(b, i, j)) {
                    *x = i;
                    *y = j;
                    return true;
               }
          }
     }

     return false;
}

static int rand_int(int max) {
     return rand() % max;
}

static void swap(int* x, int* y) {
     int temp = *x;
     *x = *y;
     *y = temp;
}

static void randomize(int *arr, int size) {
     for (int i = 0; i < size; i++) {
          arr[i] = i + 1;
     }

     for (int x = 0; x < size; x++) {
          swap(&arr[x], &arr[rand_int(GRIDSIZE)]);
     }
}

//backtracking algorithm which solves a given board
bool solve(Board* b, int x, int y) {
     //print_board(b);
     //getchar();

     //if there are no empty cells, we are done
     if (!find_empty(b, &x, &y)) return true;

     int digits[GRIDSIZE] = {0};
     //shuffle digits 1-GRIDSIZE
     randomize(digits, GRIDSIZE);

     //otherwise, try placing digits 1-GRIDSIZE in the empty cell
     for (int n = 0; n < GRIDSIZE; n++) {
          //if we found a digit that's valid in the current spot
          if (try_cell(b, x, y, digits[n])) {
               //set the cell to n for now
               set_value(b, x, y, digits[n]);

               //recursively call solve to deal with the rest of the board
               if (solve(b, x, y)) return true;

               //if solve failed, we set this cell back to empty and try the next digit
               set_value(b, x, y, 0);
          }
     }

     /* we tried every number from 1-GRIDSIZE in the mpty spot and none were valid
        the board is unsolvable, time to backtrack */
     return false;
}
/* this function fills the first row with digits 1-GRIDSIZE and shuffles
   we call this when we are filling an empty board so that our first row
   is more random than just the digits in order */
void seed_board(Board* b) {
     for (int i = 0; i < GRIDSIZE; i++) {
          set_value(b, 0, i, i + 1);
     }

     for (int j = 0; j < GRIDSIZE;  j++) {
          swap(peek(b, 0, rand_int(GRIDSIZE)), peek(b, 0, rand_int(GRIDSIZE)));
     }
}

//checks if board is empty
static bool board_empty(Board* b) {
     for (int i = 0; i < GRIDSIZE; i++) {
          for(int j = 0; j < GRIDSIZE; j++) {
               if (!is_empty(b, i, j)) {
                    return false;
               }
          }
     }
     return true;
}

//takes and empty board and attempts to fill with a valid sudoku solution
bool fill_board(Board* b) {
     assert(board_empty(b));
     if(solve(b, rand_int(GRIDSIZE), rand_int(GRIDSIZE))) return true;

     printf("failed to fill given board\n");
     return false;
}

static int count_solutions(Board* b, int pos, int count) {
     if (pos == 81) return 1 + count;

     if(b->puzzle[pos] != 0) {
          return count_solutions(b, pos + 1, count);
     } else {
          for (int val = 1; val <= GRIDSIZE && count < 2; ++val) {
               if (try_cell(b, pos / GRIDSIZE, pos % GRIDSIZE, val)) {
                    set_value(b, pos / GRIDSIZE, pos % GRIDSIZE, val);
                    count = count_solutions(b, pos + 1, count);
               }
          }
          set_value(b, pos / GRIDSIZE, pos % GRIDSIZE, 0);
          return count;
     }
}

static void remove_digits(Board* b) {
     int pos, val;
     int temp[CELLCOUNT] = {0};

     for (int i = 0; i < CELLCOUNT * GRIDSIZE; i++) {
          pos = rand_int(CELLCOUNT);
          int x = pos / GRIDSIZE;
          int y = pos % GRIDSIZE;
          val = get_value(b, x, y);
          if (val != 0) {
               set_value(b, x, y, 0);
               if (count_solutions(b, pos, 0) > 1) {
                    set_value(b, x, y, val);
                    continue;
               }
          }
     }
}

static void save_solution(Board* b) {
     for (int i = 0; i < GRIDSIZE; i++) {
          for (int j = 0; j < GRIDSIZE; j++) {
               b->solution[i * GRIDSIZE + j] = get_value(b, i, j);
          }
     }
}

static bool compare(int* a, int* b, int size) {
     for (int i = 0; i < size; i++) {
          if (a[i] != b[i]) return false;
     }
     return true;
}

static void copy(int* src, int* dest, int size) {
     for (int i = 0; i < size; i++) {
          dest[i] = src[i];
     }
}

static void sort(int* arr, int size) {
     for (int i = 1; i < size; i++) {
          if (arr[i] < arr[i - 1]) {
               while (arr[i] < arr[i - 1]) {
                    swap(&arr[i], &arr[i - 1]);
               }
          }
     }
}

static void print_array(int* arr, int size) {
     for (int i = 0; i < size; i++) {
          printf("%d", arr[i]);
     }
     printf("\n");
}

/* static bool valid_row(Board* b, int* row) {
     for (int i = 0; i < GRIDSIZE; i++) {
          if (row[i] == )
     }
} */

static bool check_values(Board* b) {
     for (int i = 0; i < GRIDSIZE; i++) {
          for (int j = 0; j < GRIDSIZE; j++) {
               if (get_value(b, i, j) < 0 || get_value(b, i, j) > GRIDSIZE)
                    return false;
          }
     }
     return true;
}

Board gen_board() {
     Board b = new_board();
     fill_board(&b);
     save_solution(&b);
     remove_digits(&b);
     return b;
}

void list_boards(Board* list, int count) {
     printf("generating %d boards\n", count);
     fputs("\e[?25l", stdout); /* hide the cursor */
     for (int i = 0; i < count; i++) {
          printf("%d%%\r", i * 5);
          fflush(stdout);
          list[i] = gen_board();
     }
     fputs("\e[?25h", stdout); /* show the cursor */
}

bool remove_board(Board* list, int count, int idx) {
     if (idx < 0 || idx >= count) return false;
     for (int i = 0; i < count; ++i) {
          if (i == idx) {
               list[i] = new_board();
               return true;
          }
     }
     return false;
}

Board choose_board(Board* list, int count) {
     for (int i = 0; i < count; ++i) {
          if (count_filled(&list[i]) < 33) {
               return list[i];
          }
     }
     return list[0];
 }

bool guess(Board* b, int pos, int val) {
     if (b->solution[pos] == val) return true;
     return false;
}
