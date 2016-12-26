/**
 * SetMinutes_CH.h
 * Klasse fuer das Setzten der Woerter fuer die Minuten. Hier: Schweizerisch.
 *
 * @mc       Arduino/RBBB
 * @autor    Thomas Schuler / thomas.schuler _AT_ vtg _DOT_ admin _DOT_ ch
 * @version  1.0
 * @created  18.3.2012
 *
 */
#ifndef SETMINUTES_CH_H
#define SETMINUTES_CH_H

#include "SetHours_CH.h"

/**
 * Setzt die Wortminuten, je nach hours/minutes.
 */
void setMinutes(int hours, int minutes) {
  while (hours > 12) {
    hours -= 12;
  }

  ESIST;

  switch (minutes / 5) {
  case 0:
    // glatte Stunde
    setHoures(hours, true);
    break;
  case 1:
    // 5 ab
    FUENF;
    AB;
    setHoures(hours, false);
    break;
  case 2:
    // 10 ab
    ZEHN;
    AB;
    setHoures(hours, false);
    break;
  case 3:
    // viertel ab
    VIERTEL;
    AB;
    setHoures(hours, false);
    break;
  case 4:
    // 20 ab
    ZWANZIG;
    AB;
    setHoures(hours, false);
    break;
  case 5:
    // 5 vor halb
    FUENF;
    VOR;
    HALB;
    setHoures(hours + 1, false);
    break;
  case 6:
    // halb
    HALB;
    setHoures(hours + 1, false);
    break;
  case 7:
    // 5 ab halb
    FUENF;
    AB;
    HALB;
    setHoures(hours + 1, false);
    break;
  case 8:
    // 20 vor
    ZWANZIG;
    VOR;
    setHoures(hours + 1, false);
    break;
  case 9:
    // viertel vor
    VIERTEL;
    VOR;
    setHoures(hours + 1, false);
    break;
  case 10:
    // 10 vor
    ZEHN;
    VOR;
    setHoures(hours + 1, false);
    break;
  case 11:
    // 5 vor
    FUENF;
    VOR;
    setHoures(hours + 1, false);
    break;
  }
}

#endif

