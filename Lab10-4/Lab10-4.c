// ���α׷��ַ� - 10��
// Lab10-4 : 2���� Player ������
//
#include <stdio.h>
#include <conio.h>
#include <Windows.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

// ���� ����
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

#define STAR1 'O' // player1 ǥ��
#define STAR2 'X' // player2 ǥ��
#define BLANK '.' // ' ' ���ϸ� ������ �������� 

#define ESC 0x1b //  ESC ������ ����

#define SPECIAL1 0xe0 // Ư��Ű�� 0xe0 + key ������ �����ȴ�.
#define SPECIAL2 0x00 // keypad ��� 0x00 + key �� �����ȴ�.

#define UP  0x48 // Up key�� 0xe0 + 0x48 �ΰ��� ���� ���´�.
#define DOWN 0x50
#define LEFT 0x4b
#define RIGHT 0x4d
#define ENTER '\r'

#define UP2		'w'
#define DOWN2	's'
#define LEFT2	'a'
#define RIGHT2	'd'

#define WIDTH 80
#define HEIGHT 24

int Delay = 100; // 100 msec delay, �� ���� ���̸� �ӵ��� ��������.
int keep_moving = 1; // 1:����̵�, 0:��ĭ���̵�.
int time_out = 60; // ���ѽð�

void removeCursor(void){ // Ŀ���� �Ⱥ��̰� �Ѵ�

	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible=0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}

void gotoxy(int x, int y) //���� ���ϴ� ��ġ�� Ŀ�� �̵�
{
	COORD pos = {x, y};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);// WIN32API �Լ��Դϴ�. �̰� ���ʿ� �����
}

void putstar(int x, int y,  char ch)
{
	gotoxy(x,y);
	putchar(ch);
}
void erasestar(int x, int y)
{
	gotoxy(x,y);
	putchar(BLANK);
}

void textcolor(int fg_color, int bg_color)
{
	SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), fg_color | bg_color<<4);
}
// ȭ�� ������ ���ϴ� �������� �����Ѵ�.
void cls(int bg_color, int text_color) 
{
	char cmd[100];
	system("cls");
	sprintf(cmd, "COLOR %x%x", bg_color, text_color);
	system(cmd);

}
// box �׸��� �Լ�, ch ���ڿ��� (x1,y1) ~ (x2,y2) box�� �׸���.
void draw_box(int x1, int y1, int x2, int y2, char ch)
{
	int x, y;
	// Lab11-1
}

void player1(unsigned char ch)
{
	static int oldx=50,oldy=10, newx=50, newy=10;
	int move_flag = 0;
	static int called = 0;
	static unsigned char last_ch=0;
	
	if (called == 0) { // ���� ��ġ
		removeCursor();
		putstar(oldx,oldy,STAR1);
		called = 1;
	}
	if (keep_moving && ch==0)
		ch = last_ch;
	last_ch = ch;

	switch(ch) {
	case UP:
		if (oldy>0)
			newy = oldy - 1;
		else { // ���� �ε�ġ�� ������ �ݴ�� �̵�
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
		if (oldx > 0 )
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
		erasestar(oldx, oldy); // ������ ��ġ�� * �� �����
		putstar(newx, newy, STAR1); // ���ο� ��ġ���� * �� ǥ���Ѵ�.
		oldx = newx; // ������ ��ġ�� ����Ѵ�.
		oldy = newy;
	}

}
void player2(unsigned char ch)
{
	static int oldx=20,oldy=10, newx=20, newy=10;
	int move_flag = 0;
	static int called = 0;
	static char last_ch=0;
	
	if (called == 0) { // ���� ��ġ 
		putstar(oldx,oldy,STAR2);
		called = 1;
	}
	// ����Ű�� �������� ���� ��� ������ �̵��ϴ� �������� ��� �̵�
	if (keep_moving && ch==0) 
		ch = last_ch;
	last_ch = ch;
		
	switch(ch) {
	case UP2:
		if (oldy>0)
			newy = oldy - 1;
		else { // ���� �ε�ġ�� ������ �ݴ�� �̵�
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
		if (oldx > 0 )
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
		erasestar(oldx, oldy); // ������ ��ġ�� X �� �����
		putstar(newx, newy, STAR2); // ���ο� ��ġ���� X �� ǥ���Ѵ�.
		oldx = newx; // ������ ��ġ�� ����Ѵ�.
		oldy = newy;
		move_flag = 1; // 1:��� �̵�, 0:��ĭ�� �̵�
	}
}

// ������ ��ġ (0<= x < WIDTH, 1<= y < HEIGHT �� gold�� ǥ���Ѵ�.
#define GOLD	"*"
void show_gold()
{
	//�ϼ��� ��
	//srand(time(NULL));
	int x,y;
	x = rand() % WIDTH;
	y = rand() % (HEIGHT-1) + 1;
	
		gotoxy(x, y);
		printf(GOLD);
}


// (30,0) �� "�����ð� : 60" ������ �ð��� ǥ���Ѵ�.
void show_time(int remain_time)
{
	// �ϼ��� ��
	gotoxy(30, 0);
	textcolor(RED2, BLUE2);
	printf("�����ð� : %02d", remain_time);
}

void main()
{
	// Ư��Ű 0xe0 �� �Է¹������� unsigned char �� �����ؾ� ��
	unsigned char ch;
	int run_time, start_time, remain_time, last_remain_time;
	int gold_time = 0;

	srand(time(NULL));
	start_time=time(NULL);
	last_remain_time = remain_time = time_out;
	show_time(remain_time);
	while (1) {
		// �ð� check
		run_time = time(NULL) - start_time;
		if (run_time>gold_time && (run_time%5==0)) {
			// 5�ʿ� �ѹ��� GOLD �� ǥ���Ѵ�

			show_gold();
			gold_time = run_time; // ������ GOLD ǥ�� �ð� ���
		}
		remain_time = time_out - run_time;
		if (remain_time<last_remain_time) {
			// �����ð��� ǥ���Ѵ�.
			show_time(remain_time); // �ð��� ���Ҷ��� ���
			last_remain_time = remain_time;
		}
		if (remain_time==0) // �ð� ����
			break;
		 
		if (kbhit()==1) {  // Ű���尡 ������ ������
			ch = getch(); // key ���� �д´�
			//
			// ESC ������ ���α׷� ����
			if (ch == ESC) break;
			//
			if (ch==SPECIAL1 || ch==SPECIAL2) { // ���� Ư��Ű
				// ���� ��� UP key�� ��� 0xe0 0x48 �ΰ��� ���ڰ� ���´�.
				ch = getch();
				// Player1�� ����Ű�� �����δ�.
				switch(ch) {
				case UP:
				case DOWN:
				case LEFT:
				case RIGHT:
					player1(ch);
					player2(0);
					break;
				default:// Ư��Ű ������ ����Ű�� �ƴ� ���
					player1(0);
					player2(0);
				}
			} else {
				// Player2�� AWSD �� �����δ�.
				switch(ch) {
				case UP2:
				case DOWN2:
				case LEFT2:
				case RIGHT2:
					player2(ch);
					player1(0);
					break;
				default: // AWSD �� �ƴ� ���
					player1(0);
					player2(0);
				}
			}
		} else { 
			// keyboard �� �������� ������ ��� �����δ�.
			// �̵����̴� �������� ��� �̵�
			player1(0);
			player2(0);
		}
		Sleep(Delay); // Delay�� ���̸� �ӵ��� ��������
	}
}
