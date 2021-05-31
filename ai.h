//
// Created by mitchell on 4/8/21.
//

#ifndef HW08_AI_H
#define HW08_AI_H

#include "game.h"

#define AI_REACTION 30
#define AI_ERROR 10
#define AI_TARGET_MARGIN 4
#define AI_ENTROPY_FRAMES 5

typedef struct {
  char name[10];
  short reaction, error, targetMargin;
} difficulty;

extern difficulty difficultyOptions[4];

void resetAI(difficulty *diff);
int controlAI(struct body paddle, struct body pong, int pongCharged);
void findTarget(struct body paddle, struct body pong, int pongCharged);

#endif //HW08_AI_H
