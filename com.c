#include <stdlib.h>
#include "com.h"
#include "board.h"
#include "evaluator.h"

void com_Delete(Com *com)
{
    if(com->Board)
    {
        board_Delete(com->Board);
    }
    free(com);
}

int com_Initialize(Com *com, Evaluator *evaluator)
{
    memset(com, 0, sizeof(Com));
    com->Board = board_New();
    if(!com->Board) return 0;
    com->Evaluator = evaluator;
    if(!com->Evaluator) return 0;
    com->MidDepth = 1;
    com->EndDepth = 1;
    com->Node = 0;

    return 1;
}

Com *com_New(Evaluator *evaluator)
{
    Com *com;
    com = malloc(sizeof(Com));
    if(com)
    {
        if(!com_Initialize(com, evaluator))
        {
            com_Delete(com);
            com = NULL;
        }
    }
    return com;
}

void com_SetLevel(Com *com, int in_mid, int in_exact)
{
    com->MidDepth = in_mid;
    com->EndDepth = in_exact;
}

int com_CountNodes(const Com *com)
{
    return com->Node;
}

int com_NextMove(Com *com, const Board *in_board, int in_color)
{
    int result;
    int left;
    int color;

    *com->Board = *in_board;
    board_Copy(in_board, com->Board);
    com->Node = 0;
    left = board_CountDisk(com->Board, EMPTY);
    if(left <= com->EndDepth)
    {
        com_EndSearch(com, left, -BOARD_SIZE * BOARD_SIZE, BOARD_SIZE * BOARD_SIZE, in_color, -in_color, 0, &result);
    }
    else
    {
        if((in_color == WHITE && com->MidDepth % 2 == 0) || (in_color == BLACK && com->MidDepth % 2 == 1))
        {
            board_Reverse(com->Board);
            color = -in_color;
        }
        else
        {
            color = in_color;
        }
        com_MidSearch(com, com->MidDepth, -MAX_VALUE, MAX_VALUE, color, -color, 0, &result);
    }

    return result;
}

int com_MidSearch(Com *com, int in_depth, int in_alpha, int in_beta, int in_color, int in_opponent, int in_pass, int *out_move)
{
    int value, max = in_alpha;
    int can_move = 0;
    int move;

    if(in_depth == 0)
    {
        com->Node++;
        return evaluator_Value(com->Evaluator, com->Board);
    }
    *out_move = NOMOVE;
    for(int x = 1; x <= BOARD_SIZE; x++)
    {
        for(int y = 1; y <= BOARD_SIZE; y++)
        {
            if(board_Flip(com->Board, in_color, BOARD_POS(x, y)))
            {
                if(!can_move)
                {
                    *out_move = BOARD_POS(x, y);
                    can_move = 1;
                }
                value = -com_MidSearch(com, in_depth - 1, -in_beta, -max, in_opponent, in_color, 0, &move);
                board_Unflip(com->Board);
                if(value > max)
                {
                    max = value;
                    *out_move = BOARD_POS(x, y);
                    if(max >= in_beta) return in_beta;
                }
            }
        }
    }
    if(!can_move)
    {
        if(in_pass)
        {
            *out_move = NOMOVE;
            com->Node++;
            max = DISK_VALUE * (board_CountDisk(com->Board, in_color) - board_CountDisk(com->Board, in_opponent));
        }
        else
        {
            *out_move = PASS;
            max = -com_MidSearch(com, in_depth - 1, -in_beta, -max, in_opponent, in_color, 1, &move);
        }
    }
    return max;
}

int com_EndSearch(Com *com, int in_depth, int in_alpha, int in_beta, int in_color, int in_opponent, int in_pass, int *out_move)
{
    int value, max = in_alpha;
    int can_move = 0;
    int move;

    if(in_depth == 0)
    {
        com->Node++;
        return board_CountDisk(com->Board, in_color) - board_CountDisk(com->Board, -in_color);
    }
    *out_move = NOMOVE;
    for(int x = 1; x <= BOARD_SIZE; x++)
    {
        for(int y = 1; y <= BOARD_SIZE; y++)
        {
            if(board_Flip(com->Board, in_color, BOARD_POS(x, y)))
            {
                if(!can_move)
                {
                    *out_move = BOARD_POS(x, y);
                    can_move = 1;
                }
                value = -com_EndSearch(com, in_depth - 1, -in_beta, -max, in_opponent, in_color, 0, &move);
                board_Unflip(com->Board);
                if(value > max)
                {
                    max = value;
                    *out_move = BOARD_POS(x, y);
                    if(max >= in_beta) return in_beta;
                }
            }
        }
    }
    if(!can_move)
    {
        if(in_pass)
        {
            *out_move = NOMOVE;
            com->Node++;
            max = board_CountDisk(com->Board, in_color) - board_CountDisk(com->Board, in_opponent);
        }
        else
        {
            *out_move = PASS;
            max = -com_EndSearch(com, in_depth - 1, -in_beta, -max, in_opponent, in_color, 1, &move);
        }
    }

    return max;
}
