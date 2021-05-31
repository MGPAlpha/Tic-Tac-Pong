//
// Created by mitchell on 4/5/21.
//

#include "game.h"
#include "gba.h"
#include "main.h"
#include "ai.h"
#include "sound.h"
#include "sfx.h"
#include <stdlib.h>
#include <stdio.h>

#include "images/battery.h"
#include "images/x.h"
#include "images/o.h"

struct body playerPaddle;
struct body comPaddle;

struct body pong;

struct player player;
struct player com;

struct player *symbols[3][3];
struct rect symbolColliders[3][3];

struct player *pongCharge;

struct player *gameWinner;
struct vector2 winningSymbols[3];
int wonByScore = 0;

int gameEndFrameCount = 0;

void gameInit(void) {
  resetPaddle(&playerPaddle, LEFT_PADDLE_COL);
  resetPaddle(&comPaddle, RIGHT_PADDLE_COL);
  resetPong(1);
  resetPlayer(&player, CYAN, x);
  resetPlayer(&com, MAGENTA, o);
  resetSymbols();
  resetAI();
  gameWinner = NULL;
  wonByScore = 0;
  gameEndFrameCount = 0;
  waitForVBlank();
  fillScreenDMA(COLOR(0,0,0));
  waitForVBlank(); // LOAD BEARING TOMATO!! If this vblank is removed, the background is dark blue for some reason
}

void gameUpdate(int currentButtons, int previousButtons) {
  if (!gameWinner) {
    if (KEY_JUST_PRESSED(BUTTON_A, currentButtons, previousButtons) && player.energy >= MAX_ENERGY) {
      player.energy = 0;
      player.charged = 1;
      playSoundEffect(&chargePaddle);
    }
    if (com.energy >= MAX_ENERGY) {
      com.energy = 0;
      com.charged = 1;
      playSoundEffect(&chargePaddle);
    }
    int controlDir = (KEY_DOWN(BUTTON_UP, currentButtons) ? -1 : 0) + (KEY_DOWN(BUTTON_DOWN, currentButtons) ? 1 : 0);
    controlPaddle(&playerPaddle, controlDir);
    int comDir = controlAI(comPaddle, pong, pongCharge != NULL);
    controlPaddle(&comPaddle, comDir);

    updatePong();
  }

  int playerTotal = totalMarks(&player);
  int comTotal = totalMarks(&com);
  char playerTotalString[11];
  char comTotalString[11];
  sprintf(playerTotalString, "%d", playerTotal);
  sprintf(comTotalString, "%d", comTotal);

  if (!gameWinner) {
    struct player *winner = checkForWin();
    if (!winner && checkBoardFull()) {
      winner = (playerTotal > comTotal) ? &player : &com;
      wonByScore = 1;
    }
    if (winner) {
      setWinningSymbols();
      playSoundEffect(winner == &player ? &win : &lose);
    }
    gameWinner = winner;
  } else {
    gameEndFrameCount++;
    if (gameEndFrameCount >= GAME_END_FRAMES) {
      state = (gameWinner == &player) ? ENTER_WIN : ENTER_LOSE;
    }
  }

  waitForVBlank();

  undrawBody(&playerPaddle);
  undrawBody(&comPaddle);
  undrawBody(&pong);

  undrawScores();

  drawBoard();
  drawBorders();
  drawSymbols();

  drawBody(&playerPaddle, player.charged ? (BLINK_VALUE(vBlankCounter, WHITE, player.color)) : WHITE);
  drawBody(&comPaddle, com.charged ? (BLINK_VALUE(vBlankCounter, WHITE, com.color)) : WHITE);
  if (!gameWinner) drawBody(&pong, pongCharge ? (BLINK_VALUE(vBlankCounter, WHITE, pongCharge->color)) : WHITE);

  drawBatteries();
  drawString(SCORE_ROW, LEFT_SCORE_COL, playerTotalString, WHITE);
  drawString(SCORE_ROW, RIGHT_SCORE_COL, comTotalString, WHITE);

}

