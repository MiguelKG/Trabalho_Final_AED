#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

struct matrix {
	struct matrix* right;
	struct matrix* below;
	int line;
	int column;
	float info;
};
typedef struct matrix Matrix;

Matrix* matrix_create( unsigned int m );
Matrix* matrix_create_100mb( void );
void matrix_destroy( Matrix* m );
void matrix_print( Matrix* m );
void matrix_table( Matrix *mMasterHead );

int main () {
    Matrix *matrix = matrix_create( 15 );
    matrix_print( matrix );
    matrix_table( matrix );
    matrix_destroy( matrix );
}

/*
====================
matrix_create()
    
    Cria uma matriz quadrada recebendo o tamanho de suas linhas\colunas
    e aleatorizando seus valores
====================
*/

Matrix* matrix_create( unsigned int m ) {
    if ( m == 0 ) {
        return NULL;
    }

    Matrix *mMasterHead;
    Matrix *mHead;
    Matrix *temp;
    Matrix *temp2;
    int x;

    mMasterHead = ( Matrix * ) malloc ( sizeof( Matrix ) );
    mMasterHead->below = mMasterHead;
    mMasterHead->right = mMasterHead;
    mMasterHead->line = -1;
    mMasterHead->column = -1;

    temp = mMasterHead;
    temp2 = mMasterHead;
    for ( int i = 1; i <= m; i++ ) {
        mHead = ( Matrix * ) malloc ( sizeof( Matrix ) );
        mHead->line = i;
        mHead->column = -1;
        mHead->right = mHead;

        temp->below = mHead;
        temp = temp->below;

        mHead = ( Matrix * ) malloc ( sizeof( Matrix ) );
        mHead->column = i;
        mHead->line = -1;
        mHead->below = mHead;

        temp2->right = mHead;
        temp2 = temp2->right;
    }
    temp->below = mMasterHead;
    temp2->right = mMasterHead;

    x = 1;
    srand( time( NULL ) );
    temp = mMasterHead->below;
    temp2 = mMasterHead->right;
    mHead = mMasterHead->below;

    while ( temp != mMasterHead ) {
        Matrix *node = ( Matrix * ) malloc ( sizeof(Matrix) );
        node->column = temp2->column;
        node->line = temp->line;
        node->info = rand() % 101;
        node->below = temp2;
        
        temp->right = node;
        temp2->below = node;

        temp = temp->right;
        temp2 = temp2->right;

        if ( x < m ) {
            x++;
        } else {
            x = 1;
            temp->right = mHead;
            temp = mHead->below;
            temp2 = mHead->right;
            mHead = mHead->below;
        }
    }

    return mMasterHead;
}

/*
====================
matrix_create_100mb()
    
    Cria uma matriz de 100MB utilizando a função matrix_create()
====================
*/

Matrix* matrix_create_100mb( void ) {
    return matrix_create( 1768 );
    // raiz de 3125000
    // 3125000 = ( 100 * ( 1000000 bytes ) ) / sizeof( Matrix )
}

/*
====================
matrix_print()
    
    imprime os dados da matriz recebida
====================
*/

void matrix_print( Matrix* matrix ) {
    if ( matrix == NULL ) {
        return;
    }

    Matrix *temp, *mHead;
    int x = 0;

    for ( temp = matrix->right; temp != matrix; temp = temp->right, x++ );

    printf( "\nTamanho: %d\n", x );

    temp = matrix->below;
    mHead = matrix->below;
    while ( temp != matrix ) {
        if ( temp->column != -1 ) {
            if ( fmod(temp->info, 1) == 0 ) {
                printf( "%d %d %.0f\n", temp->line, temp->column, temp->info );
            } else {
                printf( "%d %d %.2f\n", temp->line, temp->column, temp->info );
            }
        }

        if ( temp->right == mHead ) {
            temp = temp->right;
            temp = temp->below;
            mHead = temp;
        } else {
            temp = temp->right;
        }
    }
}

/*
====================
matrix_table()

    Printa a matrix inteira como uma tabela no console
====================
*/

void matrix_table( Matrix *mMasterHead ) {
    if ( mMasterHead == NULL ) {
        return;
    }

    Matrix *temp;
    Matrix *mHead;

    printf( "\n" );
    
    mHead = mMasterHead;
    temp = mMasterHead->right;

    printf( "|X     |" );
    while ( temp != mMasterHead ) {
            
        if ( temp->line == -1 ) {
            printf( "%*d", -6, temp->column );
        } else
        if ( temp->column == -1 ) {
            printf( "|%*d", -6, temp->line );
        } else {
            if ( fmod( temp->info, 1 ) == 0 ) {
                printf( "%*.0f", -6, temp->info );
            } else {
                printf( "%*.2f", -6, temp->info );
            }
        }

        printf( "|" );

        if ( temp->right == mHead ) {
            mHead = mHead->below;
            temp = mHead;
            printf( "\n" );
        } else {
            temp = temp->right;
        }
    }
    printf( "\n" );
}

/*
====================
matrix_destroy()

    Função responsável por liberar os espaços de memória que estão alocando
    os dados de uma matriz (Exclui uma matriz). utiliza-se a coluna -1 como
    base para se iterar e liberar a memória ocupada por cada nodo. Após
    isso, a coluna -1 também é excluida.

    Obs:
    1) "firstIteration" é necessário, pois "eraser" é inicializado com o
    valor-condição necessário para o encerramento das repetições

    2) As checagens de "temp != m" são especialmente importantes para
    evitar acesso indevido de memória
====================
*/

void matrix_destroy( Matrix* m ) {
    Matrix *temp = m->right;    // Mantém uma referência para o próximo elemento
    Matrix *eraser = m;         // Armazena a posição do elemento que será liberado
    int firstIteration = 1;     // Permite que a primeira iteração ocorra

    while ( eraser != m || firstIteration ) {
        if ( eraser->column != -1) {
            free ( eraser );
        }
        eraser = temp;
        if ( temp != m ) {
            if ( temp->right->column == -1 ) {
                temp = temp->right;
                temp = temp->below;
            } else {
                temp = temp->right;
            }
        }
        if ( firstIteration ) {
            firstIteration = 0;
        }
    }

    firstIteration = 1;
    temp = m->below;
    eraser = m;
    
    while ( eraser != m || firstIteration ) {
        if ( eraser->line != -1 ) {
            free ( eraser );
        }
        eraser = temp;
        if ( temp != m ) {
            temp = temp->below;
        } else {
            temp = NULL;
        }
        if ( firstIteration ) {
            firstIteration = 0;
        }
    }

    free( m );
}