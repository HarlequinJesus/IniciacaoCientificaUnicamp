#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <math.h>
#include "Parallel.h"

#define PHI (1+I)

double complex* Sequential_BDD(int radius, double complex* s, int tamanho){
    int i;
    if(tamanho <=  pow(2, radius)){
        for(i=0; i<tamanho; i++)
            s[i] = round(creal(s[i])) + round(cimag(s[i]))*I;
        return s;
    }
    //computamos nosso target binario e sua confiabiilidade
    double* target_b = malloc(tamanho*sizeof(double));
    double* target_r = malloc(tamanho*sizeof(double));
    //retorno do reed-muller e da funcao
    double* reed_muller;
    double complex* v_retorno;
    double complex* v_computado = malloc(tamanho*sizeof(double complex));
    //calculo do alvo
    for(i=0; i<tamanho; i++){
        target_b[i] = fmod(round(cimag(s[i])) + round(creal(s[i])), 2);
        target_r[i] = fabs(cimag(s[i]) - round(cimag(s[i])));
        if(target_r[i] > fabs(creal(s[i]) - round(creal(s[i])))){
            target_r[i] = fabs(creal(s[i]) - round(creal(s[i])));
        }
        target_r[i] = 1 - 2*target_r[i];
    }
    //chama soft decision do reed-muller
    reed_muller = Soft_decision(radius, target_b, target_r, tamanho);
    for(i=0; i<tamanho; i++){
        v_computado[i] = (s[i] - reed_muller[i])/PHI;
    }
    //calcula o retorno do bdd sequencial que foi ajustado pela
    //funcao de reed-muller
    v_retorno = Sequential_BDD(radius+1, v_computado, tamanho);
    free(v_computado);
    for(i=0; i<tamanho; i++){
        v_retorno[i] = reed_muller[i] + PHI*v_retorno[i];
    }
    free(reed_muller);
    return v_retorno;
}
