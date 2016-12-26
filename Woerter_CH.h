/**
 * Woerter_CH.h
 * Definition der schweizerischen Woerter fuer die Zeitansage.
 * Die Woerter sind Bitmasken fuer die Matrix.
 *
 * @mc       Arduino/RBBB
 * @autor    Thomas Schuler / thomas.schuler _AT_ vtg _DOT_ admin _DOT_ ch
 * @version  1.0
 * @created  18.3.2012
 */
#ifndef WOERTER_CH_H
#define WOERTER_CH_H

/**
 * Definition der Woerter
 */
#define VOR          matrix[2] |= 0b0000000111000000
#define AB           matrix[3] |= 0b1100000000000000
#define ESIST        matrix[0] |= 0b1101111000000000
#define UHR          matrix[9] |= 0b0000000000000000

#define FUENF        matrix[0] |= 0b0000000011100000
#define ZEHN         matrix[1] |= 0b0000000011100000
#define VIERTEL      matrix[1] |= 0b1111110000000000
#define ZWANZIG      matrix[2] |= 0b1111110000000000
#define HALB         matrix[3] |= 0b0001111100000000

#define H_EIN        matrix[4] |= 0b1110000000000000
#define H_EINS       matrix[4] |= 0b1110000000000000
#define H_ZWEI       matrix[4] |= 0b0001111000000000
#define H_DREI       matrix[4] |= 0b0000000011100000
#define H_VIER       matrix[5] |= 0b1111100000000000
#define H_FUENF      matrix[5] |= 0b0000001111000000
#define H_SECHS      matrix[6] |= 0b1111110000000000
#define H_SIEBEN     matrix[6] |= 0b0000001111100000
#define H_ACHT       matrix[7] |= 0b1111100000000000
#define H_NEUN       matrix[7] |= 0b0000001111000000
#define H_ZEHN       matrix[8] |= 0b1111000000000000
#define H_ELF        matrix[8] |= 0b0000000111100000
#define H_ZWOELF     matrix[9] |= 0b1111110000000000

#endif

