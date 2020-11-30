#include "stdio.h"
#include <string.h>
#include "definiciones.h"


#define MAXSTRJUGADA 16


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
    //printf("Antes de return isvalid\n");
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
        if(strncmp("0-0-0",aux,5) == 0){
            m=insert_move(WQCA + t->side*6, EMPTY,EMPTY,EMPTY, EMPTY,EMPTY, EMPTY);
            if(is_Valid(m,t)) return m;
            free_move(m);
            return NULL;
        }
        else if(strncmp("0-0",aux,3) == 0){
            //printf("Esoty en enroque\n");
            m=insert_move(WKCA + t->side*3, EMPTY,EMPTY,EMPTY, EMPTY,EMPTY, EMPTY);
            if(is_Valid(m,t)) return m;
            free_move(m);
            return NULL;
        }
        else return NULL;
    }
    else if (*aux >= 'a' && *aux <= 'h'){
        pieza = wP +t->side*(CAMBIO_LADO);
      //  printf("es una jugada de peon\n");
    }

    switch (*aux)
    {
        case 'N': pieza = wN + t->side*(CAMBIO_LADO);aux++; break;
        case 'B': pieza = wB + t->side*(CAMBIO_LADO);aux++; break;
        case 'R': pieza = wR + t->side*(CAMBIO_LADO);aux++; break;
        case 'Q': pieza = wQ + t->side*(CAMBIO_LADO);aux++; break;
        case 'K': pieza = wK + t->side*(CAMBIO_LADO);aux++; break;
        
        default:
            break;
    }

    //printf("random printf,pieza es %d\n",pieza);
    if(pieza == EMPTY) return NULL;
    if ((*aux) < 'a' || (*aux) > 'h') return NULL;
    if(*(aux+1) < '1' || *(aux+1) > '8') return NULL;

  //  printf("la columna es %d, y la fila es %d, FECAS es %d\n",*aux-'a',*(aux+1)-'1', FCCAS(0,1));
    from = FCCAS(*aux-'a',*(aux+1)-'1');
    aux++;
  //  printf("from es %d, a2 es %d\n",from,A2);
    if(from < 0 || from >= 120) return NULL;
    if(t->pieces[from] == OFFBOARD) return NULL;
    aux++;
    //printf("Ha pasado from\n");
    if(*aux == 'x'){
        captura = 1;
        aux++;
    }
    //printf(" aux1   %d      \n", *aux);
    if ((*aux) < 'a' || (*aux) > 'h') return NULL;
    //printf("aux2    %d      \n", *aux);
    
    if(*(aux+1) < '1' || *(aux+1) > '8') return NULL;
    //printf("antes de FCCAS\n");
    to = FCCAS(*aux-'a', *(aux+1) -'1');
    aux++;
    if(to < 0 || to >= 120) return NULL;
    //printf("Ha pasdo to\n");
    if(t->pieces[to] == OFFBOARD) return NULL;
    aux++;
    //printf("Antes de switch\n");
    if(*aux == '=' && pieza ==  wP +t->side*(CAMBIO_LADO)){
        //printf("Hay una coronacion\n");
        aux++;
        //printf("corono a: %c ",*aux);
        switch (*aux){
            case 'N': corona = wN + CAMBIO_LADO*(t->side); break;
            case 'B': corona = wB + CAMBIO_LADO*(t->side); break;
            case 'R': corona = wR + CAMBIO_LADO*(t->side); break;
            case 'Q': corona = wQ + CAMBIO_LADO*(t->side); break;
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
    //printf("Antes de insertmove\n");
    m = insert_move(EMPTY, from,to,pieza, captura,corona, paso);
    //PrintMove(m);
    if(is_Valid(m,t)){
    //    printf("Valid es %d\n", is_Valid(m,t));
        
        return m;
    }
    free_move(m);
    return NULL;

}


int Menu_juego(TABLERO *tab){
    int flag = OK;
    int flag1;
    INFO info;
    char bando='\0';
    char entradajugada[MAXSTRJUGADA]="\0";
    MOVE *jugada=NULL;
    int acabar = FALSE;

    info.depth = PROFUNDIDAD;
    printf("¡¡¡Bienvenido a nuestro modulo de ajedrez!!!\n\n");

    do{
        printf("Seleccione el bando con el que quiera jugar (w,b):\n");
        fflush(stdin);
        flag1 = scanf("%c",&bando);
        getchar();
        if(flag1 == EOF) return ERR;
        if(bando != 'w' && bando != 'b') printf("Error al introducir el bando.\n");
    }while(bando != 'w'&& bando != 'b'&&flag == OK);
    if (flag == ERR) return ERR;

    printf("Empezamos la partida!\n");

    if(bando == 'w'){
        PrintBoard(tab);

        do{
            printf("Introduzca su jugada:\n");
            fflush(stdin);
            if(fgets(entradajugada, sizeof(entradajugada),stdin) == NULL) flag = ERR;
            //printf("Tu jugada ha sido:%s;\n",entradajugada);
            if(flag != ERR){
                jugada = LeerMovimiento(entradajugada,tab);
                if(!jugada) printf("Jugada inválida.\n");
                else{
                    if(HacerJugada(tab, jugada) == FALSE){
                    printf("jugada inválida.\n");
                    free_move(jugada);
                    jugada = NULL;
                    }
                }
            }

        }while(flag == OK && !jugada);

        free_move(jugada);
        jugada=NULL;
    }

    acabar = FinPartida(tab);
    if(acabar != FALSE){
        switch (acabar){
            case GANAN_NEGRAS: printf("Las negras han ganado esta partida. Bien jugado\n"); break;
            case TABLAS: printf("Esta partida ha sido tablas. Bien jugado\n"); break;
            case GANAN_BLANCAS: printf("Las blancas han ganando esta partida. Bien jugado\n"); break;
            default:break;
        }
    }
    PrintBoard(tab);
    while(flag == OK && acabar == FALSE){
        jugada = SearchPosition(tab,&info);
        HacerJugada(tab,jugada);
        PrintBoard(tab);
        printf("El módulo ha jugado ");
        PrintMove(jugada);
        printf("\n\n");
        free_move(jugada);
        jugada=NULL;
        acabar = FinPartida(tab);
        if(acabar != FALSE){
            switch (acabar){
                case GANAN_NEGRAS: printf("Las negras han ganado esta partida. Bien jugado\n"); break;
                case TABLAS: printf("Esta partida ha sido tablas. Bien jugado\n"); break;
                case GANAN_BLANCAS: printf("Las blancas han ganando esta partida. Bien jugado\n"); break;
                default:break;
            }
        }
        else{
            do{
                printf("Introduzca su jugada:\n");
                fflush(stdin);
                if(fgets(entradajugada, sizeof(entradajugada),stdin) == NULL) flag = ERR;
                //printf("Tu jugada ha sido:%s;\n",entradajugada);
                if(flag != ERR){
                    jugada = LeerMovimiento(entradajugada,tab);
                    if(!jugada) printf("Jugada inválida.\n");
                    else{
                        if(HacerJugada(tab, jugada) == FALSE){
                            
                            free_move(jugada);
                            jugada = NULL;
                        }
                       
                    }
                }

            }while(flag == OK && !jugada);

            free_move(jugada);
            jugada=NULL;
            }
        PrintBoard(tab);
        acabar = FinPartida(tab);
        if(acabar != FALSE){
            switch (acabar){
                case GANAN_NEGRAS: printf("Las negras han ganado esta partida. Bien jugado\n"); break;
                case TABLAS: printf("Esta partida ha sido tablas. Bien jugado\n"); break;
                case GANAN_BLANCAS: printf("Las blancas han ganando esta partida. Bien jugado\n"); break;
                default:break;
            }
        }
        
    }
    return OK;

}

