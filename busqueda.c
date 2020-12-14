#include "stdio.h"
#include "definiciones.h"


#define INFINITO 50000
#define JAQUEMATE 30000
#define PROFMAX 64
#define NOMOV 0

/***********************************************************/
/* Función: Alphabeta                             
/* Autores: Omicron: Pablo Soto, Sergio Leal, Raúl Díaz                                  
/*                                                         
/* Parámetros de entrada:
/* alpha: mejor opcion para el maximizador
/* beta: mejor opcion para el minimizador
/* depth: profundidad del algoritmo                                                  
/* pos: puntero a un tablero
/* info: puntero a tipo de dato info con los datosm de la busqueda
/* best: puntero donde se almacenara el movimiento seleccioando
/* 
/* Retorno:
/* alfa: alfa/beta en cada caso
/*
/* Descripción:
/* Algoritmo de busqueda del mejor movimiento a realizar a una cierta profundidad seleccionada
/***********************************************************/

static int AlphaBeta(int alpha, int beta, int depth, TABLERO *pos, INFO *info,MOVE** Best) { 
	int Legal = 0;
	int Score = -INFINITO;
	MOVE ** movelist;
	int count,c;
	int index=0;


	ASSERT(CheckBoard(pos)); 
	
	if(depth == 0) {
		info->visited++;
		
		return EvalPosition(pos);// hacer evalucacion
	}
	info->visited++;
	
	
	if(esTablas(pos)) {
		return 0;
	}
	
	Score = -INFINITO;
	
    movelist = Generador_Movimientos(pos,&count); 
      
    
	for(index= 1; index< count; index++) {	
       
        if ( HacerJugada(pos,movelist[index])==FALSE)  {
            continue;
        }
        
		Legal++;
		
		Score = -AlphaBeta( -beta, -alpha, depth-1, pos, info, Best);		
        DeshacerJugada(pos);
		if(Score >= beta) {
			return beta;
		}
		if(Score > alpha) {
			alpha = Score;
			if(depth == PROFUNDIDAD){
				free_move(*Best);
				(*Best)=move_copy(movelist[index]);
                PrintMove(*Best);
              
                printf("   %d\n",Score);
                

			}
		}	
    }
	for(index=0; index<(count);index++){
		free_move(movelist[index]);
		
	}
	
	if(Legal == 0) {
		if(SqAttacked(pos->KingSq[pos->side],pos->side^1,pos)) {
			return -JAQUEMATE+ pos->j_real;
		} else {
			return 0;
		}
	}
	free(movelist);

	return alpha;
} 


/***********************************************************/
/* Función: SearchPosition                             
/* Autores: Omicron: Pablo Soto, Sergio Leal, Raúl Díaz                                  
/*                                                         
/* Parámetros de entrada:                                            
/* pos: puntero a un tablero
/* info: puntero a tipo de dato info con los datos de la busqueda
/* 
/* Retorno:
/* Puntero al movimiento que se ha seleccionado tras la busqueda
/* 
/* Descripcion:
/* Almacena los datos de la busqueda de alphabeta en info y devuelve el mejor movimiento para una posicion
/* 
/***********************************************************/

MOVE* SearchPosition(TABLERO *pos, INFO  *info) {

	MOVE **Best;
	MOVE *retorno;
	Best=(MOVE**)malloc(sizeof(MOVE*));
	int bestScore = -INFINITO;
	int actualDepth = info->depth;
	*Best=NULL;

		bestScore = AlphaBeta(-INFINITO, INFINITO, actualDepth, pos, info,Best);
		info->bestScore=bestScore;
		PrintMove((*Best));
	retorno=(*Best);
	free(Best);
	return retorno;
}
