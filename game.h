//
// Created by mitchell on 4/5/21.
//

#ifndef HW08_GAME_H
#define HW08_GAME_H

#define BORDER_WIDTH 3

#define BOARD_COLOR GRAY
#define BOARD_SIZE 90
#define BOARD_CELL_SIZE 30
#define BOARD_START_COL ((WIDTH - BOARD_SIZE) / 2)
#define BOARD_START_ROW ((HEIGHT - BOARD_SIZE) / 2)
#define HORIZ_0_ROW ((HEIGHT - BOARD_CELL_SIZE) / 2 - 1)
#define HORIZ_1_ROW ((HEIGHT + BOARD_CELL_SIZE) / 2 - 1)
#define VERT_0_COL ((WIDTH - BOARD_CELL_SIZE) / 2 - 1)
#define VERT_1_COL ((WIDTH + BOARD_CELL_SIZE) / 2 - 1)

#define BATTERY_MARGIN 6

#define PADDLE_MARGIN 20
#define PADDLE_SIZE 20
#define PADDLE_WIDTH 2
#define LEFT_PADDLE_COL PADDLE_MARGIN
#define RIGHT_PADDLE_COL (WIDTH - PADDLE_WIDTH - PADDLE_MARGIN)

#define PONG_SIZE 2
#define PONG_MARGIN 60

#define PHYSICS_SCALE 6
#define TO_PHYS_COORD(coord) ((coord) << PHYSICS_SCALE)
#define TO_SCREEN_COORD(coord) ((coord) >> PHYSICS_SCALE)

#define PADDLE_ACCEL 10
#define PADDLE_DRAG 20

#define PADDLE_BASE_SPIN (PONG_VEL_INIT / 3)
#define PADDLE_LOCATION_SPIN 20

#define PONG_VEL_INIT 100

#define MAX_ENERGY 12
#define HIT_ENERGY 2
#define BONUS_HIT_ENERGY 1
#define BONUS_HIT_VEL (PONG_VEL_INIT * 3 / 2)
#define MISS_ENERGY_LOSS 1

#define BLINK_INTERVAL 20
#define BLINK_VALUE(val, res0, res1) ((val) % BLINK_INTERVAL > BLINK_INTERVAL / 2 ? (res0) : (res1))
#define WINNER_BLINK_INTERVAL 60

#define BLANK_SYMBOL_SIZE 6
#define SYMBOL_SIZE 18

#define SCORE_MARGIN 7
#define SCORE_ROW (HEIGHT - SCORE_MARGIN - 4)
#define LEFT_SCORE_COL (SCORE_MARGIN - 3)
#define RIGHT_SCORE_COL (WIDTH - SCORE_MARGIN - 4)

#define GAME_END_FRAMES 180

struct rect {
    int col, row, width, height;
};

struct vector2 {
    int x, y;
};

struct body {
    struct rect transform, pastTransform;
    struct vector2 velocity;
};

struct player {
    int energy, score, charged;
    unsigned short color;
    const unsigned short *symbol;
};

void gameInit(void);
void gameUpdate(int currentButtons, int previousButtons);
void drawBoard(void);
void drawBorders(void);
void drawBatteries(void);
void drawSymbols(void);
void resetPaddle(struct body *paddle, int col);
void controlPaddle(struct body *paddle, int dir);
void drawBody(struct body *body, unsigned short color);
void undrawBody(struct body *body);
void undrawScores(void);
void resetPong(int toPlayer);
void updatePong(void);
void spinPong(struct body *paddle);
void resetPlayer(struct player *player, unsigned short color, const unsigned short *symbol);
void resetSymbols(void);

struct player *checkSameSymbol(struct player *symbol1, struct player *symbol2, struct player *symbol3);
struct player *checkForWin(void);
void setWinningSymbols(void);
int isInWinningSymbols(int i, int j);
int totalMarks(struct player *player);
int checkBoardFull(void);

int rectCollide(struct rect r1, struct rect r2);

#endif //HW08_GAME_H
