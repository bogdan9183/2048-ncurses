#include "header.h"

void draw_menu(char options[][50]) {
    //desenare meniu
    int current_row, current_column;
    int i=0;
    clear();
    print_title(stdscr);
    current_row = 18; current_column = 18;
    for (i=1;i<=2;i++)	{
        mvaddstr (current_row, current_column, options[i] );
        current_row += 2;
    }
    attron(A_STANDOUT);
    mvaddstr ( 16 , current_column, options[0] );
    attroff(A_STANDOUT);
    //scrie optiunile disponibile, prima va fi cea selectata
    refresh();
}

int Play_Game( int A[][6] , int B[][6], int CP[][6] , int *Score, int *Moves ) {
    int nfds, sel;
    int Time = TIMEOUT;
    int highscore;
    int bkp[6][6], Score_bkp, prev_Scr;
    wchar_t Key;

    WINDOW *Field[16], *c_panel, *title, *background;
    struct timeval timeout;
    init_colors();
    fd_set read_descriptors;
    nfds = 1;
    FD_ZERO(&read_descriptors);
    FD_SET(KEYBOARD, &read_descriptors);
    timeout.tv_sec = S_TO_WAIT;
    timeout.tv_usec = MILIS_TO_WAIT;
    highscore = get_highscore();
    //obtine highscore din fisier
    background = newwin(25, 41 , 15, 10);
    init_Fields( Field, background);
    title = newwin ( 8, XMAX, 0, 0);
    c_panel = newwin ( 20, 60, 10, 50);
    box(c_panel, 0 , 0 );
    print_title(title);
    //initializare fereastra, creare panouri.
    wrefresh(c_panel);
    keypad(c_panel, TRUE);
    time_t timer;
    timer = time(NULL);
    srand(timer);
    update_cpanel( c_panel , *Score, *Moves, Time, highscore);
    print_matrix( Field , A);

    while ( finished ( A ) == 0 ) {
        // executa cat timp se pot efecuta miscari
        sel = select(nfds, &read_descriptors, NULL, NULL, &timeout);
        Key = 'x';
        switch (sel) {
            //asteapta eveniment de la tastatura
            case SELECT_EVENT:
                Key = wgetch ( c_panel ); //tasta citita, se incarca in Key
                Time = TIMEOUT;
                update_cpanel(c_panel, *Score, *Moves, Time, highscore);
                break;
            case SELECT_NO_EVENT:
                // nu se citeste tasta -> timerul scade pana la 0, apoi
                // efectueaza miscarea automat
                if (Time!=0)
                Time -= 1;
                else {
                    Key = auto_move( A );
                    Time = TIMEOUT;
                }
                update_cpanel(c_panel, *Score, *Moves, Time, highscore);
                break;
        }
        FD_SET(KEYBOARD, &read_descriptors);
        timeout.tv_sec = S_TO_WAIT;
        timeout.tv_usec = MILIS_TO_WAIT;
        switch (Key) {
            case (KEY_UP) :
                //tasta introdusa este UP_ARROW
                copy_mat(A, bkp); Score_bkp = *Score;
                if (move_piece(0, -1, A, CP, Score) ) {
                    copy_mat(bkp, B);
                    prev_Scr = Score_bkp;
                    //daca se muta o piesa, se retine ultima amplasare a pieselor
                    //precum si scorul precedent
                    (*Moves)+=1;
                }
                break;
            case (KEY_RIGHT) :
                copy_mat(A, bkp);
                Score_bkp = *Score;
                if (move_piece(1, 0, A, CP, Score) ) {
                    copy_mat(bkp, B);
                    prev_Scr = Score_bkp;
                    (*Moves)+=1;
                }
                break;
            case (KEY_DOWN) :
                copy_mat(A, bkp);
                Score_bkp = *Score;
                if (move_piece(0, 1, A, CP, Score)) {
                    copy_mat(bkp, B);
                    prev_Scr = Score_bkp;
                    (*Moves)+=1;
                }
                break;
            case (KEY_LEFT) :
                copy_mat(A, bkp);
                Score_bkp = *Score;
                if ( move_piece(-1, 0, A, CP, Score ) ) {
                    copy_mat(bkp, B);
                    prev_Scr = Score_bkp;
                    (*Moves)+=1;
                }
                break;
            case 'q' :
                //se intoarce in meniu
                return 1;
            case 'u' :
                copy_mat( B , A );
                copy_mat( B, bkp);
                *Score = prev_Scr;
                Score_bkp = prev_Scr;
                //se intoarce la miscarea precedenta
                break;
            case 'r' :
                copy_mat(A, bkp);
                Score_bkp = *Score;
                update_highscore(0);
                highscore = 0;
                update_cpanel(c_panel, *Score, *Moves, Time, 0);
                break;
            default :
                copy_mat(A, bkp);
                Score_bkp = *Score;
                break;
        }
        if ( different( A, bkp ) ) {
            add_random( A );
            //daca s-a efectuat o miscare valida, se adauga o piesa noua
        }
        if (*Score > highscore) {
            highscore = *Score;
            //actualizare highscore pe ecran + in fisier
            update_highscore(highscore);
        }
        update_cpanel(c_panel, *Score, *Moves, Time, highscore);
        print_matrix(Field, A);
        //actualizare matrice + panou control dupa fiecare miscare
    }
    erase();
    wclear(title);
    mvwprintw(title, 4 ,40,"YOU LOST. BETTER LUCK NEXT TIME!");
    wrefresh(title);
    wgetch(c_panel);
    fill_with_0(A);
    //can jocul se termina, se reinitializeaza matricea A cu 0.
    return 0;

}

