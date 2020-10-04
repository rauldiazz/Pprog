#include "stdio.h"
#include "definiciones.h"

U64 GeneratePosKey(const TABLERO *pos) {

	int sq = 0;
	U64 finalKey = 0;
	int piece = EMPTY;
	
	// pieces
	for(sq = 0; sq < BRD_SQ_NUM; ++sq) {
		piece = pos->pieces[sq];
		if(piece!=NO_SQ && piece!=EMPTY && piece != OFFBOARD) {
			ASSERT(piece>=wP && piece<=bK);
			finalKey ^= PieceKeys[piece][sq];
		}		
	}
	
	if(pos->side == WHITE) {
		finalKey ^= SideKey;
	}
		
	if(pos->AlPaso != NO_SQ) {
		ASSERT(pos->AlPaso>=0 && pos->AlPaso<BRD_SQ_NUM);
		finalKey ^= PieceKeys[EMPTY][pos->AlPaso];
	}
	
	ASSERT(pos->enroque>=0 && pos->enroque<=15);
	
	finalKey ^= CastleKeys[pos->enroque];
	
	return finalKey;
}
