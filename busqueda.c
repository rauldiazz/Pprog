#include "stdio.h"
#include "definiciones.h"


#define INFINITO 50000
#define JAQUEMATE 30000
#define PROFMAX 64
#define NOMOV 0

static int AlphaBeta(int alpha, int beta, int depth, TABLERO *pos, INFO *info,MOVE** Best) { 
	int Legal = 0;
	int Score = -INFINITO;
	MOVE ** movelist;
	int count;
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
	
	/*if(pos->j_real > PROFMAX - 1) { 

		return EvalPosition(pos); // hacer evalucacion
	}*/
	
	
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
			if(depth == 5){
				free_move(*Best);
				(*Best)=move_copy(movelist[index]);
			}
		}	
    }
	for(index=0; index<(count);index++){
		free_move(movelist[index]);
		
	}
	
	if(Legal == 0) {
		if(SqAttacked(pos->KingSq[pos->side],CAMBIO_LADO*pos->side,pos)) {
			return -JAQUEMATE+ pos->j_real;
		} else {
			return 0;
		}
	}
	free(movelist);

	return alpha;
} 


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
	
