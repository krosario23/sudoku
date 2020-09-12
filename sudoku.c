#include "board.h"
#include <time.h>
#include <unistd.h>

#define BOARDCOUNT 20

static bool is_letter(char row) {
     return (row >= 'A' && row < 'A' + GRIDSIZE) ||
            (row >= 'a' && row < 'a' + GRIDSIZE);
}

static bool is_digit(char col) {
     return col > '0' && col <= '9';
}

static void title_menu(int* choice) {
     system("clear");
     printf("+------------------------------------+\n");
     printf("|                                    |\n");
     printf("|               sudoku               |\n");
     printf("|                                    |\n");
     printf("|            (1) new game            |\n");
     printf("|            (2) load game           |\n");
     printf("|                                    |\n");
     printf("+------------------------------------+\n");
     printf("> ");
     scanf("%d", choice);
}

static void load_menu(int* choice) {
     system("clear");
     printf("+------------------------------------+\n");
     printf("|                                    |\n");
     printf("|             load board             |\n");
     printf("|                                    |\n");
     printf("|            (1) load from file      |\n");
     printf("|            (2) enter string        |\n");
     printf("|                                    |\n");
     printf("+------------------------------------+\n");
     printf("> ");
     scanf("%d", choice);
}

static bool lowercase(char c) {
     return c >= 'a' && c <= 'a' + GRIDSIZE;
}

static char get_row() {
     //get row index
     char row;
     printf("input row (A-Z): ");
     fseek(stdin, 0, SEEK_END);
     scanf("%c", &row);

     while (!is_letter(row)) {
          printf("'%c' is not a valid row index\n", row);
          printf("try again: ");
          fseek(stdin, 0, SEEK_END);
          scanf("%c", &row);
     }

     if (lowercase(row)) {
          row -= 32;
     }

     return row;
}

static int get_col() {
     char col;
     printf("input col (1-9): ");
     fseek(stdin, 0, SEEK_END);
     scanf("%c", &col);

     while (!is_digit(col)) {
          printf("'%c' is not a valid column index\n", col);
          printf("try again: ");
          fseek(stdin, 0, SEEK_END);
          scanf("%c", &col);
     }
     return col - 49;
}

static int get_val() {
     char val;
     printf("input value (1-9): ");
     fseek(stdin, 0, SEEK_END);
     scanf("%c", &val);

     while (!is_digit(val)) {
          printf("'%c' is not a valid value\n", val);
          printf("try again: ");
          fseek(stdin, 0, SEEK_END);
          scanf("%c", &val);
     }
     return val - 48;
}

static Board select_board(Board* boards) {
     char select;
     int i;
     for (i = 0; i < BOARDCOUNT;) {
          print_board(&boards[i]);
          printf("           board #%d/%d\n", i + 1, BOARDCOUNT);
          printf("        <--- (b) (n) --->   \n");
          printf("enter b to go to previous board\n");
          printf("enter n to go to next board\n");
          printf("enter s select board: ");
          scanf("%c", &select);
          switch (select) {
               case 'n': {
                    if (i == BOARDCOUNT - 1) {
                         i = 0;
                    } else {
                         i++;
                    }
                    break;
               }
               case 'b' : {
                    if (i == 0) {
                         i = BOARDCOUNT - 1;
                    } else {
                         i--;
                    }
                    break;
               }
               case 's': {
                    goto exit_loop;
               }
               default:
                    printf("'%c' is not a valid command, try again\n", select);
                    sleep(1);
                    continue;
          }
     }
     exit_loop:
          return boards[i];
}

static void start_game(Board* b) {
     while (count_filled(b) < 81) {
          //to_string(b);
          print_board(b);

          //show_solution(&b);
          int row = get_row();
          int col = get_col();

          if (!is_empty(b, row - 65, col)) {
               printf("this cell is already full\n");
               sleep(1);
               continue;
          }
          while(1) {
               int val = get_val();

               if (guess(b, (row - 65) * GRIDSIZE + col, val)) {
                    set_value(b, row - 65, col, val);
                    break;
               } else {
                    printf("wrong number, try again\n");
                    sleep(1);
                    continue;
               }
          }
     }
}

static void run(Board* boards) {
     int choice;

title:
     title_menu(&choice);
     switch (choice) {
          case 1: {
               list_boards(boards, BOARDCOUNT);
               Board b = select_board(boards);
               start_game(&b);
               break;

          }
          case 2: {
               int load;
               load_menu(&load);
               switch (load) {
                    case 1:
                    /*load from file*/
                    case 2:
                    /*read string*/
                    default:;

               }

               break;
          }
          default:
               printf("please select either (1) or (2)\n");
               goto title;
     }
}

int main() {
     srand(time(NULL));
     Board boards[BOARDCOUNT];

     run(boards);

     /* clock_t t;
     t = clock();
     list_boards(boards, BOARDCOUNT);
     t = clock() - t;
     double elapsed = ((double)t) / CLOCKS_PER_SEC;
     printf("generator took %f seconds to create %d puzzles\n", elapsed, BOARDCOUNT);

     int hist[50] = {0};
     for (int i = 0; i < BOARDCOUNT; i++) {
          for (int n = 25; n < 51; n++) {
               if (n == count_filled(&boards[i])) {
                    hist[n]++;
               }
          }
     }

     for (int x = 25; x < 50; x++) {
          //printf("# of boards with %d clues: %d\n", x, hist[x]);
     } */
}
