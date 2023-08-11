#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//  Structs e Typedefs

struct matrix {
	struct matrix* right;
	struct matrix* below;
	int line;
	int column;
	float info;
};
typedef struct matrix Matrix;

//  Declaração de funções

Matrix* matrix_create( void );
void matrix_destroy( Matrix* m );
void matrix_print( Matrix* m );
float matrix_getelem( Matrix* m, int x, int y );
void matrix_setelem( Matrix* m, int x, int y, float elem );
void matrix_table( Matrix *mMasterHead );
int matrix_table_string_length( float n );

//  Declaração de funções de debug ( REMOVER ANTES DE ENTREGAR )

void test_print_line ( Matrix *mHead );
void test_print_column ( Matrix *mHead );
int test_matrix_byte_size( Matrix* mMasterHead );

//  Main

int main () {
    Matrix *matrix = matrix_create();
    matrix_destroy( matrix );
}

//  Definição de funções

/*
====================
matrix_create()
    Cria uma matriz de acordo com inputs do usuário
====================
*/

Matrix* matrix_create( void ) {
    int line = -1;
    int column = -1;
    float value = -1.0F;
    /* 
        variáveis temp (temporárias) são utilizadas para armazenar posições de listas
        que estão sendo iteradas, tempPrev serve para situações em que é necessário
        guardar um valor anterior de temp durante a iteração
    */
    Matrix *temp = NULL;        
    Matrix *tempPrev;
    Matrix *mHeadLine, *mHeadColumn; // matrix head line, matrix head column

    //  Definição da cabeça principal (indíce 0 - 0)

    Matrix* mMasterHead = ( Matrix * ) malloc ( sizeof( Matrix ) );
    mMasterHead->below = mMasterHead;
    mMasterHead->right = mMasterHead;
    mMasterHead->line = -1;
    mMasterHead->column = -1;

    int lines, columns;

    //  Definição do número total de linhas e colunas da matriz

    do {
        printf( "Digite o número de linhas e colunas da matriz: " );
        scanf( "%d %d", &lines, &columns );
    } while ( lines < 1 || columns < 1 ) ;

    /*
        As seguintes duas repetições criam as cabeças de cada linha e coluna e 
        as conectam (construindo a matriz vazia), conforme o número de linhas
        e colunas informados pelo usuário
    */

    int i;
    
    //  Criação das cabeças de cada linha
    
    temp = mMasterHead;
    for ( i = 1; i <= lines; i++ ) {
        Matrix *mHead = ( Matrix * ) malloc ( sizeof( Matrix ) );
        mHead->line = i;
        mHead->column = -1;
        mHead->below = mMasterHead;
        mHead->right = mHead;

        temp->below = mHead;
        temp = temp->below;
    }

    //  Criação das cabeças de cada coluna

    temp = mMasterHead;
    for ( i = 1; i <= columns; i++ ) {
        Matrix *mHead = ( Matrix * ) malloc ( sizeof( Matrix ) );
        mHead->column = i;
        mHead->line = -1;
        mHead->right = mMasterHead;
        mHead->below = mHead;

        temp->right = mHead;
        temp = temp->right;
    }

    printf( "\ndigite a linha e a coluna da tabela, seguido do valor armazenado: \n" );
    printf( "(Digite 0 para cancelar a qualquer momento)\n" );

    do {
        //  Recebendo dados de cada item da matriz e checando se os valores são válidos
        do {
            scanf( "%d", &line );
            if ( line != 0 ) {
                scanf( "%d %f", &column, &value );

                if ( line < 0 || line > lines ) {
                    printf( "Linha inválida, digite novamente os 3 valores: \n" );
                }
                if ( column < 1 || column > columns ) {
                    printf( "Coluna inválida, digite novamente os 3 valores: \n" );
                }
            } else {
                break;
            }    
        } while ( ( line < 1 || line > lines ) || ( column < 1 || column > columns ) );

        if ( line != 0 ) {

            //  Criação de um novo nodo
            
            Matrix *node = ( Matrix * ) malloc ( sizeof( Matrix ) );

            node->line = line;
            node->column = column;
            node->info = value;
            node->right = NULL;
            node->below = NULL;

            //  Encontra a cabeça da linha do novo nodo

            temp = mMasterHead;
            for ( i = 1; i <= line; i++ ) {
                temp = temp->below;
            }
            mHeadLine = temp;

            //  Encontra a cabeça da coluna do novo nodo

            temp = mMasterHead;
            for ( i = 1; i <= column; i++ ) {
                temp = temp->right;
            }
            mHeadColumn = temp;

            /*
                As repetições a seguir são responsáveis por encontrar o nodo que será precedido
                e o que será sucedido pelo novo nodo adicionado. A primeira checa as linhas e a
                segunda as colunas

                Obs:
                - Caso seja adicionado um valor a uma posição já preenchida, o antigo nodo terá
                seu valor sobrescrito pelo do novo, ao invés de ser trocada a referência de seu
                predecessor e ser liberada a memória alocada pelo antigo, visto que isso cria
                confusão entre os ponteiros da matriz

                - Caso o valor zero seja adicionado a uma posição livre, ele será ignorado e o
                nodo será excluído. Porém, caso a posição esteja ocupada, o antigo nodo que ali
                se localizava será removido
            */

            for (
                temp = mHeadLine->right, tempPrev = mHeadLine;
                temp != mHeadLine && temp->column < node->column;
                tempPrev = temp, temp = temp->right
            );

            if ( node->info != 0 ) {
                if ( temp->column != node->column ) {
                    tempPrev->right = node;
                    node->right = temp;
                } else {
                    temp->info = node->info;
                }
            } else {
                if ( temp->column == node->column ) {
                   tempPrev->right = temp->right;
                }
            }

            for (
                temp = mHeadColumn->below, tempPrev = mHeadColumn;
                temp != mHeadColumn && temp->line < node->line;
                tempPrev = temp, temp = temp->below
            );

            if ( node->info != 0 ) {
                if ( temp->line != node->line ) {
                    tempPrev->below = node;
                    node->below = temp;
                } else {
                    free ( node );
                }
            } else {
                if ( temp->line == node->line ) {
                    tempPrev->below = temp->below;
                    free ( temp );
                    free ( node );
                } else {
                    free ( node );
                }
            }

            matrix_table( mMasterHead );

        }
    } while ( line != 0 );

    return mMasterHead;
}

