#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double* Soft_decision(int radius, double* target_b, double* target_r, int tamanho){
    int i;
    if(radius==0){
        double soma1=0, soma0=0;
        for(i=0; i<tamanho; i++){
            if(target_b[i]==0){
                soma0 = target_r[i] + soma0;
            }
            if(target_b[i]==1){
                soma1 = target_r[i] + soma1;
            }
        }
        free(target_r);
        if(soma0>soma1){
            for(i=0; i<tamanho; i++){
                target_b[i] = 0;
            }
            return target_b;
        }
        else{
            for(i=0; i<tamanho; i++){
                target_b[i] = 1;
            }
            return target_b;
        }
    }
    else if(tamanho == pow(2, radius)){
        free(target_r);
        return target_b;
    }
    else{
        double** tarr = malloc(2*sizeof(double*));
        double** tarb = malloc(2*sizeof(double*));
        double* v;
        double* u;
        for(i=0;i<2;i++){
            tarb[i] = malloc((tamanho/2)*sizeof(double));
            tarr[i] = malloc((tamanho/2)*sizeof(double));
        }
        for(i=0; i<tamanho/2; i++){
            if(target_r[i]<target_r[i+(tamanho/2)]){
                tarr[0][i] = target_r[i];
            }
            else{
                tarr[0][i] = target_r[i+(tamanho/2)];
            }
            tarb[0][i] = fmod(target_b[i] + target_b[i+(tamanho/2)],2);
        }
        v = Soft_decision(radius-1, tarb[0], tarr[0], (tamanho/2));
        for(i=0; i<(tamanho/2); i++){
            if(fmod(target_b[i]+target_b[i+(tamanho/2)], 2) == v[i]){
                tarb[1][i] = target_b[i];
                tarr[1][i] = (target_r[i+(tamanho/2)] + target_r[i])/2;
            }
            else{
                if((target_r[i+(tamanho/2)] > target_r[i])){
                    tarb[1][i] = fmod((target_b[i+(tamanho/2)]+1), 2);
                }
                else{
                    tarb[1][i] = target_b[i+(tamanho/2)];
                }
                tarr[1][i] = fabs(target_r[i+(tamanho/2)] - target_r[i])/2;
            }
        }
        u = Soft_decision(radius, tarb[1], tarr[1], (tamanho/2));
        //computa o retorno
        for(i=0; i<(tamanho/2); i++){
            target_b[i] = u[i];
            target_b[i+(tamanho/2)] = v[i] + u[i];
        }
        for(i=0;i<2;i++){
            free(tarb[i]);
            free(tarr[i]);
        }
        free(tarr);
        free(tarb);
        return target_b;
    }
    return NULL;
}
