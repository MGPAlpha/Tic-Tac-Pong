//
// Created by mitchell on 4/8/21.
//

#include "ai.h"
#include "game.h"
#include "gba.h"
#include <stdlib.h>

int targetFound = 0;
int reactionAccumulated = 0;
int target = 0;

int controlAI(struct body paddle, struct body pong, int pongCharged) {
//  int rowDiff = paddle.transform.row + paddle.transform.height / 2 - (pong.transform.row + pong.transform.height / 2);
//  if (rowDiff > 0) return -1;
//  else if (rowDiff < 0) return 1;
//  else return 0;

  if (pong.velocity.x < 0 || pong.transform.col > paddle.transform.col + paddle.transform.width) {
    targetFound = 0;
    reactionAccumulated = 0;
    return 0;
  }

  if (!targetFound) {
    reactionAccumulated++;
    if (reactionAccumulated >= AI_REACTION) {
      findTarget(paddle, pong, pongCharged);
      targetFound = 1;
    }
  }

  if (targetFound) {
    int rowDiff = paddle.transform.row + paddle.transform.height / 2 - target;
    if (abs(rowDiff) < TO_PHYS_COORD(AI_TARGET_MARGIN)) return 0;
    else if (rowDiff > 0) {
      return -1;
    }
    else if (rowDiff < 0) {
      return 1;
    }
    else return 0;
  }

  return 0;
}

void findTarget(struct body paddle, struct body pong, int pongCharged) {
  int targetRow = pong.transform.row + (paddle.transform.col - pong.transform.col) * pong.velocity.y / (pong.velocity.x * (pongCharged ? 5 : 2) / 2);
  int maxRow = TO_PHYS_COORD(HEIGHT) - pong.transform.height;
  while (targetRow < 0 || targetRow > maxRow) {
    if (targetRow < 0) {
      targetRow = -targetRow;
    } else {
      targetRow = 2 * maxRow - targetRow;
    }
  }
  targetRow += randint(-TO_PHYS_COORD(AI_ERROR), TO_PHYS_COORD(AI_ERROR));
  target = targetRow;
}
