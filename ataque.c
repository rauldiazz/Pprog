
#include "stdio.h"
#include "definiciones.h"

const int KnDir[8] = { -8, -19,	-21, -12, 8, 19, 21, 12 };
const int RkDir[4] = { -1, -10,	1, 10 };
const int BiDir[4] = { -9, -11, 11, 9 };
const int KiDir[8] = { -1, -10,	1, 10, -9, -11, 11, 9 };

int pieceColour(int pce){

    if(pce == wP || pce == wR || pce == wB || pce == wN || pce == wQ || pce == wK) return WHITE;

    else if(pce == bP || pce == bR || pce == bB || pce == bN || pce == bQ || pce == bK) return BLACK;

    return BOTH;
}


int SqAttacked(const int sq, const int side, const TABLERO *pos) {

	int pce,index,t_sq,dir;
	
	// pawns
	if(side == WHITE) {
		if(pos->pieces[sq-11] == wP || pos->pieces[sq-9] == wP) {
			return TRUE;
		}
	} else {
		if(pos->pieces[sq+11] == bP || pos->pieces[sq+9] == bP) {
			return TRUE;
		}	
	}
	
	// knights
	for(index = 0; index < 8; ++index) {		
		pce = pos->pieces[sq + KnDir[index]];
		if((pce == wN || pce == bN) && pieceColour(pce)==side) {
			return TRUE;
		}
	}
	
	// rooks, queens
	for(index = 0; index < 4; ++index) {		
		dir = RkDir[index];
		t_sq = sq + dir;
		pce = pos->pieces[t_sq];
		while(pce != OFFBOARD) {
			if(pce != EMPTY) {
				if((pce == bR || pce == bQ || pce == wQ || pce == wR) && pieceColour(pce) == side) {
					return TRUE;
				}
				break;
			}
			t_sq += dir;
			pce = pos->pieces[t_sq];
		}
	}
	
	// bishops, queens
	for(index = 0; index < 4; ++index) {		
		dir = BiDir[index];
		t_sq = sq + dir;
		pce = pos->pieces[t_sq];
		while(pce != OFFBOARD) {
			if(pce != EMPTY) {
				if((pce == bB || pce == bQ || pce == wQ || pce == wB) && pieceColour(pce) == side) {
					return TRUE;
				}
				break;
			}
			t_sq += dir;
			pce = pos->pieces[t_sq];
		}
	}
	
	// kings
	for(index = 0; index < 8; ++index) {		
		pce = pos->pieces[sq + KiDir[index]];
		if((pce == wK || pce == bK) && pieceColour(pce)==side) {
			return TRUE;
		}
	}
	