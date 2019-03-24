#ifndef PARALLEL
#define PARALLEL
#include "Parallel.h"
#include <complex.h>

double complex* Parallel_BDD(int processors, double complex* s, int tamanho);
double complex* Sequential_BDD(int radius, double complex* s, int tamanho);
double* Soft_decision(int radius, double* target_b, double* target_r, int tamanho);
double Argumento(double complex* s, double complex* p, int tam);


#endif // PARALLEL
