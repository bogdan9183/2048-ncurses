#include <stdio.h>
#include <curses.h>
#include <string.h>
#include <wchar.h>
#include <time.h>
#include <sys/select.h>
#include <stdlib.h>

#define XMAX 120
#define TIMEOUT 10
#define S_TO_WAIT 1
#define MILIS_TO_WAIT 0
#define KEYBOARD 0
#define SELECT_ERROR -1
#define SELECT_EVENT 1
#define SELECT_NO_EVENT 0

void fill_with_0 ( int A[][6] ) ;
void Game ( int A[][6], int CP[][6]);
int finished (int A[][6]) ;
int move_piece(int deltax, int deltay, int A[][6], int CP[][6], int *Score);
void print_matrix(WINDOW *win [], int MAT[][6]);
void change_option(int *Row, int Column, int *Selection, int Next, char opts[][50]);
int find_move(int A[][6], int i,int j);
void init_Matrix(int A[][6]);
void copy_mat(int src[][6], int dest[][6]);
int different(int A[][6], int B[][6]);
void add_random(int A[][6]);
void update_cpanel(WINDOW *win, int Score, int Moves, int Time, int Highscore);
int count_0(int A[][6]);
wchar_t auto_move(int A[][6]);
void init_Fields(WINDOW *Field[], WINDOW *background);
void init_colors();
void print_title(WINDOW *win);
int get_highscore();
void update_highscore(int highscore);
int power_of_2(int n);
