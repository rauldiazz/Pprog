#include "stdio.h"
#include "definiciones.h"

#define PEONDIR 10


MOVE **create_move(){
    return (MOVE**)malloc(sizeof(MOVE*));
}

void free_move(MOVE *m){
    free(m);
}
MOVE* insert_move(int castle, int from, int to, int pieza, int captura, int corona, int paso){
    MOVE* m;
    m->castle = castle;
    m->from = from;
    m->piezas[0] = pieza;
    m->piezas[1] = captura;
    m->piezas[2] = corona;
    m->paso = paso;
    return m;
}


MOVE ** AddMove_K (MOVE **m,  int *count, int castle, int from, int to, int pieza, int captura, int corona, int paso){

    m = realloc(*m, *count +1);
        if (!m) return NULL;
        m[*count] = insert_move(castle,from, to, pieza, captura, corona,paso);
        *count++;
}


MOVE** add_reycaballo(TABLERO *t, MOVE **m, int *count){
    int i,j;
    int dir;
    int side;
    int cas = 0, cas_temp=0;
    int cas_aux;
    int pce_cas_aux;
    int pieza = EMPTY;
    short flag = 1;
    int dircaballo[8]={-8,-19,-21,-12,8,19,21,12};
    int dirrey[8]={-1,-10,1,10,-9,-11,9,11};
    
    if(!m || !t) return -1;

	
    //bucle caballo blanco

	for(i=0; i< t->pceNum[wN]&&flag==1; i++) {
			cas = t->pList[wN][i];
			ASSERT(SqOnBoard(cas));
			
			for(j = 0; j < 8; ++j) {
				dir = dircaballo[j];
				cas_aux = cas + dir;
				
				if(!SQOFFBOARD(cas_aux)) {	
                    pce_cas_aux=t->pieces[cas_aux];
                    if(pce_cas_aux==EMPTY || pce_cas_aux==bP || pce_cas_aux==bN || pce_cas_aux== bB || pce_cas_aux== bR || pce_cas_aux==bQ || pce_cas_aux==bK){
                        m=AddMove_K(m, count, 0, cas,cas_aux, wN, pce_cas_aux, 0, 0);
                        if(!m)flag =0;
				}
                }
				
			}
		}


    //bucle caballo negro

    for(i=0; i< t->pceNum[bN]&&flag==1; i++) {
			cas = t->pList[bN][i];
			ASSERT(cas!=OFFBOARD);
			
			for(j = 0; j < 8; ++j) {
				dir = dircaballo[j];
				cas_aux = cas + dir;
				
				if(cas_aux!=OFFBOARD) {	
                    pce_cas_aux=t->pieces[cas_aux];
                    if(pce_cas_aux==EMPTY || pce_cas_aux==wP || pce_cas_aux==wN || pce_cas_aux== wB || pce_cas_aux== wR || pce_cas_aux==wQ || pce_cas_aux==wK){
                        m=AddMove_K(m, count, 0, cas,cas_aux, bN, pce_cas_aux, 0, 0);
                        if(!m)flag =0;
				}
				}
				
			}
		}

        //bucle rey blanco

        for(i=0; i< t->pceNum[wK]&&flag==1; i++) {
			cas = t->pList[wK][i];
			ASSERT(cas!=OFFBOARD);
			
			for(j = 0; j < 8; ++j) {
				dir = dirrey[j];
				cas_aux = cas + dir;
                pce_cas_aux=t->pieces[cas_aux];
				
				if(cas_aux!=OFFBOARD && SqAttacked(cas_aux, BLACK,t)==FALSE) {	
                    if(pce_cas_aux==EMPTY || pce_cas_aux==bP || pce_cas_aux==bN || pce_cas_aux== bB || pce_cas_aux== bR || pce_cas_aux==bQ || pce_cas_aux==bK){
                        m=AddMove_K(m, count, 0, cas,cas_aux, wN, pce_cas_aux, 0, 0);
                        if(!m)flag =0;
				}
            }
				
			}
		}

        //bucle rey negro
        
           for(i=0; i< t->pceNum[bK]&&flag==1; i++) {
			cas = t->pList[bK][i];
			ASSERT(cas!=OFFBOARD);
			
			for(j = 0; j < 8; ++j) {
				dir = dirrey[j];
				cas_aux = cas + dir;
                pce_cas_aux=t->pieces[cas_aux];
				
				if(cas_aux!=OFFBOARD && SqAttacked(cas_aux, BLACK,t)==FALSE) {	
                    if(pce_cas_aux==EMPTY || pce_cas_aux==wP || pce_cas_aux==wN || pce_cas_aux== wB || pce_cas_aux== wR || pce_cas_aux==wQ || pce_cas_aux==wK){
                        m=AddMove_K(m, count, 0, cas,cas_aux, wN, pce_cas_aux, 0, 0);
                        if(!m)flag =0;
				}
            }
				
			}
		}
				
		
		

}