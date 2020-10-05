
#include "stdio.h"
#include "definiciones.h"

int C64a120 (int c64){
	int resto;
	int cociente;

	resto = c64 % 8;
	cociente = (c64-resto)/8;
	return (cociente*10 + resto + 21);
}


int C120a64 (int c120){
	int resta, resto, cociente;
	resta = c120- 21;
	if(resta<0 || resta >77 || (resta % 10) >= 8) return OFFBOARD;
	resto = resta % 10;
	cociente = (resta-resto)/10;
	return cociente*8 + resto;
}

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
		sq120 = C64a120(sq64);
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
	//ASSERT(GeneratePosKey(pos)==pos->posKey);
	
	ASSERT(pos->AlPaso==NO_SQ || ( RanksBrd[pos->AlPaso]==FILA_6 && pos->side == WHITE)
		 || ( RanksBrd[pos->AlPaso]==FILA_3 && pos->side == BLACK));
	
	ASSERT(pos->pieces[pos->KingSq[WHITE]] == wK);
	ASSERT(pos->pieces[pos->KingSq[BLACK]] == bK);
		 
	return TRUE;	
}

void UpdateListsMaterial(TABLERO *pos) {	
	
	int piece,sq,index,colour;
	
	for(index = 0; index < NUM_CASILLAS; ++index) {
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

int LeerFen(char *fen, TABLERO *pos) {
	
	ASSERT(fen!=NULL);
	ASSERT(pos!=NULL);
	
	int  fila = FILA_8, col = COL_A, piece = 0, count = 0, i = 0,  c64 = 0, c120 = 0;
	short flag =0;
	
	ResetBoard(pos);
	
	while ((fila >= FILA_1) && *fen) {
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
                fila--;
                col = COL_A;
                count=0;
                break;              

            default:
                printf("FEN error \n");
                return -1;
        }		
		
		for (i = 0; i < count; i++) {			
            c64 = fila * 8 + col;
			c120 = C64a120(c64);
            if (piece != EMPTY) {
                pos->pieces[c120] = piece;
            }
			col++;
        }
		fen++;
	}
	
	ASSERT(*fen == 'w' || *fen == 'b');
	
	if(*fen == 'w') pos->side = WHITE;
	else pos->side = BLACK;
	fen += 2;
	
	pos->enroque = 0;
	for (i = 0, flag = 1; i < 4 && flag == 1; i++) {
        //if (*fen == ' ') flag = 0;	
		switch(*fen) {
			case 'K': pos->enroque += WKCA; break;
			case 'Q': pos->enroque += WQCA; break;
			case 'k': pos->enroque += BKCA; break;
			case 'q': pos->enroque += BQCA; break;
			default: flag = 0;
        }
		fen++;
	}
	fen++;
	
	ASSERT(pos->enroque>=0 && pos->enroque <= 15);
	
	if (*fen != '-') {        
		col = fen[0] - 'a';
		fila = fen[1] - '1';
		
		ASSERT(col>=COL_A && col <= COL_H);
		ASSERT(fila>=FILA_1 && fila <= FILA_8);
		
		pos->AlPaso = FCCAS(col,fila);
		fen++;	
    }
	fen += 2;

	if(fen[1] == ' '){
		pos->fiftyMove = fen[0] - '0';
	}
	else{
		pos->fiftyMove = (fen[0] - '0')*10 + (fen[1] - '0');
		fen++;
	}
	fen+=2;

	if(fen[1] == '\0'){
		pos->j_real = 2*(fen[0]-'0') + pos->side;
	}
	else if(fen[2] == '\0'){
		pos->j_real = 2*((fen[1]-'0')*10 + (fen[0]-'0')) + pos->side;
	}
	else if(fen[3] == '\0'){
		pos->j_real = 2*((fen[2]-'0')*100 + (fen[1]-'0')*10 + (fen[0]-'0')) + pos->side;
	}
	else if (fen[4] == '\0'){
		pos->j_real = 2*((fen[3]-'0')*1000 + (fen[2]-'0')*100 + (fen[1]-'0')*10 + (fen[0]-'0')) + pos->side;
	}
	else return -1;
	pos->j_im = pos->j_real;


	pos->posKey = GeneratePosKey(pos); 
	
	UpdateListsMaterial(pos);
	
	return 0;
}

void ResetBoard(TABLERO *pos) {

	int i = 0;

	for(i = 0; i < NUM_CASILLAS; ++i) {
		pos->pieces[i] = OFFBOARD;
	}

	for(i = 0; i < 64; ++i) {
		pos->pieces[C64a120(i)] = EMPTY;
	}

	pos->KingSq[WHITE] = pos->KingSq[BLACK] = NO_SQ;
	
	pos->side = BOTH;
	pos->AlPaso = NO_SQ;
	pos->fiftyMove = 0;
	
	pos->j_im = 0;
	pos->j_real = 0;
	
	pos->enroque = 0;
	
	//pos->posKey = 0ULL;

	for(i = 0; i < 2; ++i) {
		pos->bigPce[i] = 0;
		pos->majPce[i] = 0;
		pos->minPce[i] = 0;
	//	pos->pawns[i] = 0ULL;
	}
	
	for(i = 0; i < 13; ++i) {
		pos->pceNum[i] = 0;
	}
	//Falta piece list ya veremos que hacemos con eso más tarde
	
}
void PrintBoard(const TABLERO *pos) {
	
	int sq,fila,col,piece;

	printf("\nGame Board:\n\n");
	
	for(fila = FILA_8; fila >= FILA_1; fila--) {
		printf("%d  ",col+1);
		for(col = COL_A; col <= COL_H; col++) {
			sq = FCCAS(col,fila);
			piece = pos->pieces[sq];
			printf("%3c",PceChar[piece]);
		}
		printf("\n");
	}
	
	printf("\n   ");
	for(col = COL_A; col <= COL_H; col++) {
		printf("%3c",'a'+col);	
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