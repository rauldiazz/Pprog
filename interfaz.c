#include "stdio.h"
#include <string.h>
#include "definiciones.h"


int is_Valid(MOVE *m,TABLERO *t){
    int count = 0,i,flag = FALSE;
    MOVE **array;

    if(!m||!t) return FALSE;

    array = Generador_Movimientos(t,&count);

    if(!array) return FALSE;

    for(i=0;i<count && flag == FALSE;i++){
        flag = move_cmp(array[i],m);
    }

    for(i=0;i<count;i++){
        free_move(array[i]);
    }
    free(array);
    printf("Antes de return isvalid\n");
    return flag;

}

MOVE *LeerMovimiento(char *entrada, TABLERO *t){
    MOVE *m;
    char *aux = entrada;
    int from,to,pieza= EMPTY;
    int captura = EMPTY,corona=EMPTY;
    int paso =EMPTY;

    if(!aux || !t) return NULL;


    if(*aux == '0'){
        if(strcmp("0-0",aux) == 0 || strcmp("0-0\n",aux) == 0){
            m=insert_move(BKCA - t->side*3, EMPTY,EMPTY,EMPTY, EMPTY,EMPTY, EMPTY);
            if(is_Valid(m,t)) return m;
            free_move(m);
            return NULL;
        }
        else if(strcmp("0-0-0",aux) == 0 || strcmp("0-0-0\n",aux) == 0){
            m=insert_move(BQCA - t->side*6, EMPTY,EMPTY,EMPTY, EMPTY,EMPTY, EMPTY);
            if(is_Valid(m,t)) return m;
            free_move(m);
            return NULL;
        }
        else return NULL;
    }
    else if (*aux >= 'a' && *aux <= 'h'){
        pieza = wP +t->side*(CAMBIO_LADO);
        printf("es una jugada de peon\n");
    }

    switch (*aux)
    {
        case 'N': pieza = wN + t->side*(CAMBIO_LADO); break;
        case 'B': pieza = wB + t->side*(CAMBIO_LADO); break;
        case 'R': pieza = wR + t->side*(CAMBIO_LADO); break;
        case 'Q': pieza = wQ + t->side*(CAMBIO_LADO); break;
        
        default:
            break;
    }

    printf("random printf,pieza es %d\n",pieza);
    if(pieza == EMPTY) return NULL;
    if(*(aux+1) <= '1' || *(aux+1) >= '8') return NULL;
    //printf("la columna es %d, y la fila es %d, FECAS es %d\n",*aux-'a',*(aux+1)-'1', FCCAS(4,1));
    from = FCCAS(*aux-'a',*(aux+1)-'1');
    aux++;
    //printf("from es %d, e2 es %d\n",from,E2);
    if(from < 0 || from >= 120) return NULL;
    if(t->pieces[from] == OFFBOARD) return NULL;
    aux++;
    printf("Ha pasado from\n");
    if(*aux == 'x'){
        captura = 1;
        aux++;
    }
    if (*aux <= 'a' || *aux >= 'h') return NULL;
    if(*(aux+1) <= '1' || *(aux+1) >= '8') return NULL;
    to = FCCAS(*aux-'a', *(aux+1) -'1');
    aux++;
    if(to < 0 || to >= 120) return NULL;
    if(t->pieces[to] == OFFBOARD) return NULL;
    aux++;
    if(*aux == '=' && pieza ==  wP +t->side*(CAMBIO_LADO)){
        aux++;
        switch (*aux){
            case 'N': corona = bN - CAMBIO_LADO*(t->side); break;
            case 'B': corona = bB - CAMBIO_LADO*(t->side); break;
            case 'R': corona = bR - CAMBIO_LADO*(t->side); break;
            case 'Q': corona = bQ - CAMBIO_LADO*(t->side); break;
            default: return NULL;
        }
    }
    if(captura == 1){
        captura = t->pieces[to];
        if(captura == EMPTY){
            captura = bP -CAMBIO_LADO*t->side;
            paso = to;
        }
    }
    printf("Antes de insertmove\n");
    m = insert_move(EMPTY, from,to,pieza, captura,corona, paso);
    PrintMove(m);
    if(is_Valid(m,t))return m;
    free_move(m);
    return NULL;

}

