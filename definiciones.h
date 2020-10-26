#ifndef DEFS_H
#define DEFS_H

#include "stdlib.h"

#define DEBUG

#ifndef DEBUG
#define ASSERT(n)
#else
#define ASSERT(n) \
if(!(n)) { \
printf("%s - Failed",#n); \
printf("On %s ",__DATE__); \
printf("At %s ",__TIME__); \
printf("In File %s ",__FILE__); \
printf("At Line %d\n",__LINE__); \
exit(1);}
#endif

#define NUM_CASILLAS 120
#define CAMBIO_LADO 6 

#define MAXGAMEMOVES 2048

#define START_FEN  "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

enum { EMPTY, wP, wN, wB, wR, wQ, wK, bP, bN, bB, bR, bQ, bK  };
enum { COL_A, COL_B, COL_C, COL_D, COL_E, COL_F, COL_G, COL_H, COL_NONE };
enum { FILA_1, FILA_2, FILA_3, FILA_4, FILA_5, FILA_6, FILA_7, FILA_8, FILA_NONE };

enum { WHITE, BLACK, BOTH };

enum {
  A1 = 21, B1, C1, D1, E1, F1, G1, H1,
  A2 = 31, B2, C2, D2, E2, F2, G2, H2,
  A3 = 41, B3, C3, D3, E3, F3, G3, H3,
  A4 = 51, B4, C4, D4, E4, F4, G4, H4,
  A5 = 61, B5, C5, D5, E5, F5, G5, H5,
  A6 = 71, B6, C6, D6, E6, F6, G6, H6,
  A7 = 81, B7, C7, D7, E7, F7, G7, H7,
  A8 = 91, B8, C8, D8, E8, F8, G8, H8, NO_SQ, OFFBOARD
};

enum { FALSE, TRUE };

enum { WKCA = 1, WQCA = 2, BKCA = 4, BQCA = 8 };

typedef struct {

	int jugada;
	int enroque;
	int AlPaso;
	int fiftyMove;

} S_UNDO;

typedef struct {

	int *pieces;

		
	int *KingSq;
	
	int side;
	int AlPaso;
	int fiftyMove;
	

	int j_im; 
	int j_real;
	
	int enroque;
	

	
	int *pceNum;
	int *material;
	
	S_UNDO *history;
	
	// piece list
	int **pList;	
	
} TABLERO;

typedef struct{
	int castle;
	int from;
	int to;
	//el primer elemento es la pieza que se ha movido, el segundo lo que ha capturado, y lo último en que se ha coronado
	int piezas[3];
	int paso;
}MOVE;


/* MACROS */

#define FCCAS(col,fila) ( (col + (21) ) + ( (fila) * 10 ) ) 


/* GLOBALS */


/*extern char PceChar[];
extern char SideChar[];
extern char RankChar[];
extern char FileChar[];
extern int PieceVal[13];
extern int PieceBig[13];
extern int PieceMaj[13];
extern int PieceMin[13];

extern int PieceCol[13];*/

extern int FILAsBrd[NUM_CASILLAS];
extern int COLsBrd[NUM_CASILLAS];

/* FUNCTIONS */

// init.c
extern void InitFILAsCOLsBrd();


// tablero.c
extern int pieceColour(int pce);
extern void ResetBoard(TABLERO *pos);
extern int LeerFen(char *fen, TABLERO *pos);
extern void PrintBoard(const TABLERO *pos);
extern void UpdateListsMaterial(TABLERO *pos);
extern int CheckBoard(const TABLERO *pos);
extern int C120a64(int c120);
extern int C64a120(int c64);
extern void Free_tablero(TABLERO *tab);
extern TABLERO* Create_tablero();
int Cas_Col (int cas);
int Cas_Fila (int cas);


// ataque.c
extern int SqAttacked(const int sq, const int side, const TABLERO *pos);

//movimientos.c
extern MOVE **Generador_Peones(TABLERO *t, MOVE **m, int *count );
extern MOVE **Generador_Movimientos(TABLERO *t, int *count);
extern MOVE** Generador_RC(TABLERO *t, MOVE **m, int *count);
int print_moves(MOVE **m, int count);
void free_move(MOVE *m);
extern MOVE ** Generador_Slide(TABLERO *t, MOVE **m, int *count );
#endif