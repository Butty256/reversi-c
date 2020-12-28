#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "board.h"
#include "evaluator.h"
#include "com.h"

#define EVALUATOR_FILE "eval.dat"
#define BUF_SIZE 80

void move_random(Board *board, int in_color);
void learn(Board *board, Evaluator *evaluator, Com *com);

void move_random(Board *board, int in_color)
{
    while(!board_Flip(board, in_color, BOARD_POS(rand() % BOARD_SIZE + 1, rand() % BOARD_SIZE + 1)));
}

void learn(Board *board, Evaluator *evaluator, Com *com)
{
    char buf[BUF_SIZE];
    int his_color[BOARD_SIZE * BOARD_SIZE];
    int i, j, move, num, turn;
    int color;
    int result;

    srand((unsigned int)time(NULL));

    printf("num = ");
    fgets(buf, sizeof(buf), stdin);
    num = atoi(buf);

    com_SetLevel(com, 8, 14);
    for(i = 0; i < num; i++)
    {
        printf("%8d / %8d\n", i + 1, num);
        board_Clear(board);
        color = BLACK;
        turn = 0;
        for(j = 0; j < 8; j++)
        {
            if(board_CanPlay(board, color))
            {
                move_random(board, color);
                his_color[turn] = color;
                turn++;
            }
            color *= -1;
        }
        while(1)
        {
            if(board_CanPlay(board, color) == 0)
            {
                color *= -1;
                if(board_CanPlay(board, color) == 0) break;
            }
            if(board_CountDisk(board, EMPTY) > 10 && !(rand() % 500))
            {
                move_random(board, color);
            }
            else
            {
                move = com_NextMove(com, board, color);
                board_Flip(board, color, move);
            }
            his_color[turn] = color;
            turn++;
            color *= -1;
        }
        result = (board_CountDisk(board, BLACK) - board_CountDisk(board, WHITE)) * DISK_VALUE;
        for(j = board_CountDisk(board, EMPTY); j < 8; j++)
        {
            turn--;
            board_Unflip(board);
        }
        for(j = board_CountDisk(board, EMPTY); j < BOARD_SIZE * BOARD_SIZE - 12; j++)
        {
            turn--;
            board_Unflip(board);
            if(his_color[turn] == BLACK)
            {
                evaluator_Update(evaluator, board, result);
            }
            else
            {
                board_Reverse(board);
                evaluator_Update(evaluator, board, -result);
                board_Reverse(board);
            }
        }
        if((i + 1) % 10 == 0)
        {
            evaluator_Update(evaluator, board, result);
        }
        if ((i + 1) % 100 == 0) {
            puts("saving");
            evaluator_Save(evaluator, EVALUATOR_FILE);
        }
    }
    evaluator_Update(evaluator, board, result);
    evaluator_Save(evaluator, EVALUATOR_FILE);
    puts("end");
}

int main(void)
{
    Evaluator *evaluator;
    Board *board;
    Com *com;
    evaluator = evaluator_New();
    board = board_New();
    board_Clear(board);
    com = com_New(evaluator);

    learn(board, evaluator, com);

    return 0;
}