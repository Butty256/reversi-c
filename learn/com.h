#ifndef COM_H
#define COM_H

#include <stdlib.h>
#include "board.h"
#include "evaluator.h"

#define MAX_VALUE (DISK_VALUE * 200)

typedef struct _Com Com;
struct _Com
{
    Board *Board;
    Evaluator *Evaluator;
    int MidDepth;
    int EndDepth;
    int Node;
};

void com_Delete(Com *com);
int com_Initialize(Com *com, Evaluator *evaluator);
Com *com_New(Evaluator *evaluator);
void com_SetLevel(Com *com, int in_mid, int in_exact);
int com_CountNodes(const Com *com);
int com_NextMove(Com *com, const Board *in_board, int in_color);
int com_MidSearch(Com *com, int in_depth, int in_alpha, int in_beta, int in_color, int in_opponent, int in_pass, int *out_move);
int com_EndSearch(Com *com, int in_depth, int in_alpha, int in_beta, int in_color, int in_opponent, int in_pass, int *out_move);

#endif
