#include "stdio.h"
#include "definiciones.h"


#define INFINITO 50000
#define JAQUEMATE 30000
#define PROFMAX 64
#define NOMOV 0

static int AlphaBeta(int alpha, int beta, int depth, TABLERO *pos, INFO *info,MOVE* Best) { 
	int Legal = 0;
	//MOVE *Best=NULL;
	int Score = -INFINITO;
	MOVE ** movelist;
	int* count;
	int index;


	ASSERT(CheckBoard(pos)); 
	
	if(depth == 0) {
		info->visited++;
		
		return EvalPosition(pos);// hacer evalucacion
	}
	info->visited++;
	
	
	if(esTablas(pos)) {
		return 0;
	}
	
	if(pos->j_real > PROFMAX - 1) { 

		return EvalPosition(pos); // hacer evalucacion
	}
	
	
    movelist = GenerateAllMoves(pos,count); 
      
    
	for(index= 0; index< (*count); index++) {	
       
        if ( !HacerJugada(pos,movelist[index]))  {
            continue;
        }
        
		Legal++;
		Score = -AlphaBeta( -beta, -alpha, depth-1, pos, info, Best);		
        DeshacerJugada(pos);
		
		if(Score > alpha) {
			if(Score >= beta) {
				return beta;
			}
			alpha = Score;
			if(Best!=NULL){
				free_move(Best);
			}
			Best = movelist[index];
		}	
    }
	for(index=0; index<(*count); index++){
		if(Best!=movelist[index])
			free_move(movelist[index]);

	}
	
	if(Legal == 0) {
		if(SqAttacked(pos->KingSq[pos->side],pos->side^1,pos)) {
			return -JAQUEMATE + pos->j_real;
		} else {
			return 0;
		}
	}
	

	free(movelist);
	return alpha;
} 


void SearchPosition(TABLERO *pos, INFO  *info) {

	MOVE *Best=NULL;
	int bestScore = -INFINITO;
	int actualDepth = info->depth;

		bestScore = AlphaBeta(-INFINITO, INFINITO, actualDepth, pos, info,Best);
		info->stop=bestScore;
	
	return Best;
}
	
