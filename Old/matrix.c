#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"

struct matrix {
	struct matrix* right;
	struct matrix* below;
	int line;
	int column;
	float info;
};

Matrix* matrix_create( void ) {
    Matrix* mHead = (Matrix *) malloc (sizeof(Matrix));
    int line = -1, column = -1, value;

    int lines, columns;
    printf("Digite o número de linhas e colunas da matrix: ");
    scanf("%d %d", &lines, &columns);

    do {
        Matrix *node = (Matrix *) malloc (sizeof(Matrix));
        scanf ("%d", &line);
        if (line != 0) {
            scanf("%d %d", &column, &value);
            printf("%d - %d = %d", line, column, value);
        }
    } while (line != 0 && column != 0);
}