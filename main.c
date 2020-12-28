#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "board.h"
#include "com.h"
#include "evaluator.h"

const char *fonts[3] = {"●", "・", "○"};

void show_Board(const Board *board);
int undo(Board *board, int *color);
int move_Player(Board *board, int *color);
void move_Computer(Com *com, Board *board, int color);
int set_All(Board **board, Com **com, Evaluator **evaluator, int *black, int *white);
int play_Game(void);

void show_Board(const Board *board)
{
	puts("   a b c d e f g h");
	for (int y = 1; y <= 8; y++)
	{
		printf(" %d ", y);
		for (int x = 1; x <= 8; x++)
		{
			printf("%s", fonts[board_Disk(board, BOARD_POS(x, y)) + 1]);
		}
	putchar('\n');
	}
	printf("BLACK%s : %2d, WHITE%s : %2d\n", fonts[2], board_CountDisk(board, BLACK), fonts[0], board_CountDisk(board, WHITE));
}

int undo(Board *board, int *color)
{
	if (board->Sp <= board->Stack) return 0;
	while (*((board->Sp) - 2) == *color)
	{
		board_Unflip(board);
	}
	if (board_Unflip(board) != 0) *color *= -1;
	return 1;
}

int move_Player(Board *board, int *color)
{
	char buf[80];
	int x, y;
	while(1)
	{
		putchar('>');
		fgets(buf, sizeof(buf), stdin);
		if (strncmp(buf, "quit", 4) == 0) return -1;
		if (strncmp(buf, "undo", 4) == 0)
		{
			if (undo(board, color)) return 0;
			else
			{
				puts("cannot undo");
				continue;
			}
		}
		if (!buf[2])
		{
			puts("input error");
			continue;
		}
		x = buf[0] - 'a' + 1;
		y = buf[2] - '1' + 1;
		if (x < 1 || x > 8 || y < 1 || y > 8)
		{
			puts("input error");
			continue;
		}
		if (board_Flip(board, *color, BOARD_POS(x, y)) == 0)
		{
			puts("cannot flip");
			continue;
		}
		break;
	}
	return 0;
}

void move_Computer(Com *com, Board *board, int color)
{
	int pos;
	puts("thinking now...");
	pos = com_NextMove(com, board, color);
	printf(">%c %c\n", 'a' + (pos%(BOARD_SIZE+2)) - 1, '1' + (pos/(BOARD_SIZE+2)) - 1);
	board_Flip(board, color, pos);
}

int set_All(Board **board, Com **com, Evaluator **evaluator, int *black, int *white)
{
	char buf[80];
	*evaluator = evaluator_New();
	if (!evaluator_Load(*evaluator, "eval.dat"))
	{
		puts("eval.bat cannot load");
		return -1;
	}
	*board = board_New();
	if (board == NULL)
	{
		puts("board cannot make");
		return -1;
	}
	board_Clear(*board);
	*com = com_New(*evaluator);
	if (*com == NULL)
	{
		puts("com cannot make");
		return -1;
	}
	com_SetLevel(*com, 7, 14);
	while (1)
	{
		printf("BLACK%s : 1 Player , 0 Computer\n", fonts[2]);
		fgets(buf, sizeof(buf), stdin);
		*black = atoi(buf);
		if (*black == 0 || *black == 1) break;
		puts("input error");
	}
	while (1)
	{
		printf("WHITE%s : 1 Player , 0 Computer\n", fonts[0]);
		fgets(buf, sizeof(buf), stdin);
		*white = atoi(buf);
		if (*white == 0 || *white == 1) break;
		puts("input error");
	}
	return 0;
}

int play_Game(void)
{
	/* ply 0, com 1 */
	int black, white, b_num, w_num, color = BLACK;
	int start, end;
	Evaluator *evaluator;
	Board *board;
	Com *com;
	if (set_All(&board, &com, &evaluator, &black, &white)) return -1;
	while(1)
	{
		start = time(NULL);

		if (board_CanPlay(board, color) == 0)
		{
			color *= -1;
			if(board_CanPlay(board, color) == 0) break;
		}
		putchar('\n');
		if (color == BLACK) printf("turn : BLACK%s\n", fonts[2]);
		else printf("turn : WHITE%s\n", fonts[0]);
		show_Board(board);

		if (color == BLACK)
		{
			if (black)
			{
				if (move_Player(board, &color)) break;
			}
			else move_Computer(com, board, color);
		}
		else
		{
			if (white)
			{
				if (move_Player(board, &color)) break;
			}
			else move_Computer(com, board, color);
		}

		color *= -1;

		end = time(NULL);

		printf("time = %3d[s]\n", end - start + 1);
	}
	show_Board(board);
	b_num = board_CountDisk(board, BLACK);
	w_num = board_CountDisk(board, WHITE);
	if (b_num > w_num)
	{
		printf("win : BLACK%s\n", fonts[2]);
		b_num += board_CountDisk(board, EMPTY);
	}
	else if (w_num > b_num)
	{
		printf("win : WHITE%s\n", fonts[0]);
		w_num += board_CountDisk(board, EMPTY);
	}
	if (b_num == w_num) puts("draw");
	printf("BLACK%s : %2d  WHITE%s : %2d\n", fonts[2], b_num, fonts[0], w_num);
	com_Delete(com);
	board_Delete(board);
	evaluator_Delete(evaluator);
	return 0;
}

int main(void)
{
	char buf[80];
	while (1)
	{
		if (play_Game()) break;
		puts("1 : replay, 0 : end");
		fgets(buf, sizeof(buf), stdin);
		if (buf[0] == '0') break;
	}
	return 0;
}
