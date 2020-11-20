#include "stdio.h"
#include "definiciones.h"


#define INFINITO 50000
#define JAQUEMATE 29000
#define MAXDEPTH 64
#define NOMOV 0

static int AlphaBeta(int alpha, int beta, int depth, TABLERO *pos, int *info, int DoNull) { // declarar esta estructura , S_SEARCHINFO *info, posterior
    int MoveNum = 0;
	int Legal = 0;
	int OldAlpha = alpha;
	int BestMove = NOMOV;
	int Score = -INFINITO;

	ASSERT(CheckBoard(pos)); 
	
	if(depth == 0) {
		//info->nodes++;
		(*info)++;
		return EvalPosition(pos);// hacer evalucacion
	}
	//info->nodes++;
	
	(*info)++;
	
	if(Repetida(pos) || pos->fiftyMove >= 100) {
		return 0;
	}
	
	if(pos->j_real > MAXDEPTH - 1) { 

		return EvalPosition(pos); // hacer evalucacion
	}
	
	S_MOVELIST list[1];  //declarar esta estructura
    GenerateAllMoves(pos,list); //cambiar esta funcion 

      
    
	for(MoveNum = 0; MoveNum < list->count; ++MoveNum) {	
       
        if ( !MakeMove(pos,list->moves[MoveNum].move))  {
            continue;
        }
        
		Legal++;
		Score = -AlphaBeta( -beta, -alpha, depth-1, pos, info, TRUE);		
        TakeMove(pos);
		
		if(Score > alpha) {
			if(Score >= beta) {
				return beta;
			}
			alpha = Score;
			BestMove = list->moves[MoveNum].move;
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
		StorePvMove(pos, BestMove); // hacer esta función
	}
	
	return alpha;
} 
