#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int** matrix_create( unsigned int m );
int** matrix_create_100mb( void );
void matrix_destroy( int** matrix, int sizeX, int sizeY );
void matrix_print( int** m, int sizeX, int sizeY );
void matrix_table( int **matrix, int sizeX, int sizeY );
//Matrix* matrix_multiply( Matrix* m, Matrix* n );
void matrix_internal_add_node( int **matrix, int line, int column, float value );
int** matrix_internal_create( int sizeX, int sizeY );

int matrix_byte_size( int** mMasterHead, int sizeX, int sizeY );

int firstElement = 1;

int main () {
    int **matrix = matrix_internal_create( 5, 9 );
    matrix_internal_add_node(matrix, 2, 3, 99);
    int **matrix2 = matrix_create( 5 );

    matrix_table( matrix, 5, 9 );
    matrix_table( matrix2, 5, 5 );
    matrix_destroy( matrix, 5, 9 );
    matrix_destroy( matrix2, 5, 5 );
}

/*
====================
matrix_create()
    
    Cria uma matriz quadrada recebendo o tamanho de suas linhas\colunas
    e aleatorizando seus valores
====================
*/

int** matrix_create( unsigned int m ) {
    if ( firstElement ) {
        srand( time( NULL ) );
        firstElement = 0;
    }

    if ( m == 0 ) {
        return NULL;
    }

    int **matrix = ( int ** ) malloc( sizeof( int * ) * m );

    for ( int i = 0; i < m; i++) {
        matrix[ i ] = ( int * ) malloc( sizeof( int ) * m );
        for ( int i2 = 0; i2 < m; i2++ ) {
            if ( rand() % 2 != 0 && rand() % 2 != 0 ){
                matrix[ i ][ i2 ] = rand() % 100 + 1;
            } else {
                matrix[ i ][ i2 ] = 0;
            }
        }
    }

    return matrix;
}

/*
====================
matrix_multiply()

    Retorna a multiplicação de duas matrizes introduzidas, considerando que as matrizes tem tamanho igual
====================
*/

/*Matrix* matrix_multiply( Matrix* m, Matrix* n ){
    int x = 0, tempForSum = 0;
    int tempX = 0, tempY = 0;
    Matrix *mTemp, *mHead;
    Matrix *nTemp, *nHead;
    for ( nTemp = n->right; nTemp != n; nTemp = nTemp->right, x++ );
    Matrix *finalMatrix = matrix_internal_create(x, x);
    mHead = m->below;
    mTemp = mHead->right;
    nHead = n->right;
    nTemp = nHead->below;
    do{
        if( nTemp != nHead && mTemp != mHead ){
            if( mTemp->column == nTemp->line ){
                tempForSum += mTemp->info * nTemp->info;
                tempX = mTemp->line;
                tempY = nTemp->column;
                mTemp = mTemp->right;
                nTemp = nTemp->below;
        }else{
            if(mTemp->column < nTemp->line){ 
                mTemp = mTemp->right;
            }else if ( nTemp->line < mTemp->column ){
                nTemp = nTemp->below;
            }else{
                mTemp = mTemp->right;
                nTemp = nTemp->below;
            }
        }
        } else {
            if ( tempForSum != 0 ) {
                matrix_internal_add_node( finalMatrix ,tempX ,tempY ,tempForSum );
                tempForSum = 0;
        }
        if ( mHead->below == m  && nHead != n) {
            nHead = nHead->right;
            nTemp = nHead->below;
            mHead = m->below;
            mTemp = mHead->right; 
        } else {
            mHead = mHead->below;
            mTemp = mHead->right;
            nTemp = nHead->below;
        }
    }
}while( nHead != n );
return finalMatrix;
}*/

/*
====================
matrix_create_100mb()
    
    Cria uma matriz de 100MB utilizando a função matrix_create()
====================
*/

int** matrix_create_100mb( void ) {
    return matrix_create( 5000 );
    // raiz de 25.000.000
    // 25.000.000 = ( 100 * ( 1000000 bytes ) ) / sizeof( int )
}

/*
====================
matrix_print()
    
    imprime os dados da matriz recebida
====================
*/

void matrix_print( int** matrix, int sizeX, int sizeY ) {
    printf( "%d %d\n", sizeX, sizeY );
    for ( int i = 0; i < sizeX; i++ ) {
        for ( int i2 = 0; i2 < sizeY; i2++ ) {
            printf( "%d %d %d\n", i + 1, i2 + 1, matrix[i][i2] );
        }
    }
}

/*
====================
matrix_table()

    Printa a matrix inteira como uma tabela no console
====================
*/

void matrix_table( int **matrix, int sizeX, int sizeY ) {
    if ( matrix == NULL ) {
        return;
    }

    printf( "\n" );

    printf( "|X     |" );
    for( int i = 0; i < sizeY; i++ ) {
        printf( "%-6d|", i + 1 );
    }
    printf( "\n" );

    for ( int i = 0; i < sizeX; i++ ) {
        printf( "|%-6d|", i + 1 );
        for ( int i2 = 0; i2 < sizeY; i2++ ) {
            printf( "%-6d|", matrix[i][i2] );
        }
        printf( "\n" );
    }
}

/*
====================
matrix_destroy()

    Função responsável por liberar os espaços de memória que estão alocando
    os dados de uma matriz (Exclui uma matriz)
====================
*/

void matrix_destroy( int** matrix, int sizeX, int sizeY ) {
    for ( int i = 0; i < sizeX; i++ ) {
        free( matrix[ i ] );
    }
    free( matrix );
}

void matrix_internal_add_node( int **matrix, int line, int column, float value ) {
    matrix[ line - 1 ][ column - 1 ] = value;
}

int** matrix_internal_create( int sizeX, int sizeY ) {
    if ( sizeX == 0 || sizeY == 0 ) {
        return NULL;
    }

    int **matrix = ( int ** ) malloc( sizeof( int * ) * sizeX );

    for ( int i = 0; i < sizeX; i++) {
        matrix[ i ] = ( int * ) malloc( sizeof( int ) * sizeY );
        for ( int i2 = 0; i2 < sizeY; i2++ ) {
            matrix[ i ][ i2 ] = 0;
        }
    }

    return matrix;
}

/*
====================
matrix_byte_size()

    retorna o tamanho em bytes de uma matriz
====================
*/

int matrix_byte_size( int** matrix, int sizeX, int sizeY ) {
    int byteSize = 0;

    for ( int i = 0; i < sizeX; i++ ) {
        for ( int i2 = 0; i2 < sizeY; i2++ ) {
            byteSize += (int) sizeof( matrix[i][i2] );
        }
    }

    return byteSize;
}