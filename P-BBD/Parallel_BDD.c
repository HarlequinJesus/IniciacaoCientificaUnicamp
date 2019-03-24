#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "Parallel.h"
#include <complex.h>
#include <math.h>

#define PHI (1+I)

double complex* Parallel_BDD(int processors, double complex* s, int tamanho){
    if(processors<4 || tamanho==1){
        return Sequential_BDD(0, s, tamanho);
    }
    else{
        int i;
        double complex** s_computado = malloc(4*sizeof(double complex*));
        //separa s em duas metades
        s_computado[0] = malloc((tamanho/2)*sizeof(double complex));
        s_computado[1] = malloc((tamanho/2)*sizeof(double complex));
        for(i=0; i<(tamanho/2); i++){
            s_computado[0][i] = s[i];
            s_computado[1][i] = s[i + (tamanho/2)];
        }
        /*computar a tranformacao de s em outras possibilidades
        de vetor
        */
        s_computado[2] = malloc((tamanho/2)*sizeof(double complex));
        s_computado[3] = malloc((tamanho/2)*sizeof(double complex));
        for(i=0; i<(tamanho/2); i++){
            s_computado[2][i] = (PHI/2) * (s_computado[0][i] - s_computado[1][i]);
            s_computado[3][i] = (PHI/2) * (s_computado[1][i] + s_computado[0][i]);
        }
        //executa as funcoes recurcivas em paralelo do decodificador
        double complex** z = malloc(4*sizeof(double complex*));
        #pragma omp parallel for
        for(i=0; i<4; i++){
            z[i] = Parallel_BDD(processors/4, s_computado[i], tamanho/2);
            free(s_computado[i]);
        }
        free(s_computado);
        //computa os cadidatos de vetores gerados das funcoes em paralelo
        double complex** s_candidatos = malloc(4*sizeof(double complex*));
        for(i=0; i<4; i++){
            s_candidatos[i] = malloc(tamanho*sizeof(double complex));
        }
        for(i=0; i<(tamanho/2); i++){
            s_candidatos[0][i] = z[0][i];
            s_candidatos[0][i+(tamanho/2)] = z[0][i] - (2/PHI)*z[2][i];

            s_candidatos[1][i] = z[0][i];
            s_candidatos[1][i+(tamanho/2)] = (2/PHI)*z[3][i] - z[0][i];

            s_candidatos[2][i] = z[1][i] + (2/PHI)*z[2][i];
            s_candidatos[2][i+(tamanho/2)] = z[1][i];

            s_candidatos[3][i] = (2/PHI)*z[3][i] - z[1][i];
            s_candidatos[3][i+(tamanho/2)] = z[1][i];


        }
        for(i=0; i<4; i++){
            free(z[i]);
        }
        free(z);
        //dos candidatos gerados retornamos o candidato mais proximo de s
        double complex* s_retorno = s_candidatos[0];
        double arg_temp;
        double arg_min = Argumento(s_retorno, s, tamanho);
        for(i=1; i<4; i++){
            arg_temp = Argumento(s_candidatos[i], s, tamanho);
            if(arg_min > arg_temp){
                arg_min = arg_temp;
                free(s_retorno);
                s_retorno = s_candidatos[i];
            }
            else{
                free(s_candidatos[i]);
            }
        }
        return s_retorno;
    }
}

double Argumento(double complex* s, double complex* p, int tam){
    double aux = 0;
    int i;
    for(i=0; i<tam; i++){
        aux = aux + pow(cabs(s[i]-p[i]),2);
    }
    return sqrt(aux);
}
