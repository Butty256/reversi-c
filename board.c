#include <stdlib.h>
#include "board.h"

void board_Clear(Board *board)
{
	for (int i = 0; i < NUM_DISK; i++)
	{
		board->Disk[i] = EMPTY;
	}
	board->Disk[BOARD_POS(4, 4)] = WHITE;
	board->Disk[BOARD_POS(5, 4)] = BLACK;
	board->Disk[BOARD_POS(4, 5)] = BLACK;
	board->Disk[BOARD_POS(5, 5)] = WHITE;

	board->Sp = board->Stack;
}

Board *board_New(void)
{
	Board *newboard;

	newboard = malloc(sizeof(Board));
	if (newboard)
	{
		board_Clear(newboard);
	}

	return newboard;
}

void board_Delete(Board *board)
{
	free(board);
}

int board_FlipLine(Board *board, int in_color, int in_pos, int in_dir)/* in_dir = 0~7 */
{
	int dir[8] = {-11, -10, -9, -1, 1, 9, 10, 11};
	int result = 0;
	int pos = in_pos;

	for (pos = in_pos + dir[in_dir]; board->Disk[pos] == -in_color; pos += dir[in_dir]);
	if (in_color == board->Disk[pos])
	{
		for (pos -= dir[in_dir]; board->Disk[pos] == -in_color; pos -= dir[in_dir])
		{
			result++;
			board->Disk[pos] = in_color;
			BOARD_STACK_PUSH(board, pos);
		}
	}

	return result;
}

int board_Flip(Board *board, int in_color, int in_pos)
{
	int result = 0;

	if (board->Disk[in_pos] != EMPTY) return 0;
	for (int dir = 0; dir < 8; dir++)
	{
		result += board_FlipLine(board, in_color, in_pos, dir);
	}
	if (result > 0)
	{
		board->Disk[in_pos] = in_color;
		BOARD_STACK_PUSH(board, in_pos);
		BOARD_STACK_PUSH(board, -in_color);
		BOARD_STACK_PUSH(board, result);
	}

	return result;
}

int board_Unflip(Board *board)
{
	int result;
	int color;

	if (board->Sp <= board->Stack) return 0;
	result = BOARD_STACK_POP(board);
	color = BOARD_STACK_POP(board);
	board->Disk[BOARD_STACK_POP(board)] = EMPTY;
	for (int i = 0; i < result; i++)
	{
		board->Disk[BOARD_STACK_POP(board)] = color;
	}

	return result;
}

int board_CountFlipLine(const Board *board, int in_color, int in_pos, int in_dir)
{
	int dir[8] = {-11, -10, -9, -1, 1, 9, 10, 11};
	int result = 0;
	int pos = in_pos;

	for (pos = in_pos + dir[in_dir]; board->Disk[pos] == -in_color; pos += dir[in_dir]) result++;
	if (in_color != board->Disk[pos]) return 0;

	return result;
}

int board_CountFlip(const Board *board, int in_color, int in_pos)
{
	int result = 0;

	if (board->Disk[in_pos] != EMPTY) return 0;
	for (int dir = 0; dir < 8; dir++)
	{
		result += board_CountFlipLine(board, in_color, in_pos, dir);
	}

	return result;
}

int board_CanFlip(Board *board, int in_color, int in_pos)
{
	if (board->Disk[in_pos] != EMPTY) return 0;
	for (int dir = 0; dir < 8; dir++)
	{
		if (board_CountFlipLine(board, in_color, in_pos, dir)) return 1;
	}

	return 0;
}

int board_CanPlay(const Board *board, int in_color)
{
	for (int x = 1; x <= BOARD_SIZE; x++)
	{
		for (int y = 1; y <= BOARD_SIZE; y++)
		{
			if (board_CountFlip(board, in_color, BOARD_POS(x, y))) return 1;
		}
	}

	return 0;
}

void board_Copy(const Board *board1, Board *board2)
{
	*board2 = *board1;
	board2->Sp = board1->Sp - board1->Stack + board2->Stack;
}

void board_Reverse(Board *board)
{
	int *p;
	int n;
	for (p = board->Sp; p > board->Stack; )
	{
		p--;
		n = *p;
		p--;
		*p *= -1;
		p -= n + 1;
	}
	for (int pos = 0; pos < NUM_DISK; pos++) board->Disk[pos] *= -1;
}

int board_Disk(const Board *board, int in_pos)
{
	return board->Disk[in_pos];
}

int board_CountDisk(const Board *board, int in_color)
{
	int result = 0;
	for (int x = 1; x <= BOARD_SIZE; x++)
	{
		for (int y = 1; y <= BOARD_SIZE; y++)
		{
			if (board->Disk[BOARD_POS(x, y)] == in_color) result++;
		}
	}

	return result;
}
