#ifndef BOARD_H
#define BOARD_H

#define EMPTY 0
#define BLACK 1
#define WHITE (-BLACK) /* -1 */
#define PASS -2
#define NOMOVE -3
#define BOARD_SIZE 8
#define NUM_DISK ((BOARD_SIZE + 2) * (BOARD_SIZE + 2))
#define NUM_STACK (((BOARD_SIZE - 2) * 3 + 3) * BOARD_SIZE * BOARD_SIZE)
#define BOARD_POS(x, y) ((y) * (BOARD_SIZE + 2) + (x))
#define BOARD_STACK_POP(board) (*(--board->Sp))
#define BOARD_STACK_PUSH(board, n) (*(board->Sp++) = n)

typedef struct _Board Board;
struct _Board
{
    int Disk[NUM_DISK];
    int Stack[NUM_STACK];
    int *Sp;
};

void board_Clear(Board *board);
Board *board_New(void);
void board_Delete(Board *board);
int board_FlipLine(Board *board, int in_color, int in_pos, int in_dir);
int board_Flip(Board *board, int in_color, int in_pos);
int board_Unflip(Board *board);
int board_CountFlipLine(const Board *board, int in_color, int in_pos, int in_dir);
int board_CountFlip(const Board *board, int in_color, int in_pos);
int board_CanFlip(Board *board, int in_color, int in_pos);
int board_CanPlay(const Board *board, int in_color);
void board_Copy(const Board *board1, Board *board2);
void board_Reverse(Board *board);
int board_Disk(const Board *board, int in_pos);
int board_CountDisk(const Board *board, int in_color);

#endif
