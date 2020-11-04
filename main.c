#include "stdio.h"
#include "definiciones.h"
#include "stdlib.h"

#define FEN1 "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1"
#define FEN2 "rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2"
#define FEN3 "rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2"
#define FEN4 "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R b KQkq - 0 1"
#define FEN5 "rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P5/RNBQKBNR w KQkq e6 0 1"
#define FEN6 "2n3q1/PPK5/1N3P2/2R5/1p2bNk1/2PB3p/7n/8 b - - 0 1"
#define FEN7 "rn1qkbnr/p2b1ppp/1pp5/3pp3/QP2PP2/B1PP1NP1/P2N3P/R3KB1R w KQkq - 0 1"
#define FEN8 "rn2kb1r/ppp1p2p/3p1N1n/2qP1bBp/4P3/3Q1P1N/PPP1B1PP/R3K2R w KQkq - 0 1"
#define FEN9 "r3kb1r/ppp1p2p/n2p1N1n/2qP1bBp/4P3/3Q1P1N/PPP1B1PP/R3K2R w KQkq - 0 1"
#define FEN10 "r3kb1r/ppp1p2p/n2p3N/2qP1bBp/4P3/3Q1P1N/PPP1B1PP/R3K2R b KQkq - 0 1"
#define PERFTFEN "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"
#define FENAUX2 "2kr3r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/P1N2Q1p/1PPBBPPP/R3K2R w KQ - 0 1"
#define FENAUX "k7/7P/8/N7/8/8/8/7K w  - 0 1"
int main() {	

	TABLERO *tab=NULL;
	MOVE *jugada,*jugada2;
	char *fen;

	tab=Create_tablero();
	InitFILAsCOLsBrd();

	LeerFen(FENAUX, tab);

	//PrintBoard(tab);
	/*jugada = insert_move(EMPTY,A1,A2,wR,EMPTY,EMPTY,EMPTY);
	HacerJugada(tab,jugada);
	fen = EscribirFen(tab);
	printf("fen es %s\n",fen);
	free(fen);*/
	/*free_move(jugada);*/
	/*jugada = insert_move(EMPTY,F7,F5,bP,EMPTY,EMPTY,EMPTY);
	HacerJugada(tab,jugada);
	free_move(jugada);
	jugada = insert_move(EMPTY,F1,E2,wB,EMPTY,EMPTY,EMPTY);
	HacerJugada(tab,jugada);
	free_move(jugada);*/

	Comprobaciontest(7, tab);


	Free_tablero(tab);
	
	


	return 0;
}
