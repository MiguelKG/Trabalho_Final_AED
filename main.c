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
void print_matrix( Matrix *mMasterHead );
int print_matrix_string_length ( float n );

//  Declaração de funções de debug ( REMOVER ANTES DE ENTREGAR )

void test_print_line ( Matrix *mHead );
void test_print_column ( Matrix *mHead );

//  Main

int main () {
    Matrix *matrix = matrix_create();
}

//  Definição de funções

//  matrix_create()
//  Cria uma matriz de acordo com inputs do usuário

Matrix* matrix_create( void ) {
    int line = -1;
    int column = -1;
    float value = -1.0;
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
    } while ( lines < 1 || columns < 1) ;

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
                column = 1;
            }    
        } while ( ( line < 0 || line > lines ) || ( column < 1 || column > columns ) );

        if ( line != 0 && value != 0) {

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
                e o que será sucedido pelo novo nodo adicionado. O primeiro checa as linhas e o
                segundo as colunas

                Obs:
                Caso seja adicionado um valor a uma posição já preenchida, o antigo nodo terá
                seu valor sobrescrito pelo do novo, ao invés de ser trocada a referência de seu
                predecessor e ser liberada a memória alocada pelo antigo, visto que isso cria
                confusão entre os ponteiros da matriz
            */

            for (
                temp = mHeadLine->right, tempPrev = mHeadLine;
                temp != mHeadLine && temp->column < node->column;
                tempPrev = temp, temp = temp->right
            );

            if ( temp->column == node->column ) {
                temp->info = node->info;
            } else {
                tempPrev->right = node;
                node->right = temp;
            }

            for (
                temp = mHeadColumn->below, tempPrev = mHeadColumn;
                temp != mHeadColumn && temp->line < node->line;
                tempPrev = temp, temp = temp->below
            );

            if ( ! ( temp->line == node->line ) ) {
                tempPrev->below = node;
                node->below = temp;
            }

            print_matrix( mMasterHead );

        } else {
            /*
                Se o usuário digitar o valor 0 para algum nodo, a tabela será
                printada e nada mais será feito (visto que nodos iguais a 0
                não devem ser alocados)
            */
            if ( value == 0 ) {
                print_matrix( mMasterHead );
            }
        }

    } while ( line != 0 );

    return mMasterHead;
}

//  print_matrix()
//  Printa a matrix inteira como uma tabela no console

void print_matrix( Matrix *mMasterHead ) {
    int maxElements = 0;    //  Número de elementos por linha da matriz
    int spaces = 0;         //  Armazena o resultado de print_matrix_string_length()
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
                printf( "%d     ", temp->column );
                temp = temp->right;
            } else
            if ( temp->column == -1 && i == 0) {
                printf( "|%d     ", temp->line );
                temp = temp->right;
            } else
            if ( i == temp->column ) {
                spaces = print_matrix_string_length ( temp->info );
                if ( fmod( temp->info, 1 ) == 0 ) {
                    printf( "%.0f", temp->info );
                } else {
                    printf( "%.2f", temp->info );
                }
                while ( spaces != 0 ) {
                    printf( " " );
                    spaces--;
                }
                temp = temp->right;
            } else {
                printf( "0     " );
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
    print_matrix_string_length()

    Função que recebe um valor da matriz e retorna a quantidade de espaços
    que deverão suceder o número em sua impressão na função print_matrix(),
    de modo a manter a formatação, seguindo uma série de critérios

    Números que ocupam até 6 caracteres são suportados e serão corretamente
    formatados
*/

int print_matrix_string_length ( float n ) {
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

//  << REMOVER ANTES DE ENTREGAR >>
//  Funções para ajudar a debugar

/*
    Nomes iniciados por "test"

    Obs:
    A função de printar a matriz era inicialmente para debug
    apenas, porém a formatação funcionou muito melhor do que eu
    esperava, então acredito que seja uma função interessante
    para se deixar na versão final do projeto
*/

//  test_print_line()
//  printa uma linha da matriz, recebendo como entrada sua cabeça

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

//  test_print_column()
//  printa uma coluna da matriz, recebendo como entrada sua cabeça

void test_print_column ( Matrix *mHead ) {
    printf( "\n" );
    for ( Matrix *temp = mHead->below; temp != mHead; temp = temp->below ) {
        printf( "( %d x %d ) %.2f\n", temp->line, temp->column, temp->info );
    }
    printf( "\n" );
}