void drawBoard(void) {
  drawRectDMA(HORIZ_0_ROW, BOARD_START_COL, BOARD_SIZE, 2, BOARD_COLOR);
  drawRectDMA(HORIZ_1_ROW, BOARD_START_COL, BOARD_SIZE, 2, BOARD_COLOR);
  for (int i = 0; i < BOARD_SIZE; i++) {
    setPixel(BOARD_START_ROW + i, VERT_0_COL, BOARD_COLOR);
    setPixel(BOARD_START_ROW + i, VERT_0_COL + 1, BOARD_COLOR);
    setPixel(BOARD_START_ROW + i, VERT_1_COL, BOARD_COLOR);
    setPixel(BOARD_START_ROW + i, VERT_1_COL + 1, BOARD_COLOR);
  }
}

void drawBorders(void) {
  drawRectDMA(0, 0, BORDER_WIDTH, HEIGHT, player.color);
  drawRectDMA(0, WIDTH - BORDER_WIDTH, BORDER_WIDTH, HEIGHT, com.color);
}

void drawBatteries(void) {
  const unsigned short *playerFrame = battery + FRAME_INDEX(player.energy < MAX_ENERGY ? player.energy / 2 : BLINK_VALUE(vBlankCounter, 6, 7), BATTERY_WIDTH, BATTERY_FRAME_HEIGHT);
  const unsigned short *comFrame = battery + FRAME_INDEX(com.energy < MAX_ENERGY ? com.energy / 2 : BLINK_VALUE(vBlankCounter, 6, 7), BATTERY_WIDTH, BATTERY_FRAME_HEIGHT);
  drawImageDMA(BATTERY_MARGIN, BATTERY_MARGIN, BATTERY_WIDTH, BATTERY_FRAME_HEIGHT, playerFrame);
  drawImageDMA(BATTERY_MARGIN, WIDTH - BATTERY_MARGIN - BATTERY_WIDTH, BATTERY_WIDTH, BATTERY_FRAME_HEIGHT, comFrame);
}

void drawSymbols(void) {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (symbols[i][j] == NULL) {
        drawRectDMA((HEIGHT - BLANK_SYMBOL_SIZE) / 2 + (i-1) * BOARD_CELL_SIZE, (WIDTH - BLANK_SYMBOL_SIZE) / 2 + (j-1) * BOARD_CELL_SIZE, BLANK_SYMBOL_SIZE, BLANK_SYMBOL_SIZE, (pongCharge) ? BLINK_VALUE(vBlankCounter, WHITE, pongCharge->color) : GRAY);
      } else {
        if (symbols[i][j] == gameWinner && (wonByScore || isInWinningSymbols(i, j)) && vBlankCounter % WINNER_BLINK_INTERVAL > WINNER_BLINK_INTERVAL / 2) {
          drawRectDMA((HEIGHT - SYMBOL_SIZE) / 2 + (i-1) * BOARD_CELL_SIZE, (WIDTH - SYMBOL_SIZE) / 2 + (j-1) * BOARD_CELL_SIZE, SYMBOL_SIZE, SYMBOL_SIZE, BLACK);
        } else {
          drawImageDMA((HEIGHT - SYMBOL_SIZE) / 2 + (i-1) * BOARD_CELL_SIZE, (WIDTH - SYMBOL_SIZE) / 2 + (j-1) * BOARD_CELL_SIZE, SYMBOL_SIZE, SYMBOL_SIZE, symbols[i][j]->symbol);
        }
      }
    }
  }
}

void resetPaddle(struct body *paddle, int col) {
  paddle->pastTransform = paddle->transform;

  paddle->transform.row = TO_PHYS_COORD((HEIGHT - PADDLE_SIZE) / 2);
  paddle->transform.col = TO_PHYS_COORD(col);
  paddle->transform.height = TO_PHYS_COORD(PADDLE_SIZE);
  paddle->transform.width = TO_PHYS_COORD(PADDLE_WIDTH);

  paddle->velocity.x = 0;
  paddle->velocity.y = 0;
}

