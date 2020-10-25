
#include "stdio.h"
#include "definiciones.h"

//char PceChar[] = ".♙♘♗♖♕♔♟♞♝♜♛♚";
char PceChar[] = ".PNBRQKpnbrqk";
char SideChar[] = "wb-";
char RankChar[] = "12345678";
char FileChar[] = "abcdefgh";

int PieceBig[13] = { FALSE, FALSE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, TRUE, TRUE, TRUE, TRUE, TRUE };
int PieceMaj[13] = { FALSE, FALSE, FALSE, FALSE, TRUE, TRUE, TRUE, FALSE, FALSE, FALSE, TRUE, TRUE, TRUE };
int PieceMin[13] = { FALSE, FALSE, TRUE, TRUE, FALSE, FALSE, FALSE, FALSE, TRUE, TRUE, FALSE, FALSE, FALSE };
int PieceVal[13]= { 0, 100, 325, 325, 550, 1000, 50000, 100, 325, 325, 550, 1000, 50000  };
int PieceCol[13] = { BOTH, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE,
	BLACK, BLACK, BLACK, BLACK, BLACK, BLACK };



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
			FILAsBrd[sq] = fila;
			COLsBrd[sq] = col;
		}
	}
}

int Cas_Fila (int cas){
	return FILAsBrd[cas];
}

int Cas_Col (int cas){
	return COLsBrd[cas];
}



int pieceColour(int pce){

    if(pce == wP || pce == wR || pce == wB || pce == wN || pce == wQ || pce == wK) return WHITE;

    else if(pce == bP || pce == bR || pce == bB || pce == bN || pce == bQ || pce == bK) return BLACK;

    return BOTH;
}

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

/*Función para dada una posicion en el tablero, mediante el uso de una serie de arrays y variables internas, crea una posición "espejo" para 
luego comprobar que coincide con la dada y que así se la informacion almacenada en cada una de las martes de estructura 
tablero que es redundante es crrecta*/
int CheckBoard(const TABLERO *pos) {   
	
 
	int esp_pceNum[13] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	int esp_material[2] = { 0, 0};
	
	int sq64,esp_piece,esp_pce_num,sq120,colour;

	for(esp_piece = wP; esp_piece <= bK; ++esp_piece) {
		for(esp_pce_num = 0; esp_pce_num < pos->pceNum[esp_piece]; ++esp_pce_num) {
			sq120 = pos->pList[esp_piece][esp_pce_num];
			ASSERT(pos->pieces[sq120]==esp_piece);
		}	
	}


	for(sq64 = 0; sq64 < 64; ++sq64) {
		sq120 = C64a120(sq64);
		esp_piece = pos->pieces[sq120];
		esp_pceNum[esp_piece]++;
		colour = pieceColour(esp_piece);
		
		esp_material[colour] += PieceVal[esp_piece];
	}
	
	for(esp_piece = wP; esp_piece <= bK; ++esp_piece) {
		ASSERT(esp_pceNum[esp_piece]==pos->pceNum[esp_piece]);	
	}
	

	ASSERT(esp_material[WHITE]==pos->material[WHITE] && esp_material[BLACK]==pos->material[BLACK]);

	
	ASSERT(pos->side==WHITE || pos->side==BLACK);

	
	ASSERT(pos->AlPaso==NO_SQ || ( FILAsBrd[pos->AlPaso]==FILA_6 && pos->side == WHITE)
		 || ( FILAsBrd[pos->AlPaso]==FILA_3 && pos->side == BLACK));
	
	ASSERT(pos->pieces[pos->KingSq[WHITE]] == wK);
	ASSERT(pos->pieces[pos->KingSq[BLACK]] == bK);
		 
	return TRUE;	
}



