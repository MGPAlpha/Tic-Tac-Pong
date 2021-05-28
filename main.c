#include "main.h"

#include <stdio.h>
#include <stdlib.h>

#include "gba.h"

#include "game.h"

// Include any header files for title screen or exit
// screen images generated by nin10kit. Example for the provided garbage
// image:
#include "images/splash.h"
#include "images/winscreen.h"
#include "images/losescreen.h"

// Add any additional states you need for your app. You are not requried to use
// these specific provided states.
enum gba_state state;

int main(void) {
  // Manipulate REG_DISPCNT here to set Mode 3. //
  REG_DISPCNT = MODE3 | BG2_ENABLE;

  // Save current and previous state of button input.
  u32 previousButtons = BUTTONS;
  u32 currentButtons = BUTTONS;

  int selectCounter = 0;

  // Load initial application state
  state = ENTER_START;

  while (1) {
    currentButtons = BUTTONS; // Load the current state of the buttons

    // Manipulate the state machine below as needed //
    // NOTE: Call waitForVBlank() before you draw
    switch (state) {
      case ENTER_START:
        waitForVBlank();
        drawFullScreenImageDMA(splash);
        state = START;
      case START:
        if (KEY_JUST_PRESSED(BUTTON_START, currentButtons, previousButtons)) {
          state = ENTER_PLAY;
        }
        waitForVBlank();
        break;
      case ENTER_PLAY:
        gameInit();
        state = PLAY;
      case PLAY:
        gameUpdate(currentButtons, previousButtons);
        break;
      case ENTER_WIN:
        waitForVBlank();
        drawFullScreenImageDMA(winscreen);
        state = WIN;
      case WIN:
        waitForVBlank();
        break;
      case ENTER_LOSE:
        waitForVBlank();
        drawFullScreenImageDMA(losescreen);
        state = LOSE;
      case LOSE:
        waitForVBlank();
        break;
      default:
        UNUSED(state);
    }

    if (KEY_DOWN(BUTTON_SELECT, currentButtons)) {
      selectCounter++;
    }
    else selectCounter = 0;

    if (selectCounter >= 60) {
      state = ENTER_START;
      selectCounter = 0;
    }

    previousButtons = currentButtons; // Store the current state of the buttons
  }

  UNUSED(previousButtons); // You can remove this once previousButtons is used
  return 0;
}
