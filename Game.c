#include "header.h"

int finished ( int A[][6] ) {
    //returneaza 0 daca exista spatii goale sau mutari disponibile
    //returneaza 1 in caz contrar
    int i, j;
    for ( i = 1 ; i <= 4 ; i++)
        for ( j = 1 ; j <= 4 ; j++)
            if ( A[i][j] == 0 || find_move( A, i, j ) )
                return 0;
    return 1;
}

int find_move(int A[][6], int i, int j) {
    return (A[i][j] == A[i + 1][j] || A[i][j] == A[i - 1][j] ||
            A[i][j] == A[i][j - 1] || A[i][j] == A[i][j + 1]);
// cauta daca mai exista miscari disponibile pe tabla
}

wchar_t auto_move ( int A[][6] ) {
    //returneaza directia corespunzatoare celei mai eficiente miscari
    wchar_t RESULT;
    int CP[6][6], BKP[6][6], mv=0;
    int max_zero = 0, scor=0;
    //in max 0 tinem minte numarul maxim de celule nule rezultate in urma fiecarei miscari
    copy_mat(A,BKP);
    mv = move_piece(0, -1, BKP, CP, &scor);
    // se copiaza matricea A in BKP pentru a testa fiecare miscare in parte
    // mv = 0 <=> s-a efectuat o miscare in urma careia s-au miscat piese
    if ( count_0( BKP ) > max_zero && mv > 0 ) {

        RESULT = KEY_UP;
        max_zero = count_0( BKP );
    }
    mv = 0;
    copy_mat(A, BKP);
    mv = move_piece(0, 1, BKP, CP, &scor);
    if (count_0(BKP) > max_zero && mv >0) {
        RESULT = KEY_DOWN;
        max_zero = count_0(BKP);
    }
    mv = 0;
    copy_mat(A, BKP);
    mv = move_piece(-1, 0, BKP, CP, &scor);
    if (count_0(BKP) > max_zero && mv > 0) {
        RESULT = KEY_LEFT;
        max_zero = count_0(BKP);
    }
    mv = 0;
    copy_mat(A, BKP);
    mv = move_piece(1, 0, BKP, CP, &scor);
    if (count_0(BKP) > max_zero && mv > 0){
        RESULT = KEY_RIGHT;
        max_zero = count_0(BKP);
    }
    return RESULT;
}

void add_random ( int A[][6] ) {
    // adauga o valoare aleatoare intr-o pozitie aleatoare disponibila
    int x, y, value=0;
    x = rand() % 5 ;
    y = rand() % 5 ;
    while ( A[x][y] != 0 ) {
        x = rand() % 5 ;
        y = rand() % 5 ;
    }
    while ( value == 0 ) {
        value = rand() % 3;
    }
    value *=2;
    A[x][y]=value;
}

void deplasare ( int deltax, int deltay, int line, int col, int A[][6], int CP[][6], int *Scor ) {
    if ( A[line+deltay][col+deltax] != A[line][col] && A[line+deltay][col+deltax] != 0 )
        return;
        // incercam sa mutam piesa pe o pozitie ocupata sau piesa se afla la marginea tablei
    else {
        if ( A[line+deltay][col+deltax] == A[line][col] && CP[line+deltay][col+deltax] == 0 ) {
            //pozitie disponibila pe care nu s-a efectuat o adunare pana in punctul curent
            A[line+deltay][col+deltax] *= 2;
            *Scor += A[line+deltay][col+deltax];
            //actualizare scor
            A[line][col] = 0;
            CP[line+deltay][col+deltax] = 1;
            //tinem minte ca pe pozitia respectiva s-a facut o mutare
            return;
        }
        else if (A[line+deltay][col+deltax] == 0 )	{
            A[line+deltay][col+deltax] = A[line][col];
            A[line][col] = 0;
            //mutare piesa pe spatiu gol
        }
        deplasare ( deltax, deltay, line+deltay, col+deltax, A, CP, Scor );
        // se merge recursiv cu fiecare piesa atat timp cat mai poate fi mutata
    }
}

int move_piece (int deltax, int deltay, int A[][6], int CP[][6], int * Scor ) {
    fill_with_0 ( CP ) ;
    int T[6][6];
    copy_mat(A,T);
    int i , j;
    // se stabileste ordinea de mutare a celulelor
    // pt deplasare la stanga / sus -> se merge de la 1 la 4
    // pt deplasare la dreapta / jos -> se merge de la 4 la 1
    // daca n-ar exista aceasta diferentiere, mutarile nu se executa corect
    if (deltax == -1 || deltay == -1) {
        for ( i = 1 ; i <= 4 ; i++)
            for ( j = 1 ; j <= 4 ; j++)
                deplasare ( deltax, deltay,  i , j , A, CP, Scor );
    }
    else {
        for ( i = 4 ; i >= 1 ; i-- )
            for ( j = 4 ; j >= 1 ; j-- )
                deplasare ( deltax, deltay, i , j , A, CP, Scor);

    }
    return ( different (A ,T) );
    // returneaza valoarea 1 daca s-a execuat o miscare valida ( cel putin
    // o piesa si-a schimbat pozitia sau 0 in caz contrar
}
