#include "stdio.h"
#include "definiciones.h"
#define SQOFFBOARD(sq) (FILAsBrd[sq]==OFFBOARD)
#define PEONDIR 10

char PceChar2[] = ".PNBRQKPNBRQK";

MOVE *create_move(){
    return (MOVE*)malloc(sizeof(MOVE));
}

void free_move(MOVE *m){
    free(m);
    return;
}

MOVE *move_copy(MOVE*m){
    MOVE *copy;
    if(!m)return NULL;
    if(!(copy = create_move())) return NULL;
    copy->castle=m->castle;
    copy->from = m->from;
    copy->paso = m->paso;
    copy->to = m->to;
    copy->piezas[0] = m->piezas[0];
    copy->piezas[1] = m->piezas[1];
    copy->piezas[2] = m->piezas[2];
    return copy;
}

void free_UNDO(S_UNDO * u){
    if(u){
        if(u->jugada) free_move(u->jugada);
        if(u->fen) free(u->fen);
        free(u);
    } 
    return;
}

S_UNDO *create_UNDO (MOVE *jugada){
    S_UNDO *u;
    if(!(u= (S_UNDO*)malloc(sizeof(S_UNDO)))) return NULL;
    if(!(u->jugada = move_copy(jugada))){
        free_UNDO(u);
        return NULL;
    }
    return u;
}

MOVE* insert_move(int castle, int from, int to, int pieza, int captura, int corona, int paso){
    MOVE  *m;
    m = create_move();
    //printf("Después de create move\n");
    if (!m) return NULL;        
    m->castle = castle;
    m->from = from;
    m->to =to;
    //printf("antes de piezas\n");
    m->piezas[0] = pieza;
    m->piezas[1] = captura;
    m->piezas[2] = corona;
    //printf("Despues de piezass\n");
    m->paso = paso;
    //printf("ANtes de return \n");
    return m;
}

MOVE **Generador_Movimientos(TABLERO *t, int *count){
    MOVE **m;

    ASSERT(CheckBoard(t));

    if(!t) return NULL;

    m = (MOVE**) malloc(sizeof(MOVE*));
    m[0] = insert_move(0,A1,A1,0,0,0,0);
    *count = 1;

    
    m = Generador_Enroques(t, m, count);
    m = Generador_Peones(t, m, count);
    m = Generador_Slide(t, m, count);
    m = Generador_RC(t, m, count);

    return m;
    
} 

MOVE ** AddMovePeon (MOVE **m,  int *count, int cas, int to, int to2, int captura, int side, int paso){
    if (!m) return NULL;
    //printf("Empezamos Add Move\n");
    if(Cas_Fila(cas) == FILA_7 - side*5){
        
        m = realloc(m,(*count + 4)*sizeof(MOVE*));
        if (!m) return NULL;
        m[*count] = insert_move(EMPTY,cas, to,CAMBIO_LADO*side + wP, captura, CAMBIO_LADO*side + wN, paso);
        (*count) ++;
        m[*count] = insert_move(EMPTY,cas, to,CAMBIO_LADO*side + wP, captura, CAMBIO_LADO*side + wB, paso);
        (*count) ++;
        m[*count] = insert_move(EMPTY,cas, to,CAMBIO_LADO*side + wP, captura, CAMBIO_LADO*side + wR, paso);
        (*count) ++;
        m[*count] = insert_move(EMPTY,cas, to,CAMBIO_LADO*side + wP, captura, CAMBIO_LADO*side + wQ, paso);
        (*count) ++;
        //printf("primer if\n");
    }
    else if (Cas_Fila(cas) == FILA_2 +side*5 && captura == EMPTY && to2 == EMPTY){
       // printf("Antes de realloc\n");
        m = realloc(m, (*count + 2)*sizeof(MOVE*));
        if (!m) return NULL;
        //printf("después de realloc\n");
        m[*count] = insert_move(EMPTY,cas, to,CAMBIO_LADO*side + wP, captura, EMPTY, paso);
        //printf("Después de insertmove");
        (*count)++;
        m[*count] = insert_move(EMPTY,cas, to + 10 - 20*side,CAMBIO_LADO*side + wP, captura, EMPTY, paso);
        (*count)++;
        //printf("segundo if\n");
    }
    else{
        m = realloc(m, (*count +1)*sizeof(MOVE*));
        if (!m) return NULL;
        m[*count] = insert_move(EMPTY,cas, to,CAMBIO_LADO*side + wP, captura, EMPTY, paso);
        (*count)++;
        //printf("else\n");
    }
    return m;
}


