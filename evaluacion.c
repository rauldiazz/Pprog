#include <stdio.h>
#include "definiciones.h"
#define SQOFFBOARD(sq) (FILAsBrd[sq]==OFFBOARD)
#define MATERIALFINAL (PieceVal[wR] + 2 * PieceVal[wN] + 2 * PieceVal[wP] + PieceVal[wK])
int PAREJAALFILES = 30;

/*puntpeones: Array que contiene las puntuaciones para las posiciones de los peones en el tablero*/

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

/*puntcaballo: Array que contiene las puntuaciones para las posiciones de los caballos en el tablero*/

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

/*puntalfil: Array que contiene las puntuaciones para las posiciones de los alfiles en el tablero*/

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

/*punttorre: Array que contiene las puntuaciones para las posiciones de las torres en el tablero*/

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

/*reynormal: Array que contiene las puntuaciones para las posiciones del rey en el tablero, en un estado 'early-game' o 'mid-game'*/


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

/*reyendgame: Array que contiene las puntuaciones para las posiciones del rey en el tablero, en un estado 'late-game'*/


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



/***********************************************************/
/* Funcion: Mirror64                           
/* Autores: Omicron: Pablo Soto, Sergio Leal, Raúl Díaz                                  
/*                                                         
/* Parametros de entrada:                                               
/* sq64: entero que indica una casilla del tablero
/* 
/* Retorno:
/* casilla 'opuesta'
/*
/* Descripción:
/* Esta función devuelve la casilla contraria, utilizada para
/* cambiar de blancas a negras en los arrays posicionales
/***********************************************************/

int Mirror64(int sq64){
    return 63-sq64;
}

/***********************************************************/
/* Funcion: EvalPosition                             
/* Autores: Omicron: Pablo Soto, Sergio Leal, Raúl Díaz                                  
/*                                                         
/* Parametros de entrada:                                               
/* pos: puntero a un tablero
/* 
/* Retorno:
/* Valor de la puntuación evaluacion
/*
/* Descripción:
/* Devuelve la evaluación posicional del bando que indique el tablero
/* 
/* Más en detalle: Esta función se basa en contar el material que tiene cada uno de los
/* bandos y hacer la resta para ver la diferencia, además usando arrays posicionales
/* para cada tipo de pieza, que determinan en qué casilla del tablero es más fuerte cada pieza.
/* Además valora otros factores como la conservación de las parejas de alfiles como algo positivo 
/* o diferencia entre las casillas en las que debe de estar el rey dependiendo del momento de la partida
/***********************************************************/

int EvalPosition(const TABLERO *pos) {

	int casilla, pce, npiezas, casilla64;
	int punt = pos->material[WHITE] - pos->material[BLACK];

	if(!pos->pceNum[wP] && !pos->pceNum[bP] && punt==0) {
		return 0;
	}
	
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




