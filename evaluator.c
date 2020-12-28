#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "board.h"
#include "evaluator.h"

const int PatternSize[] =
{
    POW_3_8,/* A4-H4 */
    POW_3_8,/* A3-H3 */
    POW_3_8,/* A2-H2 */
    POW_3_8,/* A1-H8 */
    POW_3_7,/* A2-G8 */
    POW_3_6,/* A3-F8 */
    POW_3_5,/* A4-E8 */
    POW_3_4,/* A5-D8 */
    POW_3_8,/* A1-G1 + B2 */
    POW_3_8,/* A1-C1 + A2-C2 + A3-B3 */
    2,/* Parity */
    0/* dummy */
};

Evaluator *evaluator_New(void)
{
    Evaluator *evaluator;

    evaluator = malloc(sizeof(Evaluator));
    if(evaluator)
    {
        if(!evaluator_Initialize(evaluator))
        {
            evaluator_Delete(evaluator);
            evaluator = NULL;
        }
    }
    return evaluator;
}

int evaluator_Initialize(Evaluator *evaluator)
{
    int i, j;
    int mirror_in, mirror_out, coeff;
    int mirror_corner_coeff[] = { POW_3_2, POW_3_5, POW_3_0, POW_3_3, POW_3_6, POW_3_1, POW_3_4, POW_3_7 };

    memset(evaluator, 0, sizeof(Evaluator));
    for(i = 0; i < PATTERN_ID_NUM; i++)
    {
        evaluator->Value[i] = calloc(PatternSize[i], sizeof(int));
        if(!evaluator->Value[i]) return 0;
    }
    for(i = 0; i < POW_3_8; i++)
    {
        mirror_in = i;
        mirror_out = 0;
        coeff = POW_3_7;
        for (j = 0; j < 8; j++)
        {
            mirror_out += mirror_in % 3 * coeff;
            mirror_in /= 3;
            coeff /= 3;
        }
        if (mirror_out < i) evaluator->MirrorLine[i] = mirror_out;
        else evaluator->MirrorLine[i] = i;
    }
    for(i = 0; i < POW_3_8; i++)
    {
        mirror_in = i;
        mirror_out = 0;
        for(j = 0; j < 8; j++)
        {
            mirror_out += mirror_in % 3 * mirror_corner_coeff[j];
            mirror_in /= 3;
        }
        if(mirror_out < i) evaluator->MirrorCorner[i] = mirror_out;
        else evaluator->MirrorCorner[i] = i;
    }

    return 1;
}

void evaluator_Delete(Evaluator *evaluator)
{
    for(int i = 0; i < PATTERN_ID_NUM; i++)
    {
        if(evaluator->Value[i]) free(evaluator->Value[i]);
    }
    free(evaluator);
}

int evaluator_Load(Evaluator *evaluator, const char *file_name)
{
    FILE *fp;
    if((fp = fopen(file_name, "rb")) == NULL) return 0;
    for(int i = 0; i < PATTERN_ID_NUM; i++)
    {
        if(fread(evaluator->Value[i], sizeof(int), PatternSize[i], fp) < (size_t)PatternSize[i])
        {
            fclose(fp);
            return 0;
        }
    }
    fclose(fp);
    return 1;
}

int evaluator_Save(const Evaluator *evaluator, const char *file_name)
{
    FILE *fp;
    if((fp = fopen(file_name, "wb")) == NULL) return 0;
    for(int i = 0; i < PATTERN_ID_NUM; i++)
    {
        if(fwrite(evaluator->Value[i], sizeof(int), PatternSize[i], fp) < (size_t)PatternSize[i])
        {
            fclose(fp);
            return 0;
        }
    }
    fclose(fp);
    return 1;
}

int evaluator_BoardIndex(const Board *board, int in_pos)
{
    int tmp;
    tmp = board->Disk[in_pos];
    if(tmp == BLACK) return 1;
    else if(tmp == WHITE) return 2;
    return 0;
}

