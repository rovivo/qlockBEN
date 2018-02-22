#ifndef MatrixFunctions_h
#define MatrixFunctions_h

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "Constants.h"

void clearNeo(Adafruit_NeoPixel* neo);

void clearMatrix(PATTERN* in);

void mergeMatrix(PATTERN* in1, PATTERN* in2);

void writeMatrix(PATTERN* in, char C, unsigned short row=0, unsigned short shiftLeft=0);

void printMatrix(PATTERN* matrix);

void generateClockMatrix(PATTERN* matrix, unsigned short h, unsigned short m);

void writeNeo(PATTERN* matrix, Adafruit_NeoPixel* neo, RGB* color, bool writeFalse=false);

#endif