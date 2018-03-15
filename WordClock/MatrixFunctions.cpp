#include "MatrixFunctions.h"

void clearNeo(Adafruit_NeoPixel* neo){
	for(int i=0; i<neo->numPixels(); i++)
		neo->setPixelColor(i, 0, 0, 0);
}

void clearMatrix(PATTERN* in){
	*in = View_Blank;
}

void mergeMatrix(PATTERN* in1, PATTERN* in2){
	for(int i=0; i<10; i++){
		in1->r[i] = in1->r[i] | in2->r[i];
	}
}

void writeMatrix(PATTERN* in, char C, unsigned short row, unsigned short shiftLeft){
	if(row > 5 or shiftLeft > 10) return;
	
	int sign[5];
	switch(C){
		case ':':{
			sign[0] = 0b0;
			sign[1] = 0b1;
			sign[2] = 0b0;
			sign[3] = 0b1;
			sign[4] = 0b0;
			break;}
		case '\'':{
			sign[0] = 0b1;
			sign[1] = 0b0;
			sign[2] = 0b0;
			sign[3] = 0b0;
			sign[4] = 0b0;
			break;}
		case 1:
		case '1':{
			sign[0] = 0b01;
			sign[1] = 0b11;
			sign[2] = 0b01;
			sign[3] = 0b01;
			sign[4] = 0b01;
			break;}
		case 2:
		case '2':{
			sign[0] = 0b111;
			sign[1] = 0b001;
			sign[2] = 0b111;
			sign[3] = 0b100;
			sign[4] = 0b111;
			break;}
		case 3:
		case '3':{
			sign[0] = 0b111;
			sign[1] = 0b001;
			sign[2] = 0b111;
			sign[3] = 0b001;
			sign[4] = 0b111;
			break;}
		case 4:
		case '4':{
			sign[0] = 0b101;
			sign[1] = 0b101;
			sign[2] = 0b111;
			sign[3] = 0b001;
			sign[4] = 0b001;
			break;}
		case 5:
		case '5':{
			sign[0] = 0b111;
			sign[1] = 0b100;
			sign[2] = 0b111;
			sign[3] = 0b001;
			sign[4] = 0b111;
			break;}
		case 6:
		case '6':{
			sign[0] = 0b010;
			sign[1] = 0b100;
			sign[2] = 0b111;
			sign[3] = 0b101;
			sign[4] = 0b111;
			break;}
		case 7:
		case '7':{
			sign[0] = 0b111;
			sign[1] = 0b001;
			sign[2] = 0b001;
			sign[3] = 0b010;
			sign[4] = 0b010;
			break;}
		case 8:
		case '8':{
			sign[0] = 0b111;
			sign[1] = 0b101;
			sign[2] = 0b111;
			sign[3] = 0b101;
			sign[4] = 0b111;
			break;}
		case 9:
		case '9':{
			sign[0] = 0b111;
			sign[1] = 0b101;
			sign[2] = 0b111;
			sign[3] = 0b001;
			sign[4] = 0b010;
			break;}
		case 0:
		case 'O':
		case '0':{
			sign[0] = 0b010;
			sign[1] = 0b101;
			sign[2] = 0b101;
			sign[3] = 0b101;
			sign[4] = 0b010;
			break;}
		case 'A':{
			sign[0] = 0b010;
			sign[1] = 0b101;
			sign[2] = 0b111;
			sign[3] = 0b101;
			sign[4] = 0b101;
			break;}
		case 'B':{
			sign[0] = 0b110;
			sign[1] = 0b101;
			sign[2] = 0b110;
			sign[3] = 0b101;
			sign[4] = 0b110;
			break;}
		case 'C':{
			sign[0] = 0b011;
			sign[1] = 0b100;
			sign[2] = 0b100;
			sign[3] = 0b100;
			sign[4] = 0b011;
			break;}
		case 'D':{
			sign[0] = 0b110;
			sign[1] = 0b101;
			sign[2] = 0b101;
			sign[3] = 0b101;
			sign[4] = 0b110;
			break;}
		case 'E':{
			sign[0] = 0b011;
			sign[1] = 0b100;
			sign[2] = 0b111;
			sign[3] = 0b100;
			sign[4] = 0b011;
			break;}
		case 'F':{
			sign[0] = 0b111;
			sign[1] = 0b100;
			sign[2] = 0b111;
			sign[3] = 0b100;
			sign[4] = 0b100;
			break;}
		case 'G':{
			sign[0] = 0b011;
			sign[1] = 0b100;
			sign[2] = 0b100;
			sign[3] = 0b101;
			sign[4] = 0b011;
			break;}
		case 'H':{
			sign[0] = 0b101;
			sign[1] = 0b101;
			sign[2] = 0b111;
			sign[3] = 0b101;
			sign[4] = 0b101;
			break;}
		case 'I':{
			sign[0] = 0b1;
			sign[1] = 0b1;
			sign[2] = 0b1;
			sign[3] = 0b1;
			sign[4] = 0b1;
			break;}
		case 'J':{
			sign[0] = 0b01;
			sign[1] = 0b01;
			sign[2] = 0b01;
			sign[3] = 0b01;
			sign[4] = 0b11;
			break;}
		case 'K':{
			sign[0] = 0b101;
			sign[1] = 0b110;
			sign[2] = 0b100;
			sign[3] = 0b110;
			sign[4] = 0b101;
			break;}
		case 'L':{
			sign[0] = 0b100;
			sign[1] = 0b100;
			sign[2] = 0b100;
			sign[3] = 0b100;
			sign[4] = 0b111;
			break;}
		case 'M':{
			sign[0] = 0b10001;
			sign[1] = 0b11011;
			sign[2] = 0b10101;
			sign[3] = 0b10001;
			sign[4] = 0b10001;
			break;}
		case 'N':{
			sign[0] = 0b10001;
			sign[1] = 0b11001;
			sign[2] = 0b10101;
			sign[3] = 0b10011;
			sign[4] = 0b10001;
			break;}
		case 'P':{
			sign[0] = 0b110;
			sign[1] = 0b101;
			sign[2] = 0b110;
			sign[3] = 0b100;
			sign[4] = 0b100;
			break;}
		case 'Q':{
			sign[0] = 0b010;
			sign[1] = 0b101;
			sign[2] = 0b101;
			sign[3] = 0b101;
			sign[4] = 0b011;
			break;}
		case 'R':{
			sign[0] = 0b110;
			sign[1] = 0b101;
			sign[2] = 0b110;
			sign[3] = 0b101;
			sign[4] = 0b101;
			break;}
		case 'S':{
			sign[0] = 0b011;
			sign[1] = 0b100;
			sign[2] = 0b010;
			sign[3] = 0b001;
			sign[4] = 0b110;
			break;}
		case 'T':{
			sign[0] = 0b111;
			sign[1] = 0b010;
			sign[2] = 0b010;
			sign[3] = 0b010;
			sign[4] = 0b010;
			break;}
		case 'U':{
			sign[0] = 0b101;
			sign[1] = 0b101;
			sign[2] = 0b101;
			sign[3] = 0b101;
			sign[4] = 0b111;
			break;}
		case 'V':{
			sign[0] = 0b10001;
			sign[1] = 0b10001;
			sign[2] = 0b01010;
			sign[3] = 0b01010;
			sign[4] = 0b00100;
			break;}
		case 'W':{
			sign[0] = 0b10001;
			sign[1] = 0b10001;
			sign[2] = 0b10101;
			sign[3] = 0b10101;
			sign[4] = 0b01010;
			break;}
		case 'X':{
			sign[0] = 0b101;
			sign[1] = 0b101;
			sign[2] = 0b010;
			sign[3] = 0b101;
			sign[4] = 0b101;
			break;}
		case 'Y':{
			sign[0] = 0b101;
			sign[1] = 0b101;
			sign[2] = 0b010;
			sign[3] = 0b010;
			sign[4] = 0b010;
			break;}
		case 'Z':{
			sign[0] = 0b111;
			sign[1] = 0b001;
			sign[2] = 0b010;
			sign[3] = 0b100;
			sign[4] = 0b111;
			break;}
		default: return;
	}
	if(row == 0) sign[4] = sign[4] << 1;
	if(row == 5) sign[0] = sign[0] << 1;
	for(int i=0; i<5; i++)
		in->r[5+i-row] = in->r[5+i-row] | (sign[i] << shiftLeft);
}