MOVE ** Generador_Peones(TABLERO *t, MOVE **m, int *count ){

    int i;
    int side;
    int cas = 0, cas_temp=0;
    int pieza = EMPTY;
    short flag = 1;

    if(!m || !t) return NULL;

    side = t->side;

    ASSERT(CheckBoard(t));

    //Las correciones donde hay *side sirven para escoger, dependiendo del lado al que le toque, escoger las características correctas
    for(i=0;i<t->pceNum[CAMBIO_LADO*side + wP]&& flag == 1;i++){
        cas = t->pList[CAMBIO_LADO*side + wP][i];
        ASSERT(cas != OFFBOARD);

        if(t->pieces[cas + 10 - 20*side] == EMPTY){
            m = AddMovePeon(m, count, cas, cas + 10 - 20*side, t->pieces[cas + 20 - 40*side],EMPTY,side,EMPTY);
            if (!m) flag = 0;
        }
        pieza = t->pieces[cas +11 - 20*side];
        if(pieza != EMPTY && pieza != OFFBOARD && pieza != NO_SQ && pieza - (2*side*pieza) > CAMBIO_LADO - 2*side*CAMBIO_LADO && flag == 1){
            m = AddMovePeon(m,count, cas, cas +11 - 20*side, EMPTY,pieza, side, EMPTY);
            if (!m) flag = 0;
        }
        pieza = t->pieces[cas +9 - 20*side];
        if (pieza != EMPTY && pieza != OFFBOARD && pieza != NO_SQ && pieza - (2*side*pieza) > CAMBIO_LADO - 2*side*CAMBIO_LADO && flag == 1){
            m = AddMovePeon(m,count, cas, cas +9 - 20*side, EMPTY,pieza, side, EMPTY);
            if (!m) flag = 0;
        }

        if (t->AlPaso == cas +9 - 20*side && flag == 1 && t->AlPaso != NO_SQ){
            m = AddMovePeon(m,count, cas, cas +9 - 20*side, EMPTY,-(CAMBIO_LADO*side) + bP, side, cas +9 - 20*side);
            if (!m) flag = 0;
        }
        else if (t->AlPaso == cas +11 - 20*side && flag == 1 && t->AlPaso != NO_SQ){
            m = AddMovePeon(m,count, cas, cas +11 - 20*side, EMPTY,-(CAMBIO_LADO*side) + bP, side,cas +11 - 20*side);
            if (!m) flag = 0;
        }
        
    }
    if (flag == 0){
        return NULL;
    }
    return m;
}


