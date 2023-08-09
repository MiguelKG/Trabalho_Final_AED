#ifndef MATRIX_H
#define MATRIX_H

struct matrix;
typedef struct matrix Matrix;

Matrix* matrix_create( void );
void matrix_destroy( Matrix* m );
void matrix_print( Matrix* m );
Matrix* matrix_add( Matrix* m, Matrix* n );
Matrix* matrix_multiply( Matrix* m, Matrix* n );
Matrix* matrix_transpose( Matrix* m );
float matrix_getelem( Matrix* m, int x, int y );
void matrix_setelem( Matrix* m, int x, int y, float elem );

#endif