void printMatrix(PATTERN* matrix){
//	for(int row=0; row<10; row++){
//		int cor = 0;
//		for(int col=0; col<13; col++){
//			if(row > 0 and row < 9)
//				cor = 1;
//			if(cor > 0 and (col == 0 or col == 12))
//				Serial.print(" - ");
//			else{
//				if( (matrix->r[row] >> (12-col-cor)) & 1 )
//					Serial.print(" X ");
//				else
//					Serial.print(" . ");
//			}
//		}
//		Serial.println();
//	}
}

void generateClockMatrix(PATTERN* matrix, unsigned short h, unsigned short m) {
	if(h > 12)
		h -= 12;
	
	clearMatrix(matrix);
	// Standard-Text (immer sichtbar)
	mergeMatrix(matrix, &View_EsIst);
	// Einzelne Minuten (1-4)
	switch(m%5){
		case 1:mergeMatrix(matrix, &View_Min_1);break;
		case 2:mergeMatrix(matrix, &View_Min_2);break;
		case 3:mergeMatrix(matrix, &View_Min_3);break;
		case 4:mergeMatrix(matrix, &View_Min_4);break;
	}
	// Minuten: 5er-Schritte
	int h_offset = 0;
	switch(m/5){
		case  0:mergeMatrix(matrix, &View_Uhr);break; // ??:00
		case  1:mergeMatrix(matrix, &View_Min05);break; // ??:05
		case  2:mergeMatrix(matrix, &View_Min10);break; // ??:15
		case  3:mergeMatrix(matrix, &View_Min15);break; // ??:15
		case  4:mergeMatrix(matrix, &View_Min20);break; // ??:20
		case  5:mergeMatrix(matrix, &View_Min25);h_offset=1;break; // ??:25
		case  6:mergeMatrix(matrix, &View_Min30);h_offset=1;break; // ??:30
		case  7:mergeMatrix(matrix, &View_Min35);h_offset=1;break; // ??:35
		case  8:mergeMatrix(matrix, &View_Min40);h_offset=1;break; // ??:40
		case  9:mergeMatrix(matrix, &View_Min45);h_offset=1;break; // ??:45
		case 10:mergeMatrix(matrix, &View_Min50);h_offset=1;break; // ??:50
		case 11:mergeMatrix(matrix, &View_Min55);h_offset=1;break; // ??:55
	}
	// Stunden
	switch(h + h_offset){
		case  1:mergeMatrix(matrix, &View_Hour01);break;
		case  2:mergeMatrix(matrix, &View_Hour02);break;
		case  3:mergeMatrix(matrix, &View_Hour03);break;
		case  4:mergeMatrix(matrix, &View_Hour04);break;
		case  5:mergeMatrix(matrix, &View_Hour05);break;
		case  6:mergeMatrix(matrix, &View_Hour06);break;
		case  7:mergeMatrix(matrix, &View_Hour07);break;
		case  8:mergeMatrix(matrix, &View_Hour08);break;
		case  9:mergeMatrix(matrix, &View_Hour09);break;
		case 10:mergeMatrix(matrix, &View_Hour10);break;
		case 11:mergeMatrix(matrix, &View_Hour11);break;
		default:mergeMatrix(matrix, &View_Hour12);break;
	}
	// Stunden-Korrektur 'Eins' -> 'Ein'
	if(h%12 == 1 and m/5 == 0)	matrix->r[4] = matrix->r[4] & 0b11101111111;
}

void writeNeo(PATTERN* matrix, Adafruit_NeoPixel* neo, RGB* color, bool writeFalse){
	int px = 0;
	for(int row=0; row<10; row++){
		int cor = 0;
		for(int col=0; col<13; col++){
			if(row > 0 and row < 9)
				cor = 1;
			if(not(cor > 0 and (col == 0 or col == 12))){
				int shift;
				if(row%2 == 0)
					shift = 12-col-cor;
				else
					shift = col-cor;
				
				if( (matrix->r[row] >> shift) & 1 )
					neo->setPixelColor(px, neo->Color(color->r, color->g, color->b));
				else{
					if(writeFalse)
						neo->setPixelColor(px, 0, 0, 0);
				}
				px++;
			}
		}
	}
}


