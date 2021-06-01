# Tic-Tac-Pong
### A Game By Mitchell Philipp

Tic-Tac-Pong is a Player vs. Computer game for the GameBoy Advance. It combines the classic video game Pong with the pen-and-paper game Tic-Tac-Toe.

**Objective:** Place 3 cyan X's in a row on the Tic-Tac-Toe board before the AI get 3 magenta O's in a row. 

## How to Play
* The player and the AI use their paddles to hit the pong back and forth.
* Each you hit the pong with your paddle, it adds energy to your battery battery.
* If you miss the pong and it hits the wall, your battery will lose a little power.
* When your battery is full, you can press `A` to charge your paddle.
* When you hit the pong with a charged paddle, the pong will speed up and become charged.
* When the charged pong passes thru a spot on the Tic-Tac-Toe board, it will place a mark there for the last player to hit it.
* If the charged pong hits a wall twice in a row, it loses its charge and slows back down.
* Get three of your marks in a row to win!

## Winning the Game
If either player manages to get three of their marks in a row on the board, they win immediately. If all the spaces on the board are captured, and neither player has three marks in a row, the player with the most marks on the board wins. Each player's total is displayed in the lower corner of the screen.

## Controls
| BUTTON          | ACTION                                                     |
| --------------- | ---------------------------------------------------------- |
| `START`         | Begin the game                                             |
| `UP` and `DOWN` | Move the player's paddle up and down the screen            |
| `A`             | Use the player's accumulated energy to charge their paddle |
|  Hold `SELECT`  | Return to the title screen                                 |

## Difficulty Options
| Difficulty | Description |
| ---------- | ----------- |
| Trivial    | Not even a challenge. The AI will almost always miss the pong.                                    |
| Easy       | An easier experience. The AI is more likely to miss the pong, and takes longer to make decisions. |
| Normal     | A medium challenge. **Recommended for new players.**                                              |
| Hard       | Above average difficulty. Provides a little extra challenge, with a more skilled AI.              |
| Expert     | Near perfect AI. It will rarely miss.                                                             |

For information on the game's development environment, see the accompanying file [dev-readme.md](dev-readme.md)

Recommended configuration for the Mednafen emulator is included in [Tic-Tac-Pong.gba.cfg](Tic-Tac-Pong.gba.cfg)

###### Contact the developer
> Mitchell Philipp (he/they): [mgpalpha1@gmail.com](mailto:mgpalpha1@gmail.com)
>
> Github: [MGPAlpha](https://github.com/MGPAlpha)