int evaluator_Value(const Evaluator *evaluator, const Board *in_board)
{
    int result = 0;

    result += evaluator->Value[PATTERN_ID_LINE4][BOARD_INDEX_8(in_board, BOARD_POS(1, 4), BOARD_POS(2, 4), BOARD_POS(3, 4), BOARD_POS(4, 4), BOARD_POS(5, 4), BOARD_POS(6, 4), BOARD_POS(7, 4), BOARD_POS(8, 4))];
    result += evaluator->Value[PATTERN_ID_LINE4][BOARD_INDEX_8(in_board, BOARD_POS(1, 5), BOARD_POS(2, 5), BOARD_POS(3, 5), BOARD_POS(4, 5), BOARD_POS(5, 5), BOARD_POS(6, 5), BOARD_POS(7, 5), BOARD_POS(8, 5))];
    result += evaluator->Value[PATTERN_ID_LINE4][BOARD_INDEX_8(in_board, BOARD_POS(4, 1), BOARD_POS(4, 2), BOARD_POS(4, 3), BOARD_POS(4, 4), BOARD_POS(4, 5), BOARD_POS(4, 6), BOARD_POS(4, 7), BOARD_POS(4, 8))];
    result += evaluator->Value[PATTERN_ID_LINE4][BOARD_INDEX_8(in_board, BOARD_POS(5, 1), BOARD_POS(5, 2), BOARD_POS(5, 3), BOARD_POS(5, 4), BOARD_POS(5, 5), BOARD_POS(5, 6), BOARD_POS(5, 7), BOARD_POS(5, 8))];
    result += evaluator->Value[PATTERN_ID_LINE3][BOARD_INDEX_8(in_board, BOARD_POS(1, 3), BOARD_POS(2, 3), BOARD_POS(3, 3), BOARD_POS(4, 3), BOARD_POS(5, 3), BOARD_POS(6, 3), BOARD_POS(7, 3), BOARD_POS(8, 3))];
    result += evaluator->Value[PATTERN_ID_LINE3][BOARD_INDEX_8(in_board, BOARD_POS(1, 6), BOARD_POS(2, 6), BOARD_POS(3, 6), BOARD_POS(4, 6), BOARD_POS(5, 6), BOARD_POS(6, 6), BOARD_POS(7, 6), BOARD_POS(8, 6))];
    result += evaluator->Value[PATTERN_ID_LINE3][BOARD_INDEX_8(in_board, BOARD_POS(3, 1), BOARD_POS(3, 2), BOARD_POS(3, 3), BOARD_POS(3, 4), BOARD_POS(3, 5), BOARD_POS(3, 6), BOARD_POS(3, 7), BOARD_POS(3, 8))];
    result += evaluator->Value[PATTERN_ID_LINE3][BOARD_INDEX_8(in_board, BOARD_POS(6, 1), BOARD_POS(6, 2), BOARD_POS(6, 3), BOARD_POS(6, 4), BOARD_POS(6, 5), BOARD_POS(6, 6), BOARD_POS(6, 7), BOARD_POS(6, 8))];
    result += evaluator->Value[PATTERN_ID_LINE2][BOARD_INDEX_8(in_board, BOARD_POS(1, 2), BOARD_POS(2, 2), BOARD_POS(3, 2), BOARD_POS(4, 2), BOARD_POS(5, 2), BOARD_POS(6, 2), BOARD_POS(7, 2), BOARD_POS(8, 2))];
    result += evaluator->Value[PATTERN_ID_LINE2][BOARD_INDEX_8(in_board, BOARD_POS(1, 7), BOARD_POS(2, 7), BOARD_POS(3, 7), BOARD_POS(4, 7), BOARD_POS(5, 7), BOARD_POS(6, 7), BOARD_POS(7, 7), BOARD_POS(8, 7))];
    result += evaluator->Value[PATTERN_ID_LINE2][BOARD_INDEX_8(in_board, BOARD_POS(2, 1), BOARD_POS(2, 2), BOARD_POS(2, 3), BOARD_POS(2, 4), BOARD_POS(2, 5), BOARD_POS(2, 6), BOARD_POS(2, 7), BOARD_POS(2, 8))];
    result += evaluator->Value[PATTERN_ID_LINE2][BOARD_INDEX_8(in_board, BOARD_POS(7, 1), BOARD_POS(7, 2), BOARD_POS(7, 3), BOARD_POS(7, 4), BOARD_POS(7, 5), BOARD_POS(7, 6), BOARD_POS(7, 7), BOARD_POS(7, 8))];
    result += evaluator->Value[PATTERN_ID_DIAG8][BOARD_INDEX_8(in_board, BOARD_POS(1, 1), BOARD_POS(2, 2), BOARD_POS(3, 3), BOARD_POS(4, 4), BOARD_POS(5, 5), BOARD_POS(6, 6), BOARD_POS(7, 7), BOARD_POS(8, 8))];
    result += evaluator->Value[PATTERN_ID_DIAG8][BOARD_INDEX_8(in_board, BOARD_POS(1, 8), BOARD_POS(2, 7), BOARD_POS(3, 6), BOARD_POS(4, 5), BOARD_POS(5, 4), BOARD_POS(6, 3), BOARD_POS(7, 2), BOARD_POS(8, 1))];
    result += evaluator->Value[PATTERN_ID_DIAG7][BOARD_INDEX_7(in_board, BOARD_POS(1, 2), BOARD_POS(2, 3), BOARD_POS(3, 4), BOARD_POS(4, 5), BOARD_POS(5, 6), BOARD_POS(6, 7), BOARD_POS(7, 8))];
    result += evaluator->Value[PATTERN_ID_DIAG7][BOARD_INDEX_7(in_board, BOARD_POS(2, 1), BOARD_POS(3, 2), BOARD_POS(4, 3), BOARD_POS(5, 4), BOARD_POS(6, 5), BOARD_POS(7, 6), BOARD_POS(8, 7))];
    result += evaluator->Value[PATTERN_ID_DIAG7][BOARD_INDEX_7(in_board, BOARD_POS(1, 7), BOARD_POS(2, 6), BOARD_POS(3, 5), BOARD_POS(4, 4), BOARD_POS(5, 3), BOARD_POS(6, 2), BOARD_POS(7, 1))];
    result += evaluator->Value[PATTERN_ID_DIAG7][BOARD_INDEX_7(in_board, BOARD_POS(2, 8), BOARD_POS(3, 7), BOARD_POS(4, 6), BOARD_POS(5, 5), BOARD_POS(6, 4), BOARD_POS(7, 3), BOARD_POS(8, 2))];
    result += evaluator->Value[PATTERN_ID_DIAG6][BOARD_INDEX_6(in_board, BOARD_POS(1, 3), BOARD_POS(2, 4), BOARD_POS(3, 5), BOARD_POS(4, 6), BOARD_POS(5, 7), BOARD_POS(6, 8))];
    result += evaluator->Value[PATTERN_ID_DIAG6][BOARD_INDEX_6(in_board, BOARD_POS(3, 1), BOARD_POS(4, 2), BOARD_POS(5, 3), BOARD_POS(6, 4), BOARD_POS(7, 5), BOARD_POS(8, 6))];
    result += evaluator->Value[PATTERN_ID_DIAG6][BOARD_INDEX_6(in_board, BOARD_POS(1, 6), BOARD_POS(2, 5), BOARD_POS(3, 4), BOARD_POS(4, 3), BOARD_POS(5, 2), BOARD_POS(6, 1))];
    result += evaluator->Value[PATTERN_ID_DIAG6][BOARD_INDEX_6(in_board, BOARD_POS(3, 8), BOARD_POS(4, 7), BOARD_POS(5, 6), BOARD_POS(6, 5), BOARD_POS(7, 4), BOARD_POS(8, 3))];
    result += evaluator->Value[PATTERN_ID_DIAG5][BOARD_INDEX_5(in_board, BOARD_POS(1, 4), BOARD_POS(2, 5), BOARD_POS(3, 6), BOARD_POS(4, 7), BOARD_POS(5, 8))];
    result += evaluator->Value[PATTERN_ID_DIAG5][BOARD_INDEX_5(in_board, BOARD_POS(4, 1), BOARD_POS(5, 2), BOARD_POS(6, 3), BOARD_POS(7, 4), BOARD_POS(8, 5))];
    result += evaluator->Value[PATTERN_ID_DIAG5][BOARD_INDEX_5(in_board, BOARD_POS(1, 5), BOARD_POS(2, 4), BOARD_POS(3, 3), BOARD_POS(4, 2), BOARD_POS(5, 1))];
    result += evaluator->Value[PATTERN_ID_DIAG5][BOARD_INDEX_5(in_board, BOARD_POS(4, 8), BOARD_POS(5, 7), BOARD_POS(6, 6), BOARD_POS(7, 5), BOARD_POS(8, 4))];
    result += evaluator->Value[PATTERN_ID_DIAG4][BOARD_INDEX_4(in_board, BOARD_POS(1, 5), BOARD_POS(2, 6), BOARD_POS(3, 7), BOARD_POS(4, 8))];
    result += evaluator->Value[PATTERN_ID_DIAG4][BOARD_INDEX_4(in_board, BOARD_POS(5, 1), BOARD_POS(6, 2), BOARD_POS(7, 3), BOARD_POS(8, 4))];
    result += evaluator->Value[PATTERN_ID_DIAG4][BOARD_INDEX_4(in_board, BOARD_POS(1, 4), BOARD_POS(2, 3), BOARD_POS(3, 2), BOARD_POS(4, 1))];
    result += evaluator->Value[PATTERN_ID_DIAG4][BOARD_INDEX_4(in_board, BOARD_POS(5, 8), BOARD_POS(6, 7), BOARD_POS(7, 6), BOARD_POS(8, 5))];
    result += evaluator->Value[PATTERN_ID_EDGE8][BOARD_INDEX_8(in_board, BOARD_POS(1, 1), BOARD_POS(2, 1), BOARD_POS(3, 1), BOARD_POS(4, 1), BOARD_POS(5, 1), BOARD_POS(6, 1), BOARD_POS(7, 1), BOARD_POS(2, 2))];
    result += evaluator->Value[PATTERN_ID_EDGE8][BOARD_INDEX_8(in_board, BOARD_POS(8, 1), BOARD_POS(7, 1), BOARD_POS(6, 1), BOARD_POS(5, 1), BOARD_POS(4, 1), BOARD_POS(3, 1), BOARD_POS(2, 1), BOARD_POS(7, 2))];
    result += evaluator->Value[PATTERN_ID_EDGE8][BOARD_INDEX_8(in_board, BOARD_POS(1, 8), BOARD_POS(2, 8), BOARD_POS(3, 8), BOARD_POS(4, 8), BOARD_POS(5, 8), BOARD_POS(6, 8), BOARD_POS(7, 8), BOARD_POS(2, 7))];
    result += evaluator->Value[PATTERN_ID_EDGE8][BOARD_INDEX_8(in_board, BOARD_POS(8, 8), BOARD_POS(7, 8), BOARD_POS(6, 8), BOARD_POS(5, 8), BOARD_POS(4, 8), BOARD_POS(3, 8), BOARD_POS(2, 8), BOARD_POS(7, 7))];
    result += evaluator->Value[PATTERN_ID_EDGE8][BOARD_INDEX_8(in_board, BOARD_POS(1, 1), BOARD_POS(1, 2), BOARD_POS(1, 3), BOARD_POS(1, 4), BOARD_POS(1, 5), BOARD_POS(1, 6), BOARD_POS(1, 7), BOARD_POS(2, 2))];
    result += evaluator->Value[PATTERN_ID_EDGE8][BOARD_INDEX_8(in_board, BOARD_POS(1, 8), BOARD_POS(1, 7), BOARD_POS(1, 6), BOARD_POS(1, 5), BOARD_POS(1, 4), BOARD_POS(1, 3), BOARD_POS(1, 2), BOARD_POS(2, 7))];
    result += evaluator->Value[PATTERN_ID_EDGE8][BOARD_INDEX_8(in_board, BOARD_POS(8, 1), BOARD_POS(8, 2), BOARD_POS(8, 3), BOARD_POS(8, 4), BOARD_POS(8, 5), BOARD_POS(8, 6), BOARD_POS(8, 7), BOARD_POS(7, 2))];
    result += evaluator->Value[PATTERN_ID_EDGE8][BOARD_INDEX_8(in_board, BOARD_POS(8, 8), BOARD_POS(8, 7), BOARD_POS(8, 6), BOARD_POS(8, 5), BOARD_POS(8, 4), BOARD_POS(8, 3), BOARD_POS(8, 2), BOARD_POS(7, 7))];
    result += evaluator->Value[PATTERN_ID_CORNER8][BOARD_INDEX_8(in_board, BOARD_POS(1, 1), BOARD_POS(2, 1), BOARD_POS(3, 1), BOARD_POS(1, 2), BOARD_POS(2, 2), BOARD_POS(3, 2), BOARD_POS(1, 3), BOARD_POS(2, 3))];
    result += evaluator->Value[PATTERN_ID_CORNER8][BOARD_INDEX_8(in_board, BOARD_POS(8, 1), BOARD_POS(7, 1), BOARD_POS(6, 1), BOARD_POS(8, 2), BOARD_POS(7, 2), BOARD_POS(6, 2), BOARD_POS(8, 3), BOARD_POS(7, 3))];
    result += evaluator->Value[PATTERN_ID_CORNER8][BOARD_INDEX_8(in_board, BOARD_POS(1, 8), BOARD_POS(2, 8), BOARD_POS(3, 8), BOARD_POS(1, 7), BOARD_POS(2, 7), BOARD_POS(3, 7), BOARD_POS(1, 6), BOARD_POS(2, 6))];
    result += evaluator->Value[PATTERN_ID_CORNER8][BOARD_INDEX_8(in_board, BOARD_POS(8, 8), BOARD_POS(7, 8), BOARD_POS(6, 8), BOARD_POS(8, 7), BOARD_POS(7, 7), BOARD_POS(6, 7), BOARD_POS(8, 6), BOARD_POS(7, 6))];
    result += evaluator->Value[PATTERN_ID_PARITY][board_CountDisk(in_board, EMPTY) & 1];

    return result;
}