MOVE** Generador_RC(TABLERO *t, MOVE **m, int *count){
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
    
    if(!m || !t) return NULL;

	side=t->side;

    ASSERT(CheckBoard(t));
   
   
    //bucle caballo blanco

   if(side==WHITE){
	for(i=0; i< t->pceNum[2] ; ++i) {
			
            cas = t->pList[2][i];
            
			
			for(j = 0; j < 8;j++) {
				dir = dircaballo[j];
				cas_aux = cas + dir;
                

				
				if(!SQOFFBOARD(cas_aux)) {	
                     
                    pce_cas_aux=t->pieces[cas_aux];
                    if(pce_cas_aux==EMPTY || pce_cas_aux==bP || pce_cas_aux==bN || pce_cas_aux== bB || pce_cas_aux== bR || pce_cas_aux==bQ || pce_cas_aux==bK){
                       
                        m = realloc(m, (*count +1)*sizeof(MOVE*));
                        if (!m) return NULL;
                      

                        m[*count] = insert_move(EMPTY,cas,cas_aux,2, pce_cas_aux, EMPTY, EMPTY);
                        (*count)++;
                       
				}
                }
				
			}
		}
   }


    //bucle caballo negro
     if(side==BLACK){

    for(i=0; i< t->pceNum[bN]; i++) {
			cas = t->pList[bN][i];
			
			
			for(j = 0; j < 8; ++j) {
				dir = dircaballo[j];
				cas_aux = cas + dir;
				
				if((!SQOFFBOARD(cas_aux))) {	
                    pce_cas_aux=t->pieces[cas_aux];
                    if(pce_cas_aux==EMPTY || pce_cas_aux==wP || pce_cas_aux==wN || pce_cas_aux== wB || pce_cas_aux== wR || pce_cas_aux==wQ || pce_cas_aux==wK){
                        m = realloc(m, (*count +1)*sizeof(MOVE*));
                        if (!m) return NULL;
                      

                        m[*count] = insert_move(EMPTY,cas,cas_aux,bN, pce_cas_aux, EMPTY, EMPTY);
                        (*count)++;
				}
				}
				
			}
		}
     }

        //bucle rey blanco
      if(side==WHITE){
        for(i=0; i< t->pceNum[wK]; i++) {
			cas = t->pList[wK][i];
			
			
			for(j = 0; j < 8; ++j) {
				dir = dirrey[j];
				cas_aux = cas + dir;
                pce_cas_aux=t->pieces[cas_aux];
				
				if((!SQOFFBOARD(cas_aux))) {	
                    if(pce_cas_aux==EMPTY || pce_cas_aux==bP || pce_cas_aux==bN || pce_cas_aux== bB || pce_cas_aux== bR || pce_cas_aux==bQ || pce_cas_aux==bK){
                        m = realloc(m, (*count +1)*sizeof(MOVE*));
                        if (!m) return NULL;
                      

                        m[*count] = insert_move(EMPTY,cas,cas_aux,wK, pce_cas_aux, EMPTY, EMPTY);
                        (*count)++;
				}
            }
				
			}
		}
      }

        //bucle rey negro
         if(side==BLACK){
           for(i=0; i< t->pceNum[bK]; i++) {
			cas = t->pList[bK][i];
		
			
			for(j = 0; j < 8; ++j) {
				dir = dirrey[j];
				cas_aux = cas + dir;
                pce_cas_aux=t->pieces[cas_aux];
				
				if((!SQOFFBOARD(cas_aux))) {	
                    if(pce_cas_aux==EMPTY || pce_cas_aux==wP || pce_cas_aux==wN || pce_cas_aux== wB || pce_cas_aux== wR || pce_cas_aux==wQ || pce_cas_aux==wK){
                        m = realloc(m, (*count +1)*sizeof(MOVE*));
                        if (!m) return NULL;
                      

                        m[*count] = insert_move(EMPTY,cas,cas_aux,bK, pce_cas_aux, EMPTY, EMPTY);
                        (*count)++;
				}
            }
				
			}
		}
         }
			
		
		
return m;
}

