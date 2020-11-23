#include <stdio.h>
#include "definiciones.h"
#define SQOFFBOARD(sq) (FILAsBrd[sq]==OFFBOARD)
#define MATERIALFINAL (PieceVal[wR] + 2 * PieceVal[wN] + 2 * PieceVal[wP] + PieceVal[wK])
int PAREJAALFILES = 30;
int puntpeones[64]={
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	,
10	,	10	,	0	,	-10	,	-10	,	0	,	10	,	10	,
5	,	0	,	0	,	5	,	5	,	0	,	0	,	5	,
0	,	0	,	10	,	20	,	20	,	10	,	0	,	0	,
5	,	5	,	5	,	10	,	10	,	5	,	5	,	5	,
10	,	10	,	10	,	20	,	20	,	10	,	10	,	10	,
20	,	20	,	20	,	30	,	30	,	20	,	20	,	20	,
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	
};

int puntcaballo[64] = {
0	,	-10	,	0	,	0	,	0	,	0	,	-10	,	0	,
0	,	0	,	0	,	5	,	5	,	0	,	0	,	0	,
0	,	0	,	10	,	10	,	10	,	10	,	0	,	0	,
0	,	0	,	10	,	20	,	20	,	10	,	5	,	0	,
5	,	10	,	15	,	20	,	20	,	15	,	10	,	5	,
5	,	10	,	10	,	20	,	20	,	10	,	10	,	5	,
0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0		
};

int puntalfil[64] = {
0	,	0	,	-10	,	0	,	0	,	-10	,	0	,	0	,
0	,	0	,	0	,	10	,	10	,	0	,	0	,	0	,
0	,	0	,	10	,	15	,	15	,	10	,	0	,	0	,
0	,	10	,	15	,	20	,	20	,	15	,	10	,	0	,
0	,	10	,	15	,	20	,	20	,	15	,	10	,	0	,
0	,	0	,	10	,	15	,	15	,	10	,	0	,	0	,
0	,	0	,	0	,	10	,	10	,	0	,	0	,	0	,
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	
};

int punttorre[64] = {
0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
25	,	25	,	25	,	25	,	25	,	25	,	25	,	25	,
0	,	0	,	5	,	10	,	10	,	5	,	0	,	0		
};

int puntdama[64]={
-3	,	-3	,	-3	,	-3	,	-3	,	-3	,	-3	,	-3	,
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	,
0	,	0	,	2	,	2	,	2	,	2	,	0	,	0	,
0	,	0	,	2	,	5	,	5	,	2	,	0	,	0	,
0	,	0	,	2	,	5	,	5	,	2	,	0	,	0	,
0	,	0	,	2	,	2	,	2	,	2	,	0	,	0	,
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	,
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0   
};

int reynormal[64]={	
	0	,	5	,	5	,	-10	,	-10	,	0	,	10	,	5	,
	-30	,	-30	,	-30	,	-30	,	-30	,	-30	,	-30	,	-30	,
	-50	,	-50	,	-50	,	-50	,	-50	,	-50	,	-50	,	-50	,
	-70	,	-70	,	-70	,	-70	,	-70	,	-70	,	-70	,	-70	,
	-70	,	-70	,	-70	,	-70	,	-70	,	-70	,	-70	,	-70	,
	-70	,	-70	,	-70	,	-70	,	-70	,	-70	,	-70	,	-70	,
	-70	,	-70	,	-70	,	-70	,	-70	,	-70	,	-70	,	-70	,
	-70	,	-70	,	-70	,	-70	,	-70	,	-70	,	-70	,	-70		
};

int reyendgame[64]={	
	-50	,	-10	,	0	,	0	,	0	,	0	,	-10	,	-50	,
	-10,	0	,	10	,	10	,	10	,	10	,	0	,	-10	,
	0	,	10	,	20	,	20	,	20	,	20	,	10	,	0	,
	0	,	10	,	20	,	40	,	40	,	20	,	10	,	0	,
	0	,	10	,	20	,	40	,	40	,	20	,	10	,	0	,
	0	,	10	,	20	,	20	,	20	,	20	,	10	,	0	,
	-10,	0	,	10	,	10	,	10	,	10	,	0	,	-10	,
	-50	,	-10	,	0	,	0	,	0	,	0	,	-10	,	-50	
};
int Mirror64(int sq64){
    return 63-sq64;
}

