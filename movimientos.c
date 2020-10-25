#include "stdio.h"
#include "definiciones.h"
#define SQOFFBOARD(sq) (FILAsBrd[sq]==OFFBOARD)
#define PEONDIR 10


MOVE *create_move(){
    return (MOVE*)malloc(sizeof(MOVE));
}

void free_move(MOVE *m){
    free(m);
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

    m = Generador_Peones(t, m, count);
    m = Generador_Slide(t, m, count);

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

    //Las correciones donde hay *side sirven para escoger, dependiendo del lado al que le toque, escoger las características correctas
    for(i=0;i<t->pceNum[CAMBIO_LADO*side + wP]&& flag == 1;i++){
        cas = t->pList[CAMBIO_LADO*side + wP][i];
        ASSERT(cas != OFFBOARD)

        if(t->pieces[cas + 10 - 20*side] == EMPTY){
            m = AddMovePeon(m, count, cas, cas + 10 - 20*side, t->pieces[cas + 20 - 40*side],EMPTY,side,EMPTY);
            if (!m) flag = 0;
        }
        pieza = t->pieces[cas +11 - 20*side];
        if(pieza != EMPTY && pieza != OFFBOARD && pieza - (2*side*pieza) > CAMBIO_LADO - 2*side*CAMBIO_LADO && flag == 1){
            m = AddMovePeon(m,count, cas, cas +11 - 20*side, EMPTY,pieza, side, EMPTY);
            if (!m) flag = 0;
        }
        pieza = t->pieces[cas +9 - 20*side];
        if (pieza != EMPTY && pieza != OFFBOARD && pieza - (2*side*pieza) > CAMBIO_LADO - 2*side*CAMBIO_LADO && flag == 1){
            m = AddMovePeon(m,count, cas, cas +9 - 20*side, EMPTY,pieza, side, EMPTY);
            if (!m) flag = 0;
        }

        if (t->AlPaso == cas +9 - 20*side && flag == 1){
            m = AddMovePeon(m,count, cas, cas +9 - 20*side, EMPTY,-(CAMBIO_LADO*side) + bP, side, cas +9 - 20*side);
            if (!m) flag = 0;
        }
        else if (t->AlPaso == cas +11 - 20*side && flag == 1){
            m = AddMovePeon(m,count, cas, cas +11 - 20*side, EMPTY,-(CAMBIO_LADO*side) + bP, side,cas +11 - 20*side);
            if (!m) flag = 0;
        }
        
    }
    if (flag == 0){
        return NULL;
    }
    return m;
}


MOVE ** Generador_Slide(TABLERO *t, MOVE **m, int *count ){

    ASSERT(CheckBoard(t));

    int pceIndex, pce, pceNum, side, index, sq, t_sq, dir;
    int PceDirSlide[6][8] = {
	
	{ -9, -11, 11, 9, 0, 0, 0, 0 },
	{ -1, -10,	1, 10, 0, 0, 0, 0 },
	{ -1, -10,	1, 10, -9, -11, 11, 9 },
	{ -9, -11, 11, 9, 0, 0, 0, 0 },
	{ -1, -10,	1, 10, 0, 0, 0, 0 },
	{ -1, -10,	1, 10, -9, -11, 11, 9 }

    };


    int NumDirSlide[6] = {
    4,4,8,4,4,8
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
			ASSERT(!SQOFFBOARD(sq));
			
			for(index = 0; index < NumDirSlide[pce]; ++index) {
				dir = PceDirSlide[pce][index];
				t_sq = sq + dir;
				
				while(!SQOFFBOARD(t_sq)) {				
					
					if(t->pieces[t_sq] != EMPTY) {
						if( pieceColour(t->pieces[t_sq]) != side) {
                            
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




int print_moves(MOVE **m, int count){
    int i;
    int col, fila;
    MOVE *mt;

    if (!m) return -1;
    printf("Antes del for, count es %d\n", count);
    for(i=0, printf("EN el bucle\n"); i<count; i++){
        printf("Jugada %d: ",i);
        mt = m[i];
        col = Cas_Col(mt->from);
        fila = Cas_Fila(mt->from);
        printf("%c%d", 'a'+col, fila +1);
        col = Cas_Col(mt->to);
        fila = Cas_Fila(mt->to);
        printf("%c%d", 'a'+col, fila +1);

        if(mt->piezas[2] != EMPTY){
            switch (mt->piezas[2])
            {
            case wN:
            case bN:
                printf("n");
                break;
            case wB:
            case bB:
                printf("b");
                break;
            case wR:
            case bR:
                printf("r");
                break;
            case wQ:
            case bQ:
                printf("q");
                break;
            default:
                break;
            }
        }
        printf("\n");
    }
}