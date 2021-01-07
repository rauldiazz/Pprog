#include "stdio.h"
#include "definiciones.h"
#include "stdlib.h"

//Posiciones para probar la partida empezada desde una determinada posición, en caso de querer usarse simplemente en la llamada
//a LeerFen, en vez de START_FEN se pone FEN1 o la que se quiera usar.

#define FEN1 "8/2k5/1p6/8/p7/3K3P/6P1/8 w  - - 0 1"
#define FEN2 "1k6/8/8/8/8/8/8/K5QR w  - - 0 1"
#define FEN3 "8/8/2k5/7R/8/8/8/2K3R1 w  - - 0 1"
#define FEN4 "8/8/2k3b1/8/8/8/8/2K5 w  - - 0 1"
#define FEN5 "rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P5/RNBQKBNR w KQkq e6 0 1"
#define FEN6 "2n3q1/PPK5/1N3P2/2R5/1p2bNk1/2PB3p/7n/8 b - - 0 1"
#define FEN7 "rn1qkbnr/p2b1ppp/1pp5/3pp3/QP2PP2/B1PP1NP1/P2N3P/R3KB1R w KQkq - 0 1"
#define FEN8 "rn2kb1r/ppp1p2p/3p1N1n/2qP1bBp/4P3/3Q1P1N/PPP1B1PP/R3K2R w KQkq - 0 1"
#define FEN9 "r3kb1r/ppp1p2p/n2p1N1n/2qP1bBp/4P3/3Q1P1N/PPP1B1PP/R3K2R w KQkq - 0 1"
#define FEN10 "rq2r2k/pp4p1/3n1pBp/2pP4/2P2RP1/P5Q1/2P2R2/6K1 b  - - 0 1"
#define PERFTFEN "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"
#define FENAUX2 "2k5/7Q/R7/2p5/8/8/1PPP1PBP/2B1K2R w K - 0 1"
#define FENAUX "r3k2r/ppp2pQp/2n5/3n4/8/8/RPPP1PBP/2B1K2R w Kkq - 0 1"
#define FENAB "6k1/R4p2/2r4p/3r2P1/5P2/3P4/6P1/3R2K1 w  - - 0 1"


int main() {	

	TABLERO *tab=NULL;
	MOVE *jugada,*jugada2;
	char *fen,jugadachar[512];
	int i, valor;
	INFO *info;

	tab=Create_tablero();
	InitFILAsCOLsBrd();

	LeerFen(START_FEN, tab);

	Menu_juego(tab);
	Free_tablero(tab);

	return 0;
}
