#ifndef EVALUATOR_H
#define EVALUATOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "board.h"

#define DISK_VALUE 1000
#define UPDATE_RATIO 0.002
#define MAX_PATTERN_VALUE (DISK_VALUE * 25)

#define POW_3_0 1
#define POW_3_1 3
#define POW_3_2 9
#define POW_3_3 27
#define POW_3_4 81
#define POW_3_5 243
#define POW_3_6 729
#define POW_3_7 2187
#define POW_3_8 6561

#define BOARD_INDEX_4(b, s1, s2, s3, s4) \
	(((evaluator_BoardIndex((b), (s1)) * 3 + evaluator_BoardIndex((b), (s2))) * 3 + evaluator_BoardIndex((b), (s3))) * 3 + \
	evaluator_BoardIndex((b), (s4)))
#define BOARD_INDEX_5(b, s1, s2, s3, s4, s5) \
	((((evaluator_BoardIndex((b), (s1)) * 3 + evaluator_BoardIndex((b), (s2))) * 3 + evaluator_BoardIndex((b), (s3))) * 3 + \
	evaluator_BoardIndex((b), (s4))) * 3 + evaluator_BoardIndex((b), (s5)))
#define BOARD_INDEX_6(b, s1, s2, s3, s4, s5, s6) \
	(((((evaluator_BoardIndex((b), (s1)) * 3 + evaluator_BoardIndex((b), (s2))) * 3 + evaluator_BoardIndex((b), (s3))) * 3 + \
	evaluator_BoardIndex((b), (s4))) * 3 + evaluator_BoardIndex((b), (s5))) * 3 + evaluator_BoardIndex((b), (s6)))
#define BOARD_INDEX_7(b, s1, s2, s3, s4, s5, s6, s7) \
	((((((evaluator_BoardIndex((b), (s1)) * 3 + evaluator_BoardIndex((b), (s2))) * 3 + evaluator_BoardIndex((b), (s3))) * 3 + \
	evaluator_BoardIndex((b), (s4))) * 3 + evaluator_BoardIndex((b), (s5))) * 3 + evaluator_BoardIndex((b), (s6))) * 3 + \
	evaluator_BoardIndex((b), (s7)))
#define BOARD_INDEX_8(b, s1, s2, s3, s4, s5, s6, s7, s8) \
	(((((((evaluator_BoardIndex((b), (s1)) * 3 + evaluator_BoardIndex((b), (s2))) * 3 + evaluator_BoardIndex((b), (s3))) * 3 + \
	evaluator_BoardIndex((b), (s4))) * 3 + evaluator_BoardIndex((b), (s5))) * 3 + evaluator_BoardIndex((b), (s6))) * 3 + \
	evaluator_BoardIndex((b), (s7))) * 3 + evaluator_BoardIndex((b), (s8)))

enum
{
	PATTERN_ID_LINE4,
	PATTERN_ID_LINE3,
	PATTERN_ID_LINE2,
	PATTERN_ID_DIAG8,
	PATTERN_ID_DIAG7,
	PATTERN_ID_DIAG6,
	PATTERN_ID_DIAG5,
	PATTERN_ID_DIAG4,
	PATTERN_ID_EDGE8,
	PATTERN_ID_CORNER8,
	PATTERN_ID_PARITY,
	PATTERN_ID_NUM
};

typedef struct _Evaluator Evaluator;
struct _Evaluator
{
	int *Value[PATTERN_ID_NUM];
	int MirrorLine[POW_3_8];
	int MirrorCorner[POW_3_8];
};

Evaluator *evaluator_New(void);
int evaluator_Initialize(Evaluator *evaluator);
void evaluator_Delete(Evaluator *evaluator);
int evaluator_Load(Evaluator *evaluator, const char *file_name);
int evaluator_Save(const Evaluator *evaluator, const char *file_name);
int evaluator_BoardIndex(const Board *evaluator, int in_pos);
int evaluator_Value(const Evaluator *evaluator, const Board *in_board);
void evaluator_UpdatePattern(Evaluator *evaluator, int in_pattern, int in_id, int in_mirror, int in_diff);
void evaluator_Update(Evaluator *evaluator, const Board *in_board, int in_value);

#endif
