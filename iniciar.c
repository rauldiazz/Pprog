#include "definiciones.h"
#include "stdio.h"
#include "stdlib.h"

#define RAND_64 	((U64)rand() | \
					(U64)rand() << 15 | \
					(U64)rand() << 30 | \
					(U64)rand() << 45 | \
					((U64)rand() & 0xf) << 60 )  
					
int Sq120ToSq64[NUM_CASILLAS];
int Sq64ToSq120[64];

U64 SetMask[64];
U64 ClearMask[64];

U64 PieceKeys[13][120];
U64 SideKey;
U64 CastleKeys[16];

int FILAsBrd[NUM_CASILLAS];
int COLsBrd[NUM_CASILLAS];

void InitFILAsCOLsBrd() {
	
	int index = 0;
	int col = COL_A;
	int fila = FILA_1;
	int sq = A1;
	int sq64 = 0;
	
	for(index = 0; index < NUM_CASILLAS; ++index) {
		FILAsBrd[index] = OFFBOARD;
		COLsBrd[index] = OFFBOARD;
	}
	
	for(fila = FILA_1; fila <= FILA_8; ++fila) {
		for(col = COL_A; col <= COL_H; ++col) {
			sq = FCCAS(col,fila);
			FILAsBrd[sq] = col;
			COLsBrd[sq] = fila;
		}
	}
}

void InitHashKeys() {
	
	int index = 0;
	int index2 = 0;
	for(index = 0; index < 13; ++index) {
		for(index2 = 0; index2 < 120; ++index2) {
			PieceKeys[index][index2] = RAND_64;
		}
	}
	SideKey = RAND_64;
	for(index = 0; index < 16; ++index) {
		CastleKeys[index] = RAND_64;
	}

}

void InitBitMasks() {
	int index = 0;
	
	for(index = 0; index < 64; index++) {
		SetMask[index] = 0ULL;
		ClearMask[index] = 0ULL;
	}
	
	for(index = 0; index < 64; index++) {
		SetMask[index] |= (1ULL << index);
		ClearMask[index] = ~SetMask[index];
	}
}


void AllInit() {	
	InitBitMasks();
	InitHashKeys();	
    InitFILAsCOLsBrd();
}