int EvalPosition(const TABLERO *pos) {

	int casilla, pce, npiezas, casilla64;
	int punt = pos->material[WHITE] - pos->material[BLACK];
	
	pce = wP;	
	for(npiezas = 0; npiezas < pos->pceNum[pce]; npiezas++) {
		casilla = pos->pList[pce][npiezas];
		ASSERT(!SQOFFBOARD(casilla));
        casilla64=C120a64(casilla);
		punt += puntpeones[casilla64];
	}	

	pce = bP;	
	for(npiezas = 0; npiezas < pos->pceNum[pce]; npiezas++) {
		casilla = pos->pList[pce][npiezas];
		ASSERT(!SQOFFBOARD(casilla));
        casilla64=C120a64(casilla);
		punt -= puntpeones[Mirror64(casilla64)];
        
	}	
	
	pce = wN;	
	for(npiezas = 0; npiezas < pos->pceNum[pce]; npiezas++) {
		casilla = pos->pList[pce][npiezas];
		ASSERT(!SQOFFBOARD(casilla));
        casilla64=C120a64(casilla);
		punt += puntcaballo[casilla64];
	}	

	pce = bN;	
	for(npiezas = 0; npiezas < pos->pceNum[pce]; npiezas++) {
		casilla = pos->pList[pce][npiezas];
		ASSERT(!SQOFFBOARD(casilla));
        casilla64=C120a64(casilla);
		punt -= puntcaballo[Mirror64(casilla64)];
	}			
	
	pce = wB;	
	for(npiezas = 0; npiezas < pos->pceNum[pce]; npiezas++) {
		casilla = pos->pList[pce][npiezas];
		ASSERT(!SQOFFBOARD(casilla));
        casilla64=C120a64(casilla);
		punt += puntalfil[casilla64];
	}	

	pce = bB;	
	for(npiezas = 0; npiezas < pos->pceNum[pce]; npiezas++) {
		casilla = pos->pList[pce][npiezas];
		ASSERT(!SQOFFBOARD(casilla));
        casilla64=C120a64(casilla);
		punt -= puntalfil[Mirror64(casilla64)];
	}	

	pce = wR;	
	for(npiezas = 0; npiezas < pos->pceNum[pce]; npiezas++) {
		casilla = pos->pList[pce][npiezas];
		ASSERT(!SQOFFBOARD(casilla));
        casilla64=C120a64(casilla);
		punt += punttorre[casilla64];
	}	

	pce = bR;	
	for(npiezas = 0; npiezas < pos->pceNum[pce]; npiezas++) {
		casilla = pos->pList[pce][npiezas];
		ASSERT(!SQOFFBOARD(casilla));
        casilla64=C120a64(casilla);
		punt -= punttorre[Mirror64(casilla64)];
	}

    pce = wQ;	
	for(npiezas = 0; npiezas < pos->pceNum[pce]; npiezas++) {
		casilla = pos->pList[pce][npiezas];
		ASSERT(!SQOFFBOARD(casilla));
        casilla64=C120a64(casilla);
		punt += puntdama[casilla64];
	}	

	pce = bQ;	
	for(npiezas = 0; npiezas < pos->pceNum[pce]; npiezas++) {
		casilla = pos->pList[pce][npiezas];
		ASSERT(!SQOFFBOARD(casilla));
        casilla64=C120a64(casilla);
		punt -= puntdama[Mirror64(casilla64)];
	}

	pce = wK;
	casilla = pos->pList[pce][0];
	if( (pos->material[BLACK] <= MATERIALFINAL) ) {
		casilla64=C120a64(casilla);
		punt += reyendgame[(casilla64)];
	} else {
		punt += reynormal[(casilla64)];
	}

	pce=bK;
	casilla = pos->pList[pce][0];
	if( (pos->material[WHITE] <= MATERIALFINAL) ) {
		casilla64=C120a64(casilla);
		punt -= reyendgame[Mirror64(casilla64)];
	} else {
		punt -= reynormal[Mirror64(casilla64)];
	}

	if(pos->pceNum[wB] >= 2) punt += PAREJAALFILES;
	if(pos->pceNum[bB] >= 2) punt -= PAREJAALFILES;

	if(pos->side == WHITE) {
		return punt;
	} else {
		return -punt;
	}	
}




