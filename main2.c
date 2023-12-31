#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

float** matrix_create( unsigned int m );
float** matrix_create_100mb( void );
void matrix_destroy( float** matrix, int sizeX );
void matrix_print( float** m, int sizeX, int sizeY );
void matrix_table( float **matrix, int sizeX, int sizeY );
float** matrix_multiply( float** m, float** n , int nColluns , int mLinhas , int collumMLineN);
void matrix_internal_add_node( float **matrix, int line, int column, float value );
float** matrix_internal_create( int sizeX, int sizeY );

int matrix_byte_size( float** mMasterHead, int sizeX, int sizeY );

int firstElement = 1;

int main () {
/*  
    //Teste para Multiplicação

    int **matrix = matrix_create( 5 );
    matrix_table( matrix, 5, 5 );

    int **matrix2 = matrix_create( 5 );
    matrix_table( matrix2, 5, 5 );

    int **matrixFinal = matrix_multiply( matrix, matrix2 , 5 , 5, 5 );
    matrix_table( matrixFinal, 5 , 5 );

    matrix_destroy( matrix, 5);
    matrix_destroy( matrix2, 5);
    matrix_destroy( matrixFinal, 5);
*/
    float **matrix = matrix_create_100mb();
    matrix_internal_add_node(matrix, 300, 455, 99);
    float **matrix2 = matrix_create( 5 );
    
    matrix_table( matrix2, 5, 5 );
    matrix_destroy( matrix, 5000 );
    matrix_destroy( matrix2, 5 );
}

/*
====================
matrix_create()
    
    Cria uma matriz quadrada recebendo o tamanho de suas linhas\colunas
    e aleatorizando seus valores
====================
*/

float** matrix_create( unsigned int m ) {
    if ( firstElement ) {
        srand( time( NULL ) );
        firstElement = 0;
    }

    if ( m == 0 ) {
        return NULL;
    }

    float **matrix = ( float ** ) malloc( sizeof( float * ) * m );

    for ( int i = 0; i < ( int ) m; i++) {
        matrix[ i ] = ( float * ) malloc( sizeof( float ) * m );
        for ( int i2 = 0; i2 < ( int ) m; i2++ ) {
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

    Retorna a multiplicação de duas matrizes introduzidas
====================
*/

float** matrix_multiply( float** m, float** n , int nCollums , int mLines , int collumMLineN){

    int somador;
    float **finalMatrix = matrix_internal_create ( mLines , nCollums );
    int i, i2 = 1, i3 = 1;

    for ( i = 1 ; i <= nCollums ; i++ ) {
        while ( i3 <= mLines ) {
            somador = 0;
            while ( i2 <= collumMLineN ) {
                somador +=  m[ i3 - 1 ] [ i2 - 1 ] * n[ i2 - 1 ] [ i - 1 ];
                i2++;
            }

            matrix_internal_add_node( finalMatrix, i3, i, somador );
            i2 = 1;
            i3++;
        }
        i3 = 1;
    }
    
    return finalMatrix;
}

/*
====================
matrix_create_100mb()
    
    Cria uma matriz de 100MB utilizando a função matrix_create()
====================
*/

float** matrix_create_100mb( void ) {
    return matrix_internal_create( 5000, 5000 );
    // raiz de 25.000.000
    // 25.000.000 = ( 100 * ( 1000000 bytes ) ) / sizeof( int )
}

/*
====================
matrix_print()
    
    imprime os dados da matriz recebida
====================
*/

void matrix_print( float** matrix, int sizeX, int sizeY ) {
    printf( "%d %d\n", sizeX, sizeY );
    for ( int i = 0; i < sizeX; i++ ) {
        for ( int i2 = 0; i2 < sizeY; i2++ ) {
            printf( "%d %d %f\n", i + 1, i2 + 1, matrix[i][i2] );
        }
    }
}

/*
====================
matrix_table()

    Printa a matrix inteira como uma tabela no console
====================
*/

void matrix_table( float **matrix, int sizeX, int sizeY ) {
    if ( matrix == NULL ) {
        return;
    }

    printf( "\n" );

    printf( "|X     |" );
    for( int i = 0; i < sizeY; i++ ) {
        printf( "%-6d|", i + 1 );//
    }
    printf( "\n" );

    for ( int i = 0; i < sizeX; i++ ) {
        printf( "|%-6d|", i + 1 );
        for ( int i2 = 0; i2 < sizeY; i2++ ) {
            if ( fmod( matrix[i][i2], 1 ) == 0 ) {
                printf( "%-6.0f|", matrix[i][i2] );
            } else {
                printf( "%-6.2f|", matrix[i][i2] );
            }
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

void matrix_destroy( float** matrix, int sizeX ) {
    for ( int i = 0; i < sizeX; i++ ) {
        free( matrix[ i ] );
    }
    free( matrix );
}

void matrix_internal_add_node( float **matrix, int line, int column, float value ) {
    matrix[ line - 1 ][ column - 1 ] = value;
}

float** matrix_internal_create( int sizeX, int sizeY ) {
    if ( sizeX == 0 || sizeY == 0 ) {
        return NULL;
    }

    float **matrix = ( float ** ) malloc( sizeof( float * ) * sizeX );

    for ( int i = 0; i < sizeX; i++) {
        matrix[ i ] = ( float * ) malloc( sizeof( float ) * sizeY );
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

int matrix_byte_size( float** matrix, int sizeX, int sizeY ) {
    int byteSize = 0;

    for ( int i = 0; i < sizeX; i++ ) {
        for ( int i2 = 0; i2 < sizeY; i2++ ) {
            byteSize += (int) sizeof( matrix[i][i2] );
        }
    }

    return byteSize;
}