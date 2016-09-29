#include "header.h"

void fill_with_0 ( int A[][6] ) {
//completare matrice cu valori nule + bordare
    int i , j;
    for ( i = 1 ; i <= 4 ; i++ )
        for ( j = 1 ; j <= 4 ; j++ )
            A[i][j] = 0;
    for ( i = 1 ; i <= 4 ; i++ ) {
        A[5][i] = 999;
        A[i][5] = 999;
        A[0][i] = 999;
        A[i][0] = 999;
    }
}


void print_title ( WINDOW *win) {
    // printare titlu citit din fisier
    FILE *in;
    int i=0;
    char T2048[8][45];
    in = fopen("2048R", "r");
    while (fgets(T2048[i], 50, in) != NULL)
        i++;
    for (i = 0; i < 7; i++)
        mvwaddstr(win, i + 1, 40, T2048[i]);
    fclose(in);
    wrefresh(win);
}

void print_matrix ( WINDOW *win[], int MAT[][6]) {
    int i,j;
    for (i = 0; i <= 3; i++)
        for (j = 0; j <= 3; j++) {
            if (MAT[i + 1][j + 1] != 0) {
                mvwprintw(win[4 * i + j], 2, 2, "    ");
                mvwprintw(win[4 * i + j], 2, 2, "%d", MAT[i + 1][j + 1]);
            }
                // afisare continut matrice in window-ul corespunzator
            else
                mvwprintw(win[4 * i + j], 2, 2, "   ");
            // stergerea caracterelor suplimentare
            wbkgd(win[4 * i + j], COLOR_PAIR( power_of_2 ( MAT[i+1][j+1]) ) );
            //schimba culoarea casetei in functie de valoarea continuta
            wrefresh(win[4*i+j]);
        }
}

void change_option ( int *Row, int Column, int *Selection, int Next, char opts[][50]) {
// vechea optiune revine la font obisnuit; noua optiune va fi scrisa cu highlight
    mvaddstr( (*Row) , Column, opts[*Selection-1]);
    (*Row) += 2 * (Next - *Selection);
    *Selection = Next;
    attron(A_STANDOUT);
    mvaddstr( (*Row), Column, opts[*Selection - 1]);
    attroff(A_STANDOUT);
}

void init_Matrix ( int A[][6] ) {
    // fct. apelata la inceputul jocului
    // genereaza doua valori aleatoare 2 sau 4 pe pozitii aleatoare in matrice
    int x, y ;
    int first_two = 0, value = 0;
    while (first_two < 2) {
        x=0, y=0;
        while (x==0)
            x = rand() % 5;
        while (y==0)
            y = rand() % 5;
        while (value == 0) {
            value = (rand() % 3) * 2;
        }
        if (A[x][y] == 0) {
            A[x][y] = value;
            first_two += 1;
            value = 0;
        }
    }
}

void copy_mat ( int src[][6], int dest[][6]) {
    int i , j;
    // copiaza matricea sursa in matricea destinatie
    for ( i = 0; i <= 5; i++)
        for ( j = 0; j<=5 ; j++)
            dest[i][j] = src[i][j];
}

int different ( int A[][6], int B[][6]) {
   // verifica daca doua matrici au toate campurile egale
    int i , j;
    for ( i = 1 ; i<=4 ; i++)
        for ( j = 1 ; j<= 4; j++)
            if (A[i][j] != B[i][j])
                return 1;
    return 0;
}

void update_cpanel ( WINDOW *win, int Score, int Moves, int Time, int Highscore) {
    // actualizare panou de control
    time_t timer;
    char buffer[26];
    struct tm *tm_info;
    time(&timer);
    tm_info = localtime(&timer);
    strftime(buffer, 26, "%d:%m:%Y  %H:%M:%S", tm_info);
    mvwprintw(win, 1, 10, "Highscore - %4d", Highscore);
    mvwprintw ( win, 3, 10, "Score - %4d", Score);
    mvwprintw ( win, 5, 10, "Moves - %4d", Moves);
    mvwprintw ( win, 7, 10, "Time left - %2d", Time);
    mvwprintw ( win , 9, 10, "Press 'U' to undo last move");
    mvwprintw ( win , 11, 10, "Press arrows to move the tiles");
    mvwprintw ( win , 13, 10, "Press 'Q' to return to main menu");
    mvwprintw ( win, 15, 10, "Press 'R' to reset your highscore");
    mvwprintw ( win , 17, 10, "Current time : %s", buffer);
    //wattron(win, COLOR_PAIR(1));
    wrefresh ( win );
}

int count_0 ( int A[][6] ) {
    int i,j, res=0;
    for ( i = 1 ; i<=4; i++ )
        for ( j = 1;  j<=4; j++)
            if (A[i][j] == 0)
                res++;
    return res;
    // numara pozitiile nule din matricea A
}

void init_Fields ( WINDOW * Field [] , WINDOW * background ) {
    int i, j;
    //init_pair ( 1, 2, 5 );
    //init_pair( 20 , 1 , 7);
    //wbkgd (background, COLOR_PAIR(20 ) );
    //wrefresh(background);
    for (i = 0; i <= 3; i++)
        for (j = 0; j <= 3; j++) {
            Field[4 * i + j] = newwin( 4, 7 , 13 + 5 * i, 10 + 9 * j);
            wrefresh ( Field[4*i+j]);
        }
    // initializare campuri de joc
}

int power_of_2 ( int n ) {
    if ( n == 0 || n == 1 )
        return 1;
    else return 1 + power_of_2( n / 2 );
//identificare campuri dupa puteri ale lui 2
    // 0 = 1 = 2^1; (1 nu intereseaza decat in cazul impartirilor succesive)
    // pt 2^k functia va returna k+1.
}

void init_colors ( ) {
    init_pair ( 1 , COLOR_BLACK, 237 );
    init_pair ( 2, COLOR_BLACK , 249 );
    init_pair( 3 , COLOR_BLACK , 214) ;
    init_pair( 4, COLOR_BLACK,  202 );
    init_pair( 5 , COLOR_BLACK,  196 ) ;
    init_pair( 6 , COLOR_BLACK,  199) ;
    init_pair( 7 , COLOR_BLACK,  201 );
    init_pair( 8 , COLOR_BLACK,  123 );
    init_pair( 9 , COLOR_BLACK,  121 );
    init_pair( 10 , COLOR_BLACK, 226);
    init_pair( 11 , COLOR_BLACK, 231 );
    init_pair( 12 , COLOR_BLACK, 255);
// initializare perechi de culori .
}

int get_highscore() {
    // obtine scorul maxim salvat in fisierul Highscore
    FILE *in;
    int x;
    in = fopen ("Highscore", "r");
    fscanf (in, "%d", &x );
    fclose(in);
    return x;

}

void update_highscore( int highscore ) {
    // scrie scorul maxim in fisier
    FILE *in;
    in = fopen ("Highscore","w");
    fprintf(in,"%d", highscore);
    fclose(in);
}
