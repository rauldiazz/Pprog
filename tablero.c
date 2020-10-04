
#include "stdio.h"
#include "definiciones.h"

int CheckBoard(const TABLERO *pos) {   
 
	int t_pceNum[13] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	int t_bigPce[2] = { 0, 0};
	int t_majPce[2] = { 0, 0};
	int t_minPce[2] = { 0, 0};
	int t_material[2] = { 0, 0};
	
	int sq64,t_piece,t_pce_num,sq120,colour,pcount;
	
	//U64 t_pawns[3] = {0ULL, 0ULL, 0ULL};
	
	//t_pawns[WHITE] = pos->pawns[WHITE];
	//t_pawns[BLACK] = pos->pawns[BLACK];
	//t_pawns[BOTH] = pos->pawns[BOTH];
	
	// check piece lists
	for(t_piece = wP; t_piece <= bK; ++t_piece) {
		for(t_pce_num = 0; t_pce_num < pos->pceNum[t_piece]; ++t_pce_num) {
			sq120 = pos->pList[t_piece][t_pce_num];
			ASSERT(pos->pieces[sq120]==t_piece);
		}	
	}

	// check piece count and other counters	
	for(sq64 = 0; sq64 < 64; ++sq64) {
		sq120 = SQ120(sq64);
		t_piece = pos->pieces[sq120];
		t_pceNum[t_piece]++;
		colour = PieceCol[t_piece];
		if( PieceBig[t_piece] == TRUE) t_bigPce[colour]++;
		if( PieceMin[t_piece] == TRUE) t_minPce[colour]++;
		if( PieceMaj[t_piece] == TRUE) t_majPce[colour]++;
		
		t_material[colour] += PieceVal[t_piece];
	}
	
	for(t_piece = wP; t_piece <= bK; ++t_piece) {
		ASSERT(t_pceNum[t_piece]==pos->pceNum[t_piece]);	
	}
	
	// check bitboards count
	/*pcount = CNT(t_pawns[WHITE]);
	ASSERT(pcount == pos->pceNum[wP]);
	pcount = CNT(t_pawns[BLACK]);
	ASSERT(pcount == pos->pceNum[bP]);
	pcount = CNT(t_pawns[BOTH]);
	ASSERT(pcount == (pos->pceNum[bP] + pos->pceNum[wP]));*/
	
	// check bitboards squares
	/*while(t_pawns[WHITE]) {
		sq64 = POP(&t_pawns[WHITE]);
		ASSERT(pos->pieces[SQ120(sq64)] == wP);
	}
	
	while(t_pawns[BLACK]) {
		sq64 = POP(&t_pawns[BLACK]);
		ASSERT(pos->pieces[SQ120(sq64)] == bP);
	}
	
	while(t_pawns[BOTH]) {
		sq64 = POP(&t_pawns[BOTH]);
		ASSERT( (pos->pieces[SQ120(sq64)] == bP) || (pos->pieces[SQ120(sq64)] == wP) );
	}*/
	
	ASSERT(t_material[WHITE]==pos->material[WHITE] && t_material[BLACK]==pos->material[BLACK]);
	ASSERT(t_minPce[WHITE]==pos->minPce[WHITE] && t_minPce[BLACK]==pos->minPce[BLACK]);
	ASSERT(t_majPce[WHITE]==pos->majPce[WHITE] && t_majPce[BLACK]==pos->majPce[BLACK]);
	ASSERT(t_bigPce[WHITE]==pos->bigPce[WHITE] && t_bigPce[BLACK]==pos->bigPce[BLACK]);	
	
	ASSERT(pos->side==WHITE || pos->side==BLACK);
	ASSERT(GeneratePosKey(pos)==pos->posKey);
	
	ASSERT(pos->AlPaso==NO_SQ || ( RanksBrd[pos->AlPaso]==COL_6 && pos->side == WHITE)
		 || ( RanksBrd[pos->AlPaso]==COL_3 && pos->side == BLACK));
	
	ASSERT(pos->pieces[pos->KingSq[WHITE]] == wK);
	ASSERT(pos->pieces[pos->KingSq[BLACK]] == bK);
		 
	return TRUE;	
}

void UpdateListsMaterial(TABLERO *pos) {	
	
	int piece,sq,index,colour;
	
	for(index = 0; index < BRD_SQ_NUM; ++index) {
		sq = index;
		piece = pos->pieces[index];
		if(piece!=OFFBOARD && piece!= EMPTY) {
			colour = PieceCol[piece];
			
		    if( PieceBig[piece] == TRUE) pos->bigPce[colour]++;
		    if( PieceMin[piece] == TRUE) pos->minPce[colour]++;
		    if( PieceMaj[piece] == TRUE) pos->majPce[colour]++;
			
			pos->material[colour] += PieceVal[piece];
			
			pos->pList[piece][pos->pceNum[piece]] = sq;
			pos->pceNum[piece]++;
			
			if(piece==wK) pos->KingSq[WHITE] = sq;
			if(piece==bK) pos->KingSq[BLACK] = sq;	
			
	/*		if(piece==wP) {
				SETBIT(pos->pawns[WHITE],SQ64(sq));
				SETBIT(pos->pawns[BOTH],SQ64(sq));
			} else if(piece==bP) {
				SETBIT(pos->pawns[BLACK],SQ64(sq));
				SETBIT(pos->pawns[BOTH],SQ64(sq));
			}*/
		}
	}
}

