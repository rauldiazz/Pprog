#include "stdio.h"
#include "definiciones.h"
#include "stdlib.h"

#define PERFTFEN "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"
int main() {	

	TABLERO *tab=NULL;

	tab=Create_tablero();
	InitFILAsCOLsBrd();

	LeerFen(PERFTFEN, tab);



	Comprobaciontest(3,tab);


	Free_tablero(tab);
	
	


	return 0;
}
