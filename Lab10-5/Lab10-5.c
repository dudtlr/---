// 프로그래밍랩 - 10주
// Lab10-4와 동일 : Multiplayer Game
//
#include <stdio.h>
#include <conio.h>
#include <Windows.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

// 색상 정의
#define BLACK	0
#define BLUE1	1
#define GREEN1	2
#define CYAN1	3
#define RED1	4
#define MAGENTA1 5
#define YELLOW1	6
#define GRAY1	7
#define GRAY2	8
#define BLUE2	9
#define GREEN2	10
#define CYAN2	11
#define RED2	12
#define MAGENTA2 13
#define YELLOW2	14
#define WHITE	15

#define STAR1 'O' // player1 표시
#define STAR2 'X' // player2 표시
#define BLANK '.' // ' ' 로하면 흔적이 지워진다 

#define ESC 0x1b //  ESC 누르면 종료

#define SPECIAL1 0xe0 // 특수키는 0xe0 + key 값으로 구성된다.
#define SPECIAL2 0x00 // keypad 경우 0x00 + key 로 구성된다.

#define UP  0x48 // Up key는 0xe0 + 0x48 두개의 값이 들어온다.
#define DOWN 0x50
#define LEFT 0x4b
#define RIGHT 0x4d

#define UP2		'w'
#define DOWN2	's'
#define LEFT2	'a'
#define RIGHT2	'd'

#define WIDTH 80
#define HEIGHT 24

int Delay = 100; // 100 msec delay, 이 값을 줄이면 속도가 빨라진다.
int keep_moving = 1; // 1:계속이동, 0:한칸씩이동.
int time_out = 60; // 제한시간

void removeCursor(void) { // 커서를 안보이게 한다

	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}

void gotoxy(int x, int y) //내가 원하는 위치로 커서 이동
{
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);// WIN32API 함수입니다. 이건 알필요 없어요
}

void putstar(int x, int y, char ch)
{
	gotoxy(x, y);
	putchar(ch);
}
void erasestar(int x, int y)
{
	gotoxy(x, y);
	putchar(BLANK);
}

void textcolor(int fg_color, int bg_color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), fg_color | bg_color << 4);
}
// 화면 지우기고 원하는 배경색으로 설정한다.
void cls(int bg_color, int text_color)
{
	char cmd[100];
	system("cls");
	sprintf(cmd, "COLOR %x%x", bg_color, text_color);
	system(cmd);

}
// box 그리기 함수, ch 문자열로 (x1,y1) ~ (x2,y2) box를 그린다.
void draw_box(int x1, int y1, int x2, int y2, char ch)
{
	int x, y;
	// Lab11-1
}

void player1(unsigned char ch)
{
	static int oldx = 50, oldy = 10, newx = 50, newy = 10;
	int move_flag = 0;
	static int called = 0;
	static unsigned char last_ch = 0;

	if (called == 0) { // 최초 위치
		removeCursor();
		putstar(oldx, oldy, STAR1);
		called = 1;
	}
	if (keep_moving && ch == 0)
		ch = last_ch;
	last_ch = ch;

	switch (ch) {
	case UP:
		if (oldy > 0)
			newy = oldy - 1;
		else { // 벽에 부딛치면 방향을 반대로 이동
			newy = oldy + 1;
			last_ch = DOWN;
		}
		move_flag = 1;
		break;
	case DOWN:
		if (oldy < HEIGHT - 1)
			newy = oldy + 1;
		else {
			newy = oldy - 1;
			last_ch = UP;
		}
		move_flag = 1;
		break;
	case LEFT:
		if (oldx > 0)
			newx = oldx - 1;
		else {
			newx = oldx + 1;
			last_ch = RIGHT;
		}
		move_flag = 1;
		break;
	case RIGHT:
		if (oldx < WIDTH - 1)
			newx = oldx + 1;
		else {
			newx = oldx - 1;
			last_ch = LEFT;
		}
		move_flag = 1;
		break;
	}
	if (move_flag) {
		erasestar(oldx, oldy); // 마지막 위치의 * 를 지우고
		putstar(newx, newy, STAR1); // 새로운 위치에서 * 를 표시한다.
		oldx = newx; // 마지막 위치를 기억한다.
		oldy = newy;
	}

}
void player2(unsigned char ch)
{
	static int oldx = 20, oldy = 10, newx = 20, newy = 10;
	int move_flag = 0;
	static int called = 0;
	static char last_ch = 0;

	if (called == 0) { // 최초 위치 
		putstar(oldx, oldy, STAR2);
		called = 1;
	}
	// 방향키가 눌려지지 않은 경우 예전에 이동하던 방향으로 계속 이동
	if (keep_moving && ch == 0)
		ch = last_ch;
	last_ch = ch;

	switch (ch) {
	case UP2:
		if (oldy > 0)
			newy = oldy - 1;
		else { // 벽에 부딛치면 방향을 반대로 이동
			newy = oldy + 1;
			last_ch = DOWN2;
		}
		move_flag = 1;
		break;
	case DOWN2:
		if (oldy < HEIGHT - 1)
			newy = oldy + 1;
		else {
			newy = oldy - 1;
			last_ch = UP2;
		}
		move_flag = 1;
		break;
	case LEFT2:
		if (oldx > 0)
			newx = oldx - 1;
		else {
			newx = oldx + 1;
			last_ch = RIGHT2;
		}
		move_flag = 1;
		break;
	case RIGHT2:
		if (oldx < WIDTH - 1)
			newx = oldx + 1;
		else {
			newx = oldx - 1;
			last_ch = LEFT2;
		}
		move_flag = 1;
		break;
	}
	if (move_flag) {
		erasestar(oldx, oldy); // 마지막 위치의 X 를 지우고
		putstar(newx, newy, STAR2); // 새로운 위치에서 X 를 표시한다.
		oldx = newx; // 마지막 위치를 기억한다.
		oldy = newy;
		move_flag = 1; // 1:계속 이동, 0:한칸씩 이동
	}
}

// 임의의 위치 (0<= x < WIDTH, 1<= y < HEIGHT 에 gold를 표시한다.
#define GOLD	"*"
void show_gold()
{
	int x, y;
	// 완성할 것

}
// (30,0) 에 "남은시간 : 60" 식으로 시간을 표시한다.
void show_time(int remain_time)
{
	// 완성할 것
}

void main()
{

	printf("┌──────────────────────────────────────┐\n");
	printf("│       ■■■    ■    ■     ■■■     ■■■    ■■■■■      ■■■   │\n"); 
	printf("│     ■         ■    ■    ■    ■   ■    ■       ■          ■  ■    │\n");
	printf("│      ■■■    ■■■■    ■    ■   ■    ■       ■          ■ ■     │\n"); 
	printf("│           ■   ■    ■    ■    ■   ■    ■       ■           ■       │\n");
	printf("│          ■   ■    ■    ■    ■   ■    ■       ■                     │\n"); 
	printf("│  ■■■■    ■    ■     ■■■     ■■■        ■            ■        │\n");
	printf("│                                                                            │\n"); 
	printf("│ Ver 3.0                                                  ## Made By LGH ## │\n");
	printf("└──────────────────────────────────────┘\n"); 



	
}