void evaluator_UpdatePattern(Evaluator *evaluator, int in_pattern, int in_id, int in_mirror, int in_diff)
{
    if(MAX_PATTERN_VALUE - in_diff < evaluator->Value[in_pattern][in_id])
        evaluator->Value[in_pattern][in_id] = MAX_PATTERN_VALUE;
    else if(-MAX_PATTERN_VALUE - in_diff > evaluator->Value[in_pattern][in_id])
        evaluator->Value[in_pattern][in_id] = -MAX_PATTERN_VALUE;
    else
        evaluator->Value[in_pattern][in_id] += in_diff;
    if(in_mirror >= 0)
        evaluator->Value[in_pattern][in_mirror] = evaluator->Value[in_pattern][in_id];
}

void evaluator_Update(Evaluator *evaluator, const Board *in_board, int in_value)
{
    int index, diff;

    diff = (int)((in_value - evaluator_Value(evaluator, in_board)) * UPDATE_RATIO);
    index = BOARD_INDEX_8(in_board, BOARD_POS(1, 4), BOARD_POS(2, 4), BOARD_POS(3, 4), BOARD_POS(4, 4), BOARD_POS(5, 4), BOARD_POS(6, 4), BOARD_POS(7, 4), BOARD_POS(8, 4));
    evaluator_UpdatePattern(evaluator, PATTERN_ID_LINE4, evaluator->MirrorLine[index], index, diff);
    index = BOARD_INDEX_8(in_board, BOARD_POS(1, 5), BOARD_POS(2, 5), BOARD_POS(3, 5), BOARD_POS(4, 5), BOARD_POS(5, 5), BOARD_POS(6, 5), BOARD_POS(7, 5), BOARD_POS(8, 5));
    evaluator_UpdatePattern(evaluator, PATTERN_ID_LINE4, evaluator->MirrorLine[index], index, diff);
    index = BOARD_INDEX_8(in_board, BOARD_POS(4, 1), BOARD_POS(4, 2), BOARD_POS(4, 3), BOARD_POS(4, 4), BOARD_POS(4, 5), BOARD_POS(4, 6), BOARD_POS(4, 7), BOARD_POS(4, 8));
    evaluator_UpdatePattern(evaluator, PATTERN_ID_LINE4, evaluator->MirrorLine[index], index, diff);
    index = BOARD_INDEX_8(in_board, BOARD_POS(5, 1), BOARD_POS(5, 2), BOARD_POS(5, 3), BOARD_POS(5, 4), BOARD_POS(5, 5), BOARD_POS(5, 6), BOARD_POS(5, 7), BOARD_POS(5, 8));
    evaluator_UpdatePattern(evaluator, PATTERN_ID_LINE4, evaluator->MirrorLine[index], index, diff);
    index = BOARD_INDEX_8(in_board, BOARD_POS(1, 3), BOARD_POS(2, 3), BOARD_POS(3, 3), BOARD_POS(4, 3), BOARD_POS(5, 3), BOARD_POS(6, 3), BOARD_POS(7, 3), BOARD_POS(8, 3));
    evaluator_UpdatePattern(evaluator, PATTERN_ID_LINE3, evaluator->MirrorLine[index], index, diff);
    index = BOARD_INDEX_8(in_board, BOARD_POS(1, 6), BOARD_POS(2, 6), BOARD_POS(3, 6), BOARD_POS(4, 6), BOARD_POS(5, 6), BOARD_POS(6, 6), BOARD_POS(7, 6), BOARD_POS(8, 6));
    evaluator_UpdatePattern(evaluator, PATTERN_ID_LINE3, evaluator->MirrorLine[index], index, diff);
    index = BOARD_INDEX_8(in_board, BOARD_POS(3, 1), BOARD_POS(3, 2), BOARD_POS(3, 3), BOARD_POS(3, 4), BOARD_POS(3, 5), BOARD_POS(3, 6), BOARD_POS(3, 7), BOARD_POS(3, 8));
    evaluator_UpdatePattern(evaluator, PATTERN_ID_LINE3, evaluator->MirrorLine[index], index, diff);
    index = BOARD_INDEX_8(in_board, BOARD_POS(6, 1), BOARD_POS(6, 2), BOARD_POS(6, 3), BOARD_POS(6, 4), BOARD_POS(6, 5), BOARD_POS(6, 6), BOARD_POS(6, 7), BOARD_POS(6, 8));
    evaluator_UpdatePattern(evaluator, PATTERN_ID_LINE3, evaluator->MirrorLine[index], index, diff);
    index = BOARD_INDEX_8(in_board, BOARD_POS(1, 2), BOARD_POS(2, 2), BOARD_POS(3, 2), BOARD_POS(4, 2), BOARD_POS(5, 2), BOARD_POS(6, 2), BOARD_POS(7, 2), BOARD_POS(8, 2));
    evaluator_UpdatePattern(evaluator, PATTERN_ID_LINE2, evaluator->MirrorLine[index], index, diff);
    index = BOARD_INDEX_8(in_board, BOARD_POS(1, 7), BOARD_POS(2, 7), BOARD_POS(3, 7), BOARD_POS(4, 7), BOARD_POS(5, 7), BOARD_POS(6, 7), BOARD_POS(7, 7), BOARD_POS(8, 7));
    evaluator_UpdatePattern(evaluator, PATTERN_ID_LINE2, evaluator->MirrorLine[index], index, diff);
    index = BOARD_INDEX_8(in_board, BOARD_POS(2, 1), BOARD_POS(2, 2), BOARD_POS(2, 3), BOARD_POS(2, 4), BOARD_POS(2, 5), BOARD_POS(2, 6), BOARD_POS(2, 7), BOARD_POS(2, 8));
    evaluator_UpdatePattern(evaluator, PATTERN_ID_LINE2, evaluator->MirrorLine[index], index, diff);
    index = BOARD_INDEX_8(in_board, BOARD_POS(7, 1), BOARD_POS(7, 2), BOARD_POS(7, 3), BOARD_POS(7, 4), BOARD_POS(7, 5), BOARD_POS(7, 6), BOARD_POS(7, 7), BOARD_POS(7, 8));
    evaluator_UpdatePattern(evaluator, PATTERN_ID_LINE2, evaluator->MirrorLine[index], index, diff);
    index = BOARD_INDEX_8(in_board, BOARD_POS(1, 1), BOARD_POS(2, 2), BOARD_POS(3, 3), BOARD_POS(4, 4), BOARD_POS(5, 5), BOARD_POS(6, 6), BOARD_POS(7, 7), BOARD_POS(8, 8));
    evaluator_UpdatePattern(evaluator, PATTERN_ID_DIAG8, evaluator->MirrorLine[index], index, diff);
    index = BOARD_INDEX_8(in_board, BOARD_POS(1, 8), BOARD_POS(2, 7), BOARD_POS(3, 6), BOARD_POS(4, 5), BOARD_POS(5, 4), BOARD_POS(6, 3), BOARD_POS(7, 2), BOARD_POS(8, 1));
    evaluator_UpdatePattern(evaluator, PATTERN_ID_DIAG8, evaluator->MirrorLine[index], index, diff);
    index = BOARD_INDEX_7(in_board, BOARD_POS(1, 2), BOARD_POS(2, 3), BOARD_POS(3, 4), BOARD_POS(4, 5), BOARD_POS(5, 6), BOARD_POS(6, 7), BOARD_POS(7, 8));
    evaluator_UpdatePattern(evaluator, PATTERN_ID_DIAG7, evaluator->MirrorLine[index * POW_3_1], index, diff);
    index = BOARD_INDEX_7(in_board, BOARD_POS(2, 1), BOARD_POS(3, 2), BOARD_POS(4, 3), BOARD_POS(5, 4), BOARD_POS(6, 5), BOARD_POS(7, 6), BOARD_POS(8, 7));
    evaluator_UpdatePattern(evaluator, PATTERN_ID_DIAG7, evaluator->MirrorLine[index * POW_3_1], index, diff);
    index = BOARD_INDEX_7(in_board, BOARD_POS(1, 7), BOARD_POS(2, 6), BOARD_POS(3, 5), BOARD_POS(4, 4), BOARD_POS(5, 3), BOARD_POS(6, 2), BOARD_POS(7, 1));
    evaluator_UpdatePattern(evaluator, PATTERN_ID_DIAG7, evaluator->MirrorLine[index * POW_3_1], index, diff);
    index = BOARD_INDEX_7(in_board, BOARD_POS(2, 8), BOARD_POS(3, 7), BOARD_POS(4, 6), BOARD_POS(5, 5), BOARD_POS(6, 4), BOARD_POS(7, 3), BOARD_POS(8, 2));
    evaluator_UpdatePattern(evaluator, PATTERN_ID_DIAG7, evaluator->MirrorLine[index * POW_3_1], index, diff);
    index = BOARD_INDEX_6(in_board, BOARD_POS(1, 3), BOARD_POS(2, 4), BOARD_POS(3, 5), BOARD_POS(4, 6), BOARD_POS(5, 7), BOARD_POS(6, 8));
    evaluator_UpdatePattern(evaluator, PATTERN_ID_DIAG6, evaluator->MirrorLine[index * POW_3_2], index, diff);
    index = BOARD_INDEX_6(in_board, BOARD_POS(3, 1), BOARD_POS(4, 2), BOARD_POS(5, 3), BOARD_POS(6, 4), BOARD_POS(7, 5), BOARD_POS(8, 6));
    evaluator_UpdatePattern(evaluator, PATTERN_ID_DIAG6, evaluator->MirrorLine[index * POW_3_2], index, diff);
    index = BOARD_INDEX_6(in_board, BOARD_POS(1, 6), BOARD_POS(2, 5), BOARD_POS(3, 4), BOARD_POS(4, 3), BOARD_POS(5, 2), BOARD_POS(6, 1));
    evaluator_UpdatePattern(evaluator, PATTERN_ID_DIAG6, evaluator->MirrorLine[index * POW_3_2], index, diff);
    index = BOARD_INDEX_6(in_board, BOARD_POS(3, 8), BOARD_POS(4, 7), BOARD_POS(5, 6), BOARD_POS(6, 5), BOARD_POS(7, 4), BOARD_POS(8, 3));
    evaluator_UpdatePattern(evaluator, PATTERN_ID_DIAG6, evaluator->MirrorLine[index * POW_3_2], index, diff);
    index = BOARD_INDEX_5(in_board, BOARD_POS(1, 4), BOARD_POS(2, 5), BOARD_POS(3, 6), BOARD_POS(4, 7), BOARD_POS(5, 8));
    evaluator_UpdatePattern(evaluator, PATTERN_ID_DIAG5, evaluator->MirrorLine[index * POW_3_3], index, diff);
    index = BOARD_INDEX_5(in_board, BOARD_POS(4, 1), BOARD_POS(5, 2), BOARD_POS(6, 3), BOARD_POS(7, 4), BOARD_POS(8, 5));
    evaluator_UpdatePattern(evaluator, PATTERN_ID_DIAG5, evaluator->MirrorLine[index * POW_3_3], index, diff);
    index = BOARD_INDEX_5(in_board, BOARD_POS(1, 5), BOARD_POS(2, 4), BOARD_POS(3, 3), BOARD_POS(4, 2), BOARD_POS(5, 1));
    evaluator_UpdatePattern(evaluator, PATTERN_ID_DIAG5, evaluator->MirrorLine[index * POW_3_3], index, diff);
    index = BOARD_INDEX_5(in_board, BOARD_POS(4, 8), BOARD_POS(5, 7), BOARD_POS(6, 6), BOARD_POS(7, 5), BOARD_POS(8, 4));
    evaluator_UpdatePattern(evaluator, PATTERN_ID_DIAG5, evaluator->MirrorLine[index * POW_3_3], index, diff);
    index = BOARD_INDEX_4(in_board, BOARD_POS(1, 5), BOARD_POS(2, 6), BOARD_POS(3, 7), BOARD_POS(4, 8));
    evaluator_UpdatePattern(evaluator, PATTERN_ID_DIAG4, evaluator->MirrorLine[index * POW_3_4], index, diff);
    index = BOARD_INDEX_4(in_board, BOARD_POS(5, 1), BOARD_POS(6, 2), BOARD_POS(7, 3), BOARD_POS(8, 4));
    evaluator_UpdatePattern(evaluator, PATTERN_ID_DIAG4, evaluator->MirrorLine[index * POW_3_4], index, diff);
    index = BOARD_INDEX_4(in_board, BOARD_POS(1, 4), BOARD_POS(2, 3), BOARD_POS(3, 2), BOARD_POS(4, 1));
    evaluator_UpdatePattern(evaluator, PATTERN_ID_DIAG4, evaluator->MirrorLine[index * POW_3_4], index, diff);
    index = BOARD_INDEX_4(in_board, BOARD_POS(5, 8), BOARD_POS(6, 7), BOARD_POS(7, 6), BOARD_POS(8, 5));
    evaluator_UpdatePattern(evaluator, PATTERN_ID_DIAG4, evaluator->MirrorLine[index * POW_3_4], index, diff);
    evaluator_UpdatePattern(evaluator, PATTERN_ID_EDGE8, BOARD_INDEX_8(in_board, BOARD_POS(1, 1), BOARD_POS(2, 1), BOARD_POS(3, 1), BOARD_POS(4, 1), BOARD_POS(5, 1), BOARD_POS(6, 1), BOARD_POS(7, 1), BOARD_POS(2, 2)), -1, diff);
    evaluator_UpdatePattern(evaluator, PATTERN_ID_EDGE8, BOARD_INDEX_8(in_board, BOARD_POS(8, 1), BOARD_POS(7, 1), BOARD_POS(6, 1), BOARD_POS(5, 1), BOARD_POS(4, 1), BOARD_POS(3, 1), BOARD_POS(2, 1), BOARD_POS(7, 2)), -1, diff);
    evaluator_UpdatePattern(evaluator, PATTERN_ID_EDGE8, BOARD_INDEX_8(in_board, BOARD_POS(1, 8), BOARD_POS(2, 8), BOARD_POS(3, 8), BOARD_POS(4, 8), BOARD_POS(5, 8), BOARD_POS(6, 8), BOARD_POS(7, 8), BOARD_POS(2, 7)), -1, diff);
    evaluator_UpdatePattern(evaluator, PATTERN_ID_EDGE8, BOARD_INDEX_8(in_board, BOARD_POS(8, 8), BOARD_POS(7, 8), BOARD_POS(6, 8), BOARD_POS(5, 8), BOARD_POS(4, 8), BOARD_POS(3, 8), BOARD_POS(2, 8), BOARD_POS(7, 7)), -1, diff);
    evaluator_UpdatePattern(evaluator, PATTERN_ID_EDGE8, BOARD_INDEX_8(in_board, BOARD_POS(1, 1), BOARD_POS(1, 2), BOARD_POS(1, 3), BOARD_POS(1, 4), BOARD_POS(1, 5), BOARD_POS(1, 6), BOARD_POS(1, 7), BOARD_POS(2, 2)), -1, diff);
    evaluator_UpdatePattern(evaluator, PATTERN_ID_EDGE8, BOARD_INDEX_8(in_board, BOARD_POS(1, 8), BOARD_POS(1, 7), BOARD_POS(1, 6), BOARD_POS(1, 5), BOARD_POS(1, 4), BOARD_POS(1, 3), BOARD_POS(1, 2), BOARD_POS(2, 7)), -1, diff);
    evaluator_UpdatePattern(evaluator, PATTERN_ID_EDGE8, BOARD_INDEX_8(in_board, BOARD_POS(8, 1), BOARD_POS(8, 2), BOARD_POS(8, 3), BOARD_POS(8, 4), BOARD_POS(8, 5), BOARD_POS(8, 6), BOARD_POS(8, 7), BOARD_POS(7, 2)), -1, diff);
    evaluator_UpdatePattern(evaluator, PATTERN_ID_EDGE8, BOARD_INDEX_8(in_board, BOARD_POS(8, 8), BOARD_POS(8, 7), BOARD_POS(8, 6), BOARD_POS(8, 5), BOARD_POS(8, 4), BOARD_POS(8, 3), BOARD_POS(8, 2), BOARD_POS(7, 7)), -1, diff);
    index = BOARD_INDEX_8(in_board, BOARD_POS(1, 1), BOARD_POS(2, 1), BOARD_POS(3, 1), BOARD_POS(1, 2), BOARD_POS(2, 2), BOARD_POS(3, 2), BOARD_POS(1, 3), BOARD_POS(2, 3));
    evaluator_UpdatePattern(evaluator, PATTERN_ID_CORNER8, evaluator->MirrorCorner[index], index, diff);
    index = BOARD_INDEX_8(in_board, BOARD_POS(8, 1), BOARD_POS(7, 1), BOARD_POS(6, 1), BOARD_POS(8, 2), BOARD_POS(7, 2), BOARD_POS(6, 2), BOARD_POS(8, 3), BOARD_POS(7, 3));
    evaluator_UpdatePattern(evaluator, PATTERN_ID_CORNER8, evaluator->MirrorCorner[index], index, diff);
    index = BOARD_INDEX_8(in_board, BOARD_POS(1, 8), BOARD_POS(2, 8), BOARD_POS(3, 8), BOARD_POS(1, 7), BOARD_POS(2, 7), BOARD_POS(3, 7), BOARD_POS(1, 6), BOARD_POS(2, 6));
    evaluator_UpdatePattern(evaluator, PATTERN_ID_CORNER8, evaluator->MirrorCorner[index], index, diff);
    index = BOARD_INDEX_8(in_board, BOARD_POS(8, 8), BOARD_POS(7, 8), BOARD_POS(6, 8), BOARD_POS(8, 7), BOARD_POS(7, 7), BOARD_POS(6, 7), BOARD_POS(8, 6), BOARD_POS(7, 6));
    evaluator_UpdatePattern(evaluator, PATTERN_ID_CORNER8, evaluator->MirrorCorner[index], index, diff);
    evaluator_UpdatePattern(evaluator, PATTERN_ID_PARITY, board_CountDisk(in_board, EMPTY) & 1, -1, diff);
}