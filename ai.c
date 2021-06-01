//
// Created by mitchell on 4/8/21.
//

#include "ai.h"
#include "game.h"
#include "gba.h"
#include <stdlib.h>

difficulty difficultyOptions[5] = {
        {"Trivial", 0, 100, 5},
        {"Easy", 30, 15, 5},
        {"Normal", 30, 10, 4},
        {"Hard", 20, 8, 3},
        {"Expert", 0, 0, 1}
};

static int targetFound = 0;
static int reactionAccumulated = 0;
static int target = 0;
static int entropicStartTime = 0;
static difficulty *currDifficulty;

void resetAI(difficulty *diff) {
  entropicStartTime = 0;
  currDifficulty = diff;
}

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
    if (reactionAccumulated >= currDifficulty->reaction) {
      findTarget(paddle, pong, pongCharged);
      targetFound = 1;
    }
  }

  if (entropicStartTime < AI_ENTROPY_FRAMES) {
    entropicStartTime++;
    return 1;
  }

  if (targetFound) {
    int rowDiff = paddle.transform.row + paddle.transform.height / 2 - target;
    if (abs(rowDiff) < TO_PHYS_COORD(currDifficulty->targetMargin)) return 0;
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
  targetRow += randint(-TO_PHYS_COORD(currDifficulty->error), TO_PHYS_COORD(currDifficulty->error));
  target = targetRow;
}