void controlPaddle(struct body *paddle, int dir) {
  paddle->pastTransform = paddle->transform;
  if (dir != 0) {
    dir = dir/abs(dir);
    paddle->velocity.y += PADDLE_ACCEL * dir;
  } else if (abs(paddle->velocity.y) <= PADDLE_DRAG) {
    paddle->velocity.y = 0;
  } else if (paddle->velocity.y) {
    paddle->velocity.y += (paddle->velocity.y > 0 ? -1 : 1) * PADDLE_DRAG;
  }

  paddle->transform.row += paddle->velocity.y;

  if (paddle->transform.row < 0) {
    paddle->transform.row = 0;
    paddle->velocity.y = 0;
  } else if (paddle->transform.row > TO_PHYS_COORD(HEIGHT) - paddle->transform.height) {
    paddle->transform.row = TO_PHYS_COORD(HEIGHT) - paddle->transform.height;
    paddle->velocity.y = 0;
  }
}

void undrawBody(struct body *paddle) {
  int row = TO_SCREEN_COORD(paddle->pastTransform.row);
  int col = TO_SCREEN_COORD(paddle->pastTransform.col);
  int width = TO_SCREEN_COORD(paddle->pastTransform.width);
  int height = TO_SCREEN_COORD(paddle->pastTransform.height);
  drawRectDMA(row, col, width, height, BLACK);
}

void drawBody(struct body *body, unsigned short color) {
  int row = TO_SCREEN_COORD(body->transform.row);
  int col = TO_SCREEN_COORD(body->transform.col);
  int width = TO_SCREEN_COORD(body->transform.width);
  int height = TO_SCREEN_COORD(body->transform.height);
  drawRectDMA(row, col, width, height, color);
}

void undrawScores(void) {
  drawRectDMA(SCORE_ROW, LEFT_SCORE_COL, 6, 8, BLACK);
  drawRectDMA(SCORE_ROW, RIGHT_SCORE_COL, 6, 8, BLACK);
}

void resetPong(int toPlayer) {
  pong.pastTransform = pong.transform;
  pong.transform.row = TO_PHYS_COORD(HEIGHT / 2 - 1);
  pong.transform.col = TO_PHYS_COORD(toPlayer ? PONG_MARGIN : WIDTH - PONG_MARGIN - PONG_SIZE);
  pong.transform.width = TO_PHYS_COORD(PONG_SIZE);
  pong.transform.height = TO_PHYS_COORD(PONG_SIZE);

  pong.velocity.x = (toPlayer ? -1 : 1) * PONG_VEL_INIT;
  pong.velocity.y = 0;

  pongCharge = NULL;
}