void navigate_menu ( char options[][50] ){
    //navigare prin meniul principal
    // primeste ca parametru numele optiunilor
    int Selection = 1, Row = 16 , Column = 18, Game_Started = 0, Score = 0, Moves = 0;
    wchar_t C; C = getch();
    int A[6][6], B[6][6], CP[6][6];

    while ( C != 'q' ) {
        switch ( C ) {
            case (KEY_UP) :
                //tasta UP_ARROW apasata in meniu, duce selectia curenta cu o pozitie in sus
                if ( Selection == 1 ) {
                    change_option( &Row, Column, &Selection, 3, options);
                }
                else {
                    if ( Selection == 3 && Game_Started == 0 )
                        change_option(&Row, Column, &Selection, 1, options);
                    else
                        change_option(&Row, Column, &Selection, Selection-1, options);
                }
                break;
            case ( KEY_DOWN ) :
                //tasta UP_ARROW apasata in meniu, duce selectia curenta cu o pozitie in jos
                if ( Selection == 3 ) {
                    change_option( &Row, Column, &Selection, 1, options);
                }
                else {
                    if (Game_Started == 0 )
                        //daca niciun joc nu este in desfasurare, optiunea RESUME este indisponibila
                        change_option( &Row, Column, &Selection, 3, options);
                    else
                        change_option( &Row, Column, &Selection, Selection + 1, options);
                }
                break;
            case 10 :
                clear();
                //se apasa tasta ENTER
                switch ( Selection ) {
                    case 1 :
                        //se porneste un joc nou, toate variabilele se reinitializeaza
                        Game_Started = 1;
                        Score = 0;
                        Moves = 0;
                        clear();
                        refresh();
                        fill_with_0(A);
                        fill_with_0(CP);
                        init_Matrix(A);
                        copy_mat(A, B);
                        Game_Started = Play_Game(A, B, CP, &Score, &Moves);
                        draw_menu(options);
                        Selection = 1; Row  = 16;
                        break;
                    case 2 :
                        // se continua jocul din ultimul punct
                        clear();
                        refresh();
                        Game_Started = Play_Game(A, B, CP, &Score, &Moves);
                        draw_menu(options);
                        Selection = 1; Row = 16;
                        break;
                    case 3 :
                        return;
                }
            default : break;
        }
        refresh();
        C = getch();
    }
}

int main () {

    char options[4][50];
    strcpy (options[0] , "New Game");
    strcpy (options[1] , "Resume");
    strcpy (options[2] , "Exit" );
    WINDOW * screen ;
    screen = initscr();
    cbreak(); noecho(); keypad(screen , TRUE); curs_set(0);
    start_color();
    use_default_colors();
    clear();
    draw_menu( options );
    navigate_menu ( options );
    curs_set(1);
    endwin();

    return 0;
}