MOVE ** Generador_Enroques(TABLERO *t, MOVE **m, int *count ){

    int side;
    int sq, entresq;
    int i, flag=1;
    int aux;

    if(!t||!m)return NULL;

    ASSERT(CheckBoard(t));

    side=t->side;
    
    aux=t->enroque;
    

    if(side==WHITE){

        sq = t->pList[wK][0];

        if(SqAttacked(sq, BLACK,t))return m;

        if((aux%2)==1){
            aux-=1;
            for(i=1;i<=2&&flag==1;i++){

                entresq=sq+i;
                flag=((t->pieces[entresq] == EMPTY && !SqAttacked(entresq, BLACK,t)));
                
            }
            if(flag==1){

                m = realloc(m,(*count + 1)*sizeof(MOVE*));
				m[*count] = insert_move(WKCA,EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY);
                (*count) ++;

            }

        }
        flag=1;
        aux=aux/2;
        if(aux%2==1){
            for(i=1;i<=2&&flag==1;i++){

                entresq=sq-i;
                flag=((t->pieces[entresq] == EMPTY && !SqAttacked(entresq, BLACK,t)));
                
            }
            if(flag == 1) flag = (t->pieces[sq-3] == EMPTY);
            if(flag==1){

                m = realloc(m,(*count + 1)*sizeof(MOVE*));
				m[*count] = insert_move(WQCA,EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY);
                (*count) ++;

            }

        }

    }
    if(side==BLACK){
        aux=t->enroque;
        

        sq = t->pList[bK][0];

        if(SqAttacked(sq, WHITE,t))return m;


        aux-=BQCA;
        if((aux)>=0){
            for(i=1;i<=2&&flag==1;i++){

                entresq=sq-i;
                flag=((t->pieces[entresq] == EMPTY && !SqAttacked(entresq, WHITE,t)));

            }
            if(flag == 1) flag = (t->pieces[sq-3] == EMPTY);
            if(flag==1){

                m = realloc(m,(*count + 1)*sizeof(MOVE*));
				m[*count] = insert_move(BQCA,EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY);
                (*count) ++;
                

            }

        }

        flag=1;
        if(aux<0)aux=t->enroque;

        aux-=BKCA;

        if(aux>=0){
            for(i=1;i<=2&&flag==1;i++){

                entresq=sq+i;
                flag=((t->pieces[entresq] == EMPTY && !SqAttacked(entresq, WHITE,t)));

            }
            if(flag==1){

                m = realloc(m,(*count + 1)*sizeof(MOVE*));
				m[*count] = insert_move(BKCA,EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY);
                (*count) ++;

            }

        }

    }

    return m;


}



MOVE ** Generador_Slide(TABLERO *t, MOVE **m, int *count ){

    ASSERT(CheckBoard(t));

    int pceIndex, pce, pceNum, side, index, sq, t_sq, dir;
    int PceDirSlide[8][8] = {
	{0,0,0,0,0,0,0,0},
	{ -9, -11, 11, 9, 0, 0, 0, 0 },
	{ -1, -10,	1, 10, 0, 0, 0, 0 },
	{ -1, -10,	1, 10, -9, -11, 11, 9 },
    {0,0,0,0,0,0,0,0},
	{ -9, -11, 11, 9, 0, 0, 0, 0 },
	{ -1, -10,	1, 10, 0, 0, 0, 0 },
	{ -1, -10,	1, 10, -9, -11, 11, 9 }

    };


    int NumDirSlide[13] = {
    0,0,0,4,4,8,0,0,0,4,4,8,0
    };

    int LoopSlidePce[8] = {
    wB, wR, wQ, 0, bB, bR, bQ, 0
    };

    int LoopSlideIndex[2] = { 0, 4 };


    if(!t||!m)return NULL;

    side=t->side;

	pceIndex = LoopSlideIndex[side];
	pce = LoopSlidePce[pceIndex];
    pceIndex++;
	while( pce != 0) {	
		
		for(pceNum = 0; pceNum < t->pceNum[pce]; ++pceNum) {
			sq = t->pList[pce][pceNum];
			if(SQOFFBOARD(sq)) return NULL;
			
			for(index = 0; index < NumDirSlide[pce]; ++index) {
				dir = PceDirSlide[pceIndex][index];
				t_sq = sq + dir;
				
				while(!SQOFFBOARD(t_sq)) {				
					
					if(t->pieces[t_sq] != EMPTY) {
						if( pieceColour(t->pieces[t_sq]) !=side) {
                            
                            m = realloc(m,(*count + 1)*sizeof(MOVE*));
							m[*count] = insert_move(EMPTY,sq, t_sq, pce, t->pieces[t_sq], EMPTY, EMPTY);
                            (*count) ++;
						}
						break;
					}	
					m = realloc(m,(*count + 1)*sizeof(MOVE*));
					m[*count] = insert_move(EMPTY,sq, t_sq, pce, EMPTY, EMPTY, EMPTY);
                    
                    (*count) ++;
					t_sq += dir;
				}
			}
		}
		
		pce = LoopSlidePce[pceIndex++];

}
    return m;

}