void updatePong(void) {
  pong.pastTransform = pong.transform;
  pong.transform.row += pong.velocity.y;
  pong.transform.col += pong.velocity.x * (pongCharge ? 5 : 2) / 2;

  if (rectCollide(pong.transform, playerPaddle.transform) && pong.velocity.x < 0) {
    pong.transform.col = playerPaddle.transform.col + playerPaddle.transform.width;
    pong.velocity.x *= -1;
    spinPong(&playerPaddle);
    if (!player.charged && !pongCharge) {
      player.energy += HIT_ENERGY;
      if (abs(pong.velocity.y) >= BONUS_HIT_VEL) player.energy += BONUS_HIT_ENERGY;
    }
    if (player.charged && !pongCharge) {
      player.charged = 0;
      pongCharge = &player;
      playSoundEffect(&chargePong);
    } else if (pongCharge == &com) {
      pongCharge = &player;
      playSoundEffect(&chargeSwitch);
    } else {
      playSoundEffect(&hit);
    }
  }

  if (rectCollide(pong.transform, comPaddle.transform) && pong.velocity.x > 0) {
    pong.transform.col = comPaddle.transform.col - pong.transform.width;
    pong.velocity.x *= -1;
    spinPong(&comPaddle);
    if (!com.charged && !pongCharge) {
      com.energy += HIT_ENERGY;
      if (abs(pong.velocity.y) >= BONUS_HIT_VEL) com.energy += BONUS_HIT_ENERGY;
    }
    if (com.charged && !pongCharge) {
      com.charged = 0;
      pongCharge = &com;
      playSoundEffect(&chargePong);
    } else if (pongCharge == &player) {
      pongCharge = &com;
      playSoundEffect(&chargeSwitch);
    } else {
      playSoundEffect(&hit);
    }
  }

  if (pong.transform.row < 0 && pong.velocity.y < 0) {
    pong.velocity.y *= -1;
    pong.transform.row = 0;
  }
  if (pong.transform.row > TO_PHYS_COORD(HEIGHT) - pong.transform.height && pong.velocity.y > 0) {
    pong.velocity.y *= -1;
    pong.transform.row = TO_PHYS_COORD(HEIGHT) - pong.transform.height;
  }
  if (pong.transform.col < 0 && pong.velocity.x < 0) {
    pong.velocity.x *= -1;
    pong.velocity.y /= 2;
    pong.transform.col = 0;
    if (pongCharge == &player) {
      pongCharge = NULL;
      playSoundEffect(&discharge);
    } else {
      playSoundEffect(&wall);
    }
    player.energy -= MISS_ENERGY_LOSS;
    if (player.energy < 0) player.energy = 0;
  }
  if (pong.transform.col > TO_PHYS_COORD(WIDTH) - pong.transform.width && pong.velocity.x > 0) {
    pong.velocity.x *= -1;
    pong.velocity.y /= 2;
    pong.transform.col = TO_PHYS_COORD(WIDTH) - pong.transform.width;
    if (pongCharge == &com) {
      pongCharge = NULL;
      playSoundEffect(&discharge);
    } else {
      playSoundEffect(&wall);
    }
    com.energy -= MISS_ENERGY_LOSS;
    if (com.energy < 0) com.energy = 0;
  }

  if (pongCharge) {
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        if (!symbols[i][j] && rectCollide(pong.transform, symbolColliders[i][j])) {
          symbols[i][j] = pongCharge;
          playSoundEffect(&mark);
        }
      }
    }
  }
}

void spinPong(struct body *paddle) {
  int initYVel = pong.velocity.y;
  int paddleVel = paddle->velocity.y;
  int rowDiff = (pong.transform.row + pong.transform.height / 2) - (paddle->transform.row + paddle->transform.height / 2);
  if (paddleVel != 0 && (initYVel == 0 || (abs(initYVel) < PADDLE_BASE_SPIN && initYVel/abs(initYVel) == paddleVel/abs(paddleVel)))) {
    pong.velocity.y = PADDLE_BASE_SPIN * paddleVel/abs(paddleVel);
  } else if (paddleVel < 0) {
    pong.velocity.y = initYVel * (initYVel > 0 ? 1 : 3) / 2;
  } else if (paddleVel > 0) {
    pong.velocity.y = initYVel * (initYVel < 0 ? 1 : 3) / 2;
  }
  pong.velocity.y += rowDiff / PADDLE_LOCATION_SPIN;
}

void resetPlayer(struct player *player, unsigned short color, const unsigned short *symbol) {
  player->energy = 0;
  player->score = 0;
  player->color = color;
  player->symbol = symbol;
  player->charged = 0;
}

