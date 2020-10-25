#include "stdio.h"
#include "definiciones.h"

#define PEONDIR 10


MOVE *create_move(){
    return (MOVE*)malloc(sizeof(MOVE));
}

void free_move(MOVE *m){
    free(m);
}
MOVE insert_move(int castle, int from, int to, int pieza, int captura, int corona, int paso){
    MOVE m;
    m.castle = castle;
    m.from = from;
    m.piezas[0] = pieza;
    m.piezas[1] = captura;
    m.piezas[2] = corona;
    m.paso = paso;
    return m;
}

MOVE *Generador_Movimientos(TABLERO *t){
    int count = 0;

    ASSERT(CheckBoard(t));

    if(!t) return NULL;

    
} 

MOVE * AddMovePeon (MOVE *m,  int *count, int cas, int to, int to2, int captura, int side, int paso){
    if (!m) return NULL;
    if(Cas_Fila(cas) == FILA_7 - side*5){
        m = realloc(m,*count + 4);
        if (!m) return NULL;
        m[*count] = insert_move(EMPTY,cas, to,CAMBIO_LADO*side + wP, captura, CAMBIO_LADO*side + wN, paso);
        *count ++;
        m[*count] = insert_move(EMPTY,cas, to,CAMBIO_LADO*side + wP, captura, CAMBIO_LADO*side + wB, paso);
        *count ++;
        m[*count] = insert_move(EMPTY,cas, to,CAMBIO_LADO*side + wP, captura, CAMBIO_LADO*side + wR, paso);
        *count ++;
        m[*count] = insert_move(EMPTY,cas, to,CAMBIO_LADO*side + wP, captura, CAMBIO_LADO*side + wQ, paso);
        *count ++;
    }
    else if (Cas_Fila(cas) == FILA_2 +side*5 && captura == EMPTY && to2 == EMPTY){
        m = realloc(m, *count + 2);
        if (!m) return NULL;
        m[*count] = insert_move(EMPTY,cas, to,CAMBIO_LADO*side + wP, captura, EMPTY, paso);
        *count++;
        m[*count] = insert_move(EMPTY,cas, to + 10 - 20*side,CAMBIO_LADO*side + wP, captura, EMPTY, paso);
        count++;
    }
    else{
        m = realloc(m, *count +1);
        if (!m) return NULL;
        m[*count] = insert_move(EMPTY,cas, to,CAMBIO_LADO*side + wP, captura, EMPTY, paso);
        *count++;
    }
    return m;
}


MOVE * Generador_Peones(TABLERO *t, MOVE *m, int *count ){
    MOVE mt;
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
        if(pieza != EMPTY && pieza - (2*side*pieza) < CAMBIO_LADO - 2*side*CAMBIO_LADO && flag == 1){
            m = AddMovePeon(m,count, cas, cas +11 - 20*side, EMPTY,pieza, side, EMPTY);
            if (!m) flag = 0;
        }
        pieza = t->pieces[cas +9 - 20*side];
        if (pieza != EMPTY && pieza - (2*side*pieza) < CAMBIO_LADO - 2*side*CAMBIO_LADO && flag == 1){
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

int print_moves(MOVE *m, int count){
    int i;
    int col, fila;
    MOVE mt;

    if (!m) return -1;

    for(i=0; i<count; i++){
        printf("Jugada %d: ",i+1);
        mt = m[i];
        col = Cas_Col(mt.from);
        fila = Cas_Fila(mt.from);
        printf("%c%d", 'a'+col, fila +1);
        col = Cas_Col(mt.to);
        fila = Cas_Fila(mt.to);
        printf("%c%d", 'a'+col, fila +1);

        if(mt.piezas[2] != EMPTY){
            switch (mt.piezas[2])
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