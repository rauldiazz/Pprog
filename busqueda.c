#include "stdio.h"
#include "definiciones.h"


#define INFINITO 50000
#define JAQUEMATE 30000
#define PROFMAX 64
#define NOMOV 0

static int AlphaBeta(int alpha, int beta, int depth, TABLERO *pos, int *info, int DoNull) { 
	int Legal = 0;
	int OldAlpha = alpha;
	int BestMove = NOMOV;
	int Score = -INFINITO;
	MOVE ** movelist;
	int* count;
	int index;


	ASSERT(CheckBoard(pos)); 
	
	if(depth == 0) {
		//info->nodes++;
		(*info)++;
		return EvalPosition(pos);// hacer evalucacion
	}
	//info->nodes++;
	
	(*info)++;
	
	if(esTablas(pos)) {
		return 0;
	}
	
	if(pos->j_real > PROFMAX - 1) { 

		return EvalPosition(pos); // hacer evalucacion
	}
	
	
    movelist = GenerateAllMoves(pos,count); 
      
    
	for(index= 0; index< (*count); ++index) {	
       
        if ( !HacerJugada(pos,movelist[index]))  {
            continue;
        }
        
		Legal++;
		Score = -AlphaBeta( -beta, -alpha, depth-1, pos, info, TRUE);		
        DeshacerJugada(pos);
		
		if(Score > alpha) {
			if(Score >= beta) {
				return beta;
			}
			alpha = Score;
			BestMove = movelist[index];
		}		
    }
	
	if(Legal == 0) {
		if(SqAttacked(pos->KingSq[pos->side],pos->side^1,pos)) {
			return -JAQUEMATE + pos->j_real;
		} else {
			return 0;
		}
	}
	
	if(alpha != OldAlpha) {
		StorePvMove(pos, BestMove); //hacer 
	}
	
	return alpha;
} 
