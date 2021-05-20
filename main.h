#ifndef MAIN_H
#define MAIN_H

#include "gba.h"

/*
* For example, for a Snake game, one could be:
*
* struct snake {
*   int heading;
*   int length;
*   int row;
*   int col;
* };
*
* Example of a struct to hold state machine data:
*
* struct state {
*   int currentState;
*   int nextState;
* };
*
*/

// Add any additional states you need for your app. You are not requried to use
// these specific provided states.
extern enum gba_state {
    ENTER_START,
    START,
    ENTER_PLAY,
    PLAY,
    ENTER_WIN,
    WIN,
    ENTER_LOSE,
    LOSE,
} state;

#endif