/*
====================
matrix_table()
    Printa a matrix inteira como uma tabela no console
====================
*/

void matrix_table( Matrix *mMasterHead ) {
    int maxElements = 0;    //  Número de colunas da matrix
    int spaces = 0;         //  Armazena o resultado de matrix_table_string_length()
    int i = 1;              //  Contador da quantidade de dados por linha
    Matrix *temp;

    printf( "\n" );
    
    for ( temp = mMasterHead->right; temp != mMasterHead; temp = temp->right ) {
        maxElements++;
    }

    temp = mMasterHead->right;
    printf( "|X     |" );
    i = 1;
    while ( temp != mMasterHead ) {
        while ( i <= maxElements ) {
            if ( temp->line == -1 ) {
                spaces = matrix_table_string_length( temp->column );
                printf( "%d", temp->column );
                temp = temp->right;
            } else
            if ( temp->column == -1 && i == 0) {
                spaces = matrix_table_string_length( temp->line );
                printf( "|%d", temp->line );
                temp = temp->right;
            } else
            if ( i == temp->column ) {
                spaces = matrix_table_string_length( temp->info );
                if ( fmod( temp->info, 1 ) == 0 ) {
                    printf( "%.0f", temp->info );
                } else {
                    printf( "%.2f", temp->info );
                }
                temp = temp->right;
            } else {
                spaces = 5;
                printf( "0" );
            }

            while ( spaces != 0 ) {
                printf( " " );
                spaces--;
            }
            printf( "|" );
            i++;
        }

        temp = temp->below;
        i = 0;
        printf( "\n" );
    }
    printf( "\n" );
}

/*
====================
matrix_table_string_length()
    Função que recebe um valor da matriz e retorna a quantidade de espaços
    que deverão suceder o número em sua impressão na função matrix_table(),
    de modo a manter a formatação, seguindo uma série de critérios

    Números que ocupam até 6 caracteres são suportados e serão corretamente
    formatados
====================
*/