void UpdateListsMaterial(TABLERO *pos) {	
	
	int pieza,cas,i,color;
	
	pos->material[0] = 0;
	pos->material[1] = 0;

	for (i = 0; i < 13 ;i++) {
		pos->pceNum[i] = 0;
	}

	for(i = 0; i < NUM_CASILLAS; i++) {
		cas = i;
		pieza = pos->pieces[i];

		if(pieza!=OFFBOARD && pieza!= EMPTY) {
			if(pieza <= wK) color = WHITE;
			else color = BLACK;

			pos->material[color] += PieceVal[pieza];

			pos->pList[pieza][pos->pceNum[pieza]] = cas;

			pos->pceNum[pieza]++;

			if(pieza==wK) pos->KingSq[WHITE] = cas;
			if(pieza==bK) pos->KingSq[BLACK] = cas;	
			
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
		pos->j_real = 2*(fen[0]-'0') + pos->side-1;
	}
	else if(fen[2] == '\0'){
		pos->j_real = 2*((fen[1]-'0')*10 + (fen[0]-'0')) + pos->side-1;
	}
	else if(fen[3] == '\0'){
		pos->j_real = 2*((fen[2]-'0')*100 + (fen[1]-'0')*10 + (fen[0]-'0')) + pos->side-1;
	}
	else if (fen[4] == '\0'){
		pos->j_real = 2*((fen[3]-'0')*1000 + (fen[2]-'0')*100 + (fen[1]-'0')*10 + (fen[0]-'0')) + pos->side -1;
	}
	else return -1;
	pos->j_im = pos->j_real;
	
	UpdateListsMaterial(pos);
	
	return 0;
}

void ResetBoard(TABLERO *pos) {

	int i = 0;
	int j = 0;

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
	pos->material[0] = 0;
	pos->material[1] = 0;
	
	
	for(i = 0; i < 12; ++i) {
		pos->pceNum[i] = 0;
	}
	for(i=0;i<13;i++){
		for(j=0;j<10;j++){

			pos->pList[i][j]=0;	
		}
	}
	pos->KingSq[0]=0;
	pos->KingSq[1]=0;

	
}

void PrintBoard(const TABLERO *pos) {
	
	int pieza,fila,col,piezan;

	printf("\nGame Board:\n\n");
	
	for(fila = FILA_8; fila >= FILA_1; fila--) {
		printf("%d  ",fila+1);
		for(col = COL_A; col <= COL_H; col++) {
			pieza = FCCAS(col,fila);
			piezan = pos->pieces[pieza];    
            if(piezan==0)
			printf(". ");
			if(piezan==1)
			printf("♟ ");
			if(piezan==2)
			printf("♞ ");
			if(piezan==3)
			printf("♝ ");
			if(piezan==4)
			printf("♜ ");
			if(piezan==5)
			printf("♛ ");
			if(piezan==6)
			printf("♚ ");
			if(piezan==7)
			printf("♙ ");
			if(piezan==8)
			printf("♘ ");
			if(piezan==9)
			printf("♗ ");
			if(piezan==10)
			printf("♖ ");
			if(piezan==11)
			printf("♕ ");
			if(piezan==12)
			printf("♔ ");
		
			printf(" ");
		}
		printf("\n");
	}
	
	printf("\n ");
	for(col = COL_A; col <= COL_H; col++) {
		printf("%3c",'a'+col);	
	}
	printf("\n");
	printf("side:%c\n",SideChar[pos->side]);
	if(pos->AlPaso==99)printf("AlPaso:NO\n");
	else printf("AlPaso:%d\n",pos->AlPaso);
	printf("castle:%c%c%c%c\n",
			pos->enroque & WKCA ? 'K' : '-',
			pos->enroque & WQCA ? 'Q' : '-',
			pos->enroque & BKCA ? 'k' : '-',
			pos->enroque & BQCA ? 'q' : '-'	
			);
}

TABLERO* Create_tablero(){

	TABLERO *tab=NULL;
	int i;


	tab=(TABLERO*)malloc(sizeof(TABLERO));
	if(!tab) 
		return NULL;

	tab->KingSq=(int*)malloc(2*sizeof(int));
	tab->pceNum=(int*)malloc(13*sizeof(int));
	tab->material=(int*)malloc(2*sizeof(int));
	tab->pieces=(int*)malloc(NUM_CASILLAS*sizeof(int));

	if(!tab->material||!tab->pceNum||!tab->KingSq){
		
		
		Free_tablero(tab);
		return NULL;
	}

	tab->pList=(int**)malloc(13*sizeof(int*));
	if(!tab->pList){

		Free_tablero(tab);
		return NULL;
	}

	for(i=0;i<13;i++){

	tab->pList[i]=(int*)malloc(10*sizeof(int));

	if(!tab->pList[i]){

		Free_tablero(tab);
		return NULL;

	}
	}


	return tab;
}

void Free_tablero(TABLERO *tab){
	int i;

	if(tab->pieces)free(tab->pieces);


	if(tab->KingSq)free(tab->KingSq);

	if(tab->material)free(tab->material);

	if(tab->pceNum)free(tab->pceNum);

	if(tab->pList){
		
		for(i=0;i<13;i++){
			if(tab->pList[i])free(tab->pList[i]);
		}
		free(tab->pList);
	}

	if(tab)free(tab);
	return;

}
