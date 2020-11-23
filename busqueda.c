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
	
	if(pos->j_real > PROFMAX - 1) { 

		return EvalPosition(pos); // hacer evalucacion
	}
	
	
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
/*	
int max(int *a , int *b){
    if (*a>*b)
    return *a;
    else 
    return *b;
}
int min(int *a , int *b){
    if (*a<*b)
    return *a;
    else 
    return *b;
}


static int Alphabeta(int depth,int alpha, int beta,TABLERO *pos,int side){
    int value,legal,aux1,aux2;
    int index, countt,ind;
    MOVE** movelist=NULL;
   
    if (depth == 0){

        return EvalPosition(pos);
    }

	if(esTablas(pos)) {
		return 0;
	}
    if (side==TRUE){
        value = -INFINITO;
        movelist = Generador_Movimientos(pos,&countt); 
        for(index= 1; index< countt; index++) {	
       
            if ( HacerJugada(pos,movelist[index])==FALSE)  {
                continue;
            }
            legal++;
            aux1=Alphabeta( depth - 1, alpha, beta, pos,FALSE);
            value = max(&value, &aux1);
            DeshacerJugada(pos);
            alpha= max(&alpha, &value);
             if (alpha >= beta)
                break;
             }  
        for(ind=0; ind<countt; ind++){
                    free(movelist[ind]);
        }
        free(movelist);
        return value;
    }
    else{
        value =INFINITO;
        movelist = Generador_Movimientos(pos,&countt); 
        for(index= 1; index< countt; index++) {	
        
                if ( HacerJugada(pos,movelist[index])==FALSE)  {
                    continue;
                }
                legal++;
                aux2=Alphabeta( depth - 1, alpha, beta, pos , TRUE);
                value = min(&value, &aux2);
                DeshacerJugada(pos);
                beta = min(&beta, &value);
             
                
                if (beta <= alpha)
                    break;
            }
        for(ind=0; ind<countt; ind++){
                    free(movelist[ind]);
        }
        free(movelist);
        return value;
        }
}
MOVE* SearchPosition(TABLERO *pos, INFO  *info) {

	MOVE **movelistt;
    MOVE *Best;
    int count, index;

	int bestScore = -INFINITO;
	int actualDepth = info->depth;


	bestScore = alphabeta(info->depth, -INFINITO, INFINITO,pos, TRUE);
	movelistt= Generador_Movimientos(pos, &count);
    for(index=1; index<count; index++){
        HacerJugada(pos,movelistt[index]);
        if(EvalPosition(pos)==bestScore){
            Best=move_copy(movelistt[index]);
            break;
        }
        DeshacerJugada(pos);
        }
    for(index=0; index<count; index++){
        free(movelistt[index]);
    }
         
    
    free(movelistt);
return Best;
}	
*/