void resetSymbols(void) {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      struct rect *collider = &symbolColliders[i][j];
      collider->row = TO_PHYS_COORD((HEIGHT - BLANK_SYMBOL_SIZE) / 2 + (i-1) * BOARD_CELL_SIZE);
      collider->col = TO_PHYS_COORD((WIDTH - BLANK_SYMBOL_SIZE) / 2 + (j-1) * BOARD_CELL_SIZE);
      collider->width = TO_PHYS_COORD(BLANK_SYMBOL_SIZE);
      collider->height = TO_PHYS_COORD(BLANK_SYMBOL_SIZE);
      symbols[i][j] = NULL;
    }
  }

  // For testing win states

//  symbols[0][0] = &com;
//  symbols[0][1] = &com;
//  symbols[0][2] = &com;
//  symbols[1][0] = &player;
//  symbols[1][1] = &player;
//  symbols[1][2] = NULL;
//  symbols[2][0] = &com;
//  symbols[2][1] = &com;
//  symbols[2][2] = &com;
}

struct player *checkSameSymbol(struct player *symbol1, struct player *symbol2, struct player *symbol3) {
  if (symbol1 == symbol2 && symbol1 == symbol3) return symbol1;
  return NULL;
}

struct player *checkForWin(void) {
  struct player *potentialWinner;
  for (int i = 0; i < 3; i++) {
    potentialWinner = checkSameSymbol(symbols[i][0], symbols[i][1], symbols[i][2]);
    if (potentialWinner) {
      return potentialWinner;
    }
  }
  for (int i = 0; i < 3; i++) {
    potentialWinner = checkSameSymbol(symbols[0][i], symbols[1][i], symbols[2][i]);
    if (potentialWinner) {
      return potentialWinner;
    }
  }
  potentialWinner = checkSameSymbol(symbols[0][0], symbols[1][1], symbols[2][2]);
  if (potentialWinner) {
    return potentialWinner;
  }
  potentialWinner = checkSameSymbol(symbols[0][2], symbols[1][1], symbols[2][0]);
  if (potentialWinner) {
    return potentialWinner;
  }
  return NULL;
}

void setWinningSymbols(void) {
  for (int i = 0; i < 3; i++) {
    if (checkSameSymbol(symbols[i][0], symbols[i][1], symbols[i][2])) {
      for (int j = 0; j < 3; j++) {
        winningSymbols[j].x = i;
        winningSymbols[j].y = j;
      }
      return;
    }
  }
  for (int i = 0; i < 3; i++) {
    if (checkSameSymbol(symbols[0][i], symbols[1][i], symbols[2][i])) {
      for (int j = 0; j < 3; j++) {
        winningSymbols[j].x = j;
        winningSymbols[j].y = i;
      }
      return;
    }
  }
  if (checkSameSymbol(symbols[0][0], symbols[1][1], symbols[2][2])) {
    for (int j = 0; j < 3; j++) {
      winningSymbols[j].x = j;
      winningSymbols[j].y = j;
    }
    return;
  }
  if (checkSameSymbol(symbols[0][2], symbols[1][1], symbols[2][0])) {
    for (int j = 0; j < 3; j++) {
      winningSymbols[j].x = j;
      winningSymbols[j].y = 2 - j;
    }
    return;
  }
}

int isInWinningSymbols(int i, int j) {
  for (int k = 0; k < 3; k++) {
    if (winningSymbols[k].x == i && winningSymbols[k].y == j) return 1;
  }
  return 0;
}

int totalMarks(struct player *player) {
  int total = 0;
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (symbols[i][j] == player) total++;
    }
  }
  return total;
}

int checkBoardFull(void) {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (!symbols[i][j]) return 0;
    }
  }
  return 1;
}

int rectCollide(struct rect r1, struct rect r2) {
  int r1x1 = r1.col;
  int r1x2 = r1.col + r1.width;
  int r2x1 = r2.col;
  int r2x2 = r2.col + r2.width;
  int r1y1 = r1.row;
  int r1y2 = r1.row + r1.height;
  int r2y1 = r2.row;
  int r2y2 = r2.row + r2.height;

  return (r1x1 < r2x2 && r1x2 > r2x1 && r1y1 < r2y2 && r1y2 > r2y1);
}