int matrix_table_string_length ( float n ) {
    if ( n > 0 ) {
        if ( fmod( n, 1 ) == 0 ) {
            if ( n >= 100 ) {
                return 3;   // 000XXX
            } else if ( n >= 10 ) {
                return 4;   // 00XXXX
            } else {
                return 5;   // 0XXXXX
            }
        } else {
            if ( n >= 100 ) {
                return 0;   // 000.00
            } else if ( n >= 10 ) {
                return 1;   // 00.00X
            } else {
                return 2;   // 0.00XX
            }
        }
    } else {
        if ( fmod( n, 1 ) == 0 ) {
            if ( n <= -100 ) {
                return 2;   // -000XX
            } else if ( n <= -10 ) {
                return 3;   // -00XXX
            } else {    
                return 4;   // -0XXXX
            }
        } else {
            if ( n <= -10 ) {
                return 0;   // -00.00
            } else {
                return 1;   // -0.00X
            }
        }
    }
}

/*
====================
matrix_print()
    
    imprime os dados da matriz recebida no mesmo formato da entrada de
    matrix_create()
====================
*/

void matrix_print( Matrix* m ) {
    Matrix *temp, *mHead;
    int x = 0, y = 0;

    for ( temp = m->right; temp != m; temp = temp->right, x++ );
    for ( temp = m->below; temp != m; temp = temp->below, y++ );
    printf( "\n%d %d\n", x, y );

    temp = m->below;
    mHead = m->below;
    while ( temp != m ) {
        if ( temp->column != -1 ) {
            printf( "%d %d %.2f\n", temp->line, temp->column, temp->info );
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
matrix_getelem()

    Retorna o dado armazenado na posição (x, y) da matriz (x = linha, y = coluna)
====================
*/

float matrix_getelem( Matrix* m, int x, int y ) {
    Matrix *temp, *mHead;
    
    for ( temp = m->right; temp->column != y && temp != m; temp = temp->right );
    mHead = temp;
    for ( temp = temp->below; temp->line != x && temp != mHead; temp = temp->below);

    if ( temp->line == x && temp->column == y ) {
        return temp->info;
    }
    return -1;
}

/*
====================
matrix_setelem()

    Altera o dado armazenado na posição (x, y) da matriz (x = linha, y = coluna)
    pelo dado recebido em elem
====================
*/

void matrix_setelem( Matrix* m, int x, int y, float elem ) {
    Matrix *temp, *mHead;
    
    for ( temp = m->right; temp->column != y && temp != m; temp = temp->right );
    mHead = temp;
    for ( temp = temp->below; temp->line != x && temp != mHead; temp = temp->below);

    if ( temp->line == x && temp->column == y ) {
        temp->info = elem;
    }
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

    free(m);
}

//  << REMOVER ANTES DE ENTREGAR >>
//  Funções para ajudar a debugar
//  - Nomes iniciados por "test"

/*
====================
test_print_line()

    printa uma linha da matriz, recebendo como entrada sua cabeça
====================
*/

void test_print_line ( Matrix *mHead ) {
    printf( "\n" );
    for ( Matrix *temp = mHead->right; temp != mHead; temp = temp->right ) {
        printf( "( %d x %d ) %.2f", temp->line, temp->column, temp->info );
        if ( temp->right != mHead ) {
            printf(" -> ");
        }
    }
    printf( "\n\n" );
}

/*
====================
test_print_column()

    printa uma coluna da matriz, recebendo como entrada sua cabeça
====================
*/

void test_print_column ( Matrix *mHead ) {
    printf( "\n" );
    for ( Matrix *temp = mHead->below; temp != mHead; temp = temp->below ) {
        printf( "( %d x %d ) %.2f\n", temp->line, temp->column, temp->info );
    }
    printf( "\n" );
}

/*
====================
test_matrix_byte_size()

    retorna o tamanho em bytes da matriz (útil para verificar
    vazamentos acusados pelo Dr. Memory)
====================
*/

int test_matrix_byte_size( Matrix* mMasterHead ) {
    Matrix *temp, *mHead;
    int byteSize = 0;

    temp = NULL;
    mHead = mMasterHead;

    while ( temp != mMasterHead ) {
        if ( temp == NULL ) {
            temp = mMasterHead;
        }

        byteSize += sizeof( Matrix );

        if ( temp->right == mHead ) {
            temp = temp->right->below;
            mHead = temp;
        } else {
            temp = temp->right;
        }
    }

    return byteSize;
}