int ParseFen(char *fen, TABLERO *pos) {
	
	ASSERT(fen!=NULL);
	ASSERT(pos!=NULL);
	
	int  col = COL_8;
    int  fila = FILA_A;
    int  piece = 0;
    int  count = 0;
    int  i = 0; 
	int  sq64 = 0; 
	int  sq120 = 0;
	
	ResetBoard(pos);
	
	while ((col >= COL_1) && *fen) {
	    count = 1;
		switch (*fen) {
            case 'p': piece = bP; break;
            case 'r': piece = bR; break;
            case 'n': piece = bN; break;
            case 'b': piece = bB; break;
            case 'k': piece = bK; break;
            case 'q': piece = bQ; break;
            case 'P': piece = wP; break;
            case 'R': piece = wR; break;
            case 'N': piece = wN; break;
            case 'B': piece = wB; break;
            case 'K': piece = wK; break;
            case 'Q': piece = wQ; break;

            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
                piece = EMPTY;
                count = *fen - '0';
                break;

            case '/':
            case ' ':
                col--;
                fila = FILA_A;
                fen++;
                continue;              

            default:
                printf("FEN error \n");
                return -1;
        }		
		
		for (i = 0; i < count; i++) {			
            sq64 = col * 8 + fila;
			sq120 = SQ120(sq64);
            if (piece != EMPTY) {
                pos->pieces[sq120] = piece;
            }
			fila++;
        }
		fen++;
	}
	
	ASSERT(*fen == 'w' || *fen == 'b');
	
	pos->side = (*fen == 'w') ? WHITE : BLACK;
	fen += 2;
	
	for (i = 0; i < 4; i++) {
        if (*fen == ' ') {
            break;
        }		
		switch(*fen) {
			case 'K': pos->enroque |= WKCA; break;
			case 'Q': pos->enroque |= WQCA; break;
			case 'k': pos->enroque |= BKCA; break;
			case 'q': pos->enroque |= BQCA; break;
			default:	     break;
        }
		fen++;
	}
	fen++;
	
	ASSERT(pos->enroque>=0 && pos->enroque <= 15);
	
	if (*fen != '-') {        
		fila = fen[0] - 'a';
		col = fen[1] - '1';
		
		ASSERT(fila>=FILA_A && fila <= FILA_H);
		ASSERT(col>=COL_1 && col <= COL_8);
		
		pos->AlPaso = FR2SQ(fila,col);		
    }
	
	pos->posKey = GeneratePosKey(pos); 
	
	UpdateListsMaterial(pos);
	
	return 0;
}

void ResetBoard(TABLERO *pos) {

	int index = 0;
	
	for(index = 0; index < BRD_SQ_NUM; ++index) {
		pos->pieces[index] = OFFBOARD;
	}
	
	for(index = 0; index < 64; ++index) {
		pos->pieces[SQ120(index)] = EMPTY;
	}
	
	for(index = 0; index < 3; ++index) {
		pos->bigPce[index] = 0;
		pos->majPce[index] = 0;
		pos->minPce[index] = 0;
	//	pos->pawns[index] = 0ULL;
	}
	
	for(index = 0; index < 13; ++index) {
		pos->pceNum[index] = 0;
	}
	
	pos->KingSq[WHITE] = pos->KingSq[BLACK] = NO_SQ;
	
	pos->side = BOTH;
	pos->AlPaso = NO_SQ;
	pos->fiftyMove = 0;
	
	pos->j_im = 0;
	pos->j_real = 0;
	
	pos->enroque = 0;
	
	pos->posKey = 0ULL;
	
}
void PrintBoard(const TABLERO *pos) {
	
	int sq,fila,col,piece;

	printf("\nGame Board:\n\n");
	
	for(col = COL_8; col >= COL_1; col--) {
		printf("%d  ",col+1);
		for(fila = FILA_A; fila <= FILA_H; fila++) {
			sq = FR2SQ(fila,col);
			piece = pos->pieces[sq];
			printf("%3c",PceChar[piece]);
		}
		printf("\n");
	}
	
	printf("\n   ");
	for(fila = FILA_A; fila <= FILA_H; fila++) {
		printf("%3c",'a'+fila);	
	}
	printf("\n");
	printf("side:%c\n",SideChar[pos->side]);
	printf("AlPaso:%d\n",pos->AlPaso);
	printf("castle:%c%c%c%c\n",
			pos->enroque & WKCA ? 'K' : '-',
			pos->enroque & WQCA ? 'Q' : '-',
			pos->enroque & BKCA ? 'k' : '-',
			pos->enroque & BQCA ? 'q' : '-'	
			);
	printf("PosKey:%llX\n",pos->posKey);
}