int PrintMove(MOVE *mt){
    
    int col, fila;

    

    if (!mt) return -1;
    
        //primero ver el enroque
        //Después vemos las piezas:
        if(mt->piezas[0] == wP || mt->piezas[0] == bP){
            col = Cas_Col(mt->from);
            fila = Cas_Fila(mt->from);
            printf("%c%d", 'a'+col, fila +1);

            //Captura
            if(mt->piezas[1] != EMPTY && mt->piezas[1] != OFFBOARD) printf("x");

            col = Cas_Col(mt->to);
            fila = Cas_Fila(mt->to);
            printf("%c%d", 'a'+col, fila +1);

            //Coronación
            if(mt->piezas[2] != EMPTY){
                switch (mt->piezas[2])
                {
                case wN:
                case bN:
                    printf("=N");
                    break;
                case wB:
                case bB:
                    printf("=B");
                    break;
                case wR:
                case bR:
                    printf("=R");
                    break;
                case wQ:
                case bQ:
                    printf("=Q");
                    break;
                default:
                    break;
                }
            }
        }
        else{
            if(mt->castle==EMPTY){
            printf("%c", PceChar2[mt->piezas[0]]);
            col = Cas_Col(mt->from);
            fila = Cas_Fila(mt->from);
            printf("%c%d", 'a'+col, fila +1);

            //Captura
            if(mt->piezas[1] != EMPTY && mt->piezas[1] != OFFBOARD) printf("x");

            col = Cas_Col(mt->to);
            fila = Cas_Fila(mt->to);
            printf("%c%d", 'a'+col, fila +1);
            }
            else{
                if(mt->castle==WKCA)printf("WKCA");
                else if(mt->castle==WQCA)printf("WQCA");
                else if(mt->castle==BKCA)printf("BKCA");
                else if(mt->castle==BQCA)printf("BQCA");

            }
            
        }
    }


int print_moves(MOVE **m, int count){
    int i;
    int col, fila;
    MOVE *mt;

    if (!m) return -1;
    
    for(i=0; i<count; i++){
        printf("Jugada %d: ",i);
        mt = m[i];
        //primero ver el enroque
        //Después vemos las piezas:
        if(mt->piezas[0] == wP || mt->piezas[0] == bP){
            col = Cas_Col(mt->from);
            fila = Cas_Fila(mt->from);
            printf("%c%d", 'a'+col, fila +1);

            //Captura
            if(mt->piezas[1] != EMPTY && mt->piezas[1] != OFFBOARD) printf("x");

            col = Cas_Col(mt->to);
            fila = Cas_Fila(mt->to);
            printf("%c%d", 'a'+col, fila +1);

            //Coronación
            if(mt->piezas[2] != EMPTY){
                switch (mt->piezas[2])
                {
                case wN:
                case bN:
                    printf("=N");
                    break;
                case wB:
                case bB:
                    printf("=B");
                    break;
                case wR:
                case bR:
                    printf("=R");
                    break;
                case wQ:
                case bQ:
                    printf("=Q");
                    break;
                default:
                    break;
                }
            }
        }
        else{
            if(mt->castle==EMPTY){
            printf("%c", PceChar2[mt->piezas[0]]);
            col = Cas_Col(mt->from);
            fila = Cas_Fila(mt->from);
            printf("%c%d", 'a'+col, fila +1);

            //Captura
            if(mt->piezas[1] != EMPTY && mt->piezas[1] != OFFBOARD) printf("x");

            col = Cas_Col(mt->to);
            fila = Cas_Fila(mt->to);
            printf("%c%d", 'a'+col, fila +1);
            }
            else{
                if(mt->castle==WKCA)printf("WKCA");
                else if(mt->castle==WQCA)printf("WQCA");
                else if(mt->castle==BKCA)printf("BKCA");
                else if(mt->castle==BQCA)printf("BQCA");

            }
            
        }
        printf("\n");
    }
}

