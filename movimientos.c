#include "stdio.h"
#include "definiciones.h"

#define PEONDIR 10

MOVE *create_move(){
    return (MOVE*)malloc(sizeof(MOVE));
}

void free_move(MOVE *m){
    free(m);
}


MOVE *Generador_Movimientos(TABLERO *t){
    int count = 0;

    ASSERT(CheckBoard(pos))

    if(!t) return NULL;

    
} 


MOVE * Generador_Peones(TABLERO *t, MOVE *m, int *count ){
    int i;
    int side;
    int cas = 0, cas_temp=0;
    int pieza = EMPTY;

    if(!m || !t) return -1;

    side = t->side;

    for(i=0;i<t->pceNum[CAMBIO_LADO*side + wP]){
        cas = 
    }

}