/*Introducir una jugada que sea válida o jaque*/
int HacerJugada(TABLERO *t,MOVE *m){
    S_UNDO *u;
    int aux,ksq;



    if(!t||!m)return FALSE;
    aux=t->enroque;


    if(!(u = create_UNDO(m))) return FALSE;
    u->AlPaso = t->AlPaso;
    u->fiftyMove = t->fiftyMove;
    u->enroque = t->enroque;
    u->fen = EscribirFen(t);
    if(m->castle!=EMPTY){

        if(m->castle==WKCA){

            t->pieces[E1]=EMPTY;
            t->pieces[H1]=EMPTY;
            t->pieces[G1]=wK;
            t->pieces[F1]=wR;
            if(aux%2==1){

                t->enroque-=1;
                aux-=1;
            }
            aux/=2;
            if(aux%2==1){
                t->enroque-=2;
            }
        }
        if(m->castle==WQCA){

            t->pieces[E1]=EMPTY;
            t->pieces[A1]=EMPTY;
            t->pieces[C1]=wK;
            t->pieces[D1]=wR;
            if(aux%2==1){

                t->enroque-=1;
                aux-=1;
            }
            aux/=2;
            if(aux%2==1){
                t->enroque-=2;
            }
        }
        if(m->castle==BKCA){

            t->pieces[E8]=EMPTY;
            t->pieces[H8]=EMPTY;
            t->pieces[G8]=bK;
            t->pieces[F8]=bR;
            aux-=BQCA;
            if(aux>=0){
                t->enroque-=8;
            }
            else if(aux<0)aux=t->enroque;

            aux-=BKCA;
            if(aux>=0)t->enroque-=4;
        }
        if(m->castle==BQCA){

            t->pieces[E8]=EMPTY;
            t->pieces[A8]=EMPTY;
            t->pieces[C8]=bK;
            t->pieces[D8]=bR;
            aux-=BQCA;
            if(aux>=0){
                t->enroque-=8;
            }
            else if(aux<0)aux=t->enroque;

            aux-=BKCA;
            if(aux>=0)t->enroque-=4;
    }

    }

    else{
            t->pieces[m->from] = EMPTY;
            t->pieces[m->to] = m->piezas[0];
            if(m->piezas[0]==wK || m->piezas[0]== bK) ksq = m->to;
            else ksq = t->KingSq[t->side];
            if(SqAttacked(ksq ,1 - t->side,t)){

                t->pieces[m->from] = t->pieces[m->to];
                t->pieces[m->to] = m->piezas[1];
                free_UNDO(u);
                return FALSE;
            }

            if (m->piezas[2] != EMPTY) t->pieces[m->to] = m->piezas[2];
            if (m->paso!= EMPTY) t->pieces[m->to -10 +20*t->side]=EMPTY;
    } 

    UpdateListsMaterial(t);


    CheckBoard(t);
    
    if(t->side==WHITE){
        aux-=BQCA;
        if(m->to==A8){
                if(aux>=0){                   
                    t->enroque-=8;
                }

                else if(aux<0)aux=t->enroque;

            }
            else if(m->to==H8){
                if(aux<0)aux=t->enroque;

                aux-=BKCA;
                if(aux>=0)t->enroque-=4;
             }
        aux = t->enroque;
        if(t->pieces[m->to]==wK){

            if(aux%2==1){

                t->enroque-=1;
                aux-=1;
            }
            aux/=2;
            if(aux%2==1){
                t->enroque-=2;
            }

        }
        else if(t->pieces[m->to]==wR){

            if(m->from==H1){

                if(aux%2==1)t->enroque-=1;
            }
            else if(m->from==A1){

                aux=(aux-(aux%2))/2;

                if(aux%2==1) t->enroque-=2;                    
            }
        }
    }
    else if(t->side==BLACK){
        if(m->to==H1){

                if(aux%2==1)t->enroque-=1;
            }
            else if(m->to==A1){

                aux=(aux-(aux%2))/2;

                if(aux%2==1) t->enroque-=2;                    
            }
        aux = t->enroque;
        if(t->pieces[m->to]==bK){
            //printf("Se ha movido el negro\n");
            aux-=BQCA;
            if(aux>=0){
                t->enroque-=8;
            }
            else if(aux<0)aux=t->enroque;

            aux-=BKCA;
            if(aux>=0)t->enroque-=4;
        }

        else if(t->pieces[m->to]==bR){
            //printf("Se ha movido el rey negro\n");
            aux-=BQCA;
            if(m->from==A8){
                //printf("Se ha movido la torre de A8\n");
                if(aux>=0){                   
                    t->enroque-=8;
                }

                else if(aux<0)aux=t->enroque;

            }
            else if(m->from==H8){
                if(aux<0)aux=t->enroque;
                //printf("Se ha movido la torre de H8\n");
                aux-=BKCA;
                if(aux>=0)t->enroque-=4;
            }
        }

    }



    if(m->piezas[0] == wP + CAMBIO_LADO*t->side && (m->to - m->from) == 20 -40*t->side){
        t->AlPaso = m->to -10 +20*t->side;
    }
    else{
        t->AlPaso = NO_SQ;
    }

    if(m->piezas[1] != EMPTY && m->piezas[0] != wP + CAMBIO_LADO*t->side){
        t->fiftyMove++;
    }
    else{
        t->fiftyMove = 0;
    }
    t->j_real++;


    t->history[t->histcont] = u;
    t->histcont++;
    t->side = 1- t->side;
    return TRUE;

}




void DeshacerJugada(TABLERO *t) {
    int col,pce,flag,i,captura,promo;
    MOVE * move;
	int from;
    int to;
	
    ASSERT(CheckBoard(t));
	
	t->histcont--; //histcont  hisply
    t->j_real--;  //j_real ply
	
    move = t->history[t->histcont]->jugada;
    from = move->from;
    to = move->to;	
	


    t->enroque = t->history[t->histcont]->enroque;
    t->fiftyMove = t->history[t->histcont]->fiftyMove;
    t->AlPaso = t->history[t->histcont]->AlPaso;



    
    t->side=1-t->side;
	
	if(move->paso!=EMPTY&&move->paso!= OFFBOARD && move->paso != NO_SQ) {
        if(t->side == WHITE) {
            t->pieces[to-10]=bP;
        } else {
            t->pieces[to+10]=wP;
        }
    } else if(move->castle==WKCA  || move->castle==BKCA || move->castle==WQCA || move->castle==BQCA) {
        switch(move->castle) {
           
            
            case WKCA:  
                t->pieces[E1]=wK;
                t->pieces[H1]=wR;
                t->pieces[G1]=EMPTY;
                t->pieces[F1]=EMPTY;
                break;
            case WQCA: 
                t->pieces[E1]=wK;
                t->pieces[A1]=wR;
                t->pieces[C1]=EMPTY;
                t->pieces[D1]=EMPTY;
                break;
            case BKCA: 
                t->pieces[E8]=bK;
                t->pieces[H8]=bR;
                t->pieces[G8]=EMPTY;
                t->pieces[F8]=EMPTY;
                break;
            case BQCA: 
                t->pieces[E8]=bK;
                t->pieces[A8]=bR;
                t->pieces[C8]=EMPTY;
                t->pieces[D8]=EMPTY;

                break;
            default: ASSERT(FALSE);     
                break;
        }
    }
	
	
	//move piece to-from
	pce = t->pieces[to];	
    t->pieces[to] = EMPTY;
	t->pieces[from] = pce;
     
    
	
	captura = move->piezas[1];
    if(captura != EMPTY && (move->paso == EMPTY || move->paso == NO_SQ)) {
        t->pieces[to] = captura;
    }

    promo=move->piezas[2];
	
	if(promo != EMPTY)   {
        
        t->pieces[from]=EMPTY;
        t->pieces[from] = CAMBIO_LADO*t->side +wP;

    }    
    free_UNDO(t->history[t->histcont]);
    UpdateListsMaterial(t);
  
	
    ASSERT(CheckBoard(t));
    return;

}