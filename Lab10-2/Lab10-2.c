// 프로그래밍랩 10주
// Lab10-2 글자 색 변경
// 
#include <stdio.h>
#include <conio.h>
#include <Windows.h>

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

#define ESC	0x1b // ESC key 값
void gotoxy(int x, int y) //내가 원하는 위치로 커서 이동
{
	COORD pos; // Windows.h 에 정의
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void textcolor(int fg_color, int bg_color)
{
	SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), fg_color | bg_color<<4);
}

void removeCursor(void){ // 커서를 안보이게 한다
	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible=0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}
void showCursor(void){ // 커서를 보이게 한다
	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible=1;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}

// 화면 지우기고 원하는 배경색으로 설정한다.
void cls(int text_color, int bg_color) 
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
	//완성할것

	for (x = x1; x <= x2; x++) {
		//Sleep(10);
		gotoxy(x, y1);
		putchar(ch); // printf("%c",ch);
		gotoxy(x, y2);
		putchar(ch); // printf("%c",ch);
	}


	for (y = y1; y <= y2; y++) {
		gotoxy(x1, y);
		putchar(ch);
		gotoxy(x2, y);
		putchar(ch);

	}
}
void main()
{
	int bg_color, fg_color;
	char ch;
	
	removeCursor();
	for (bg_color=0;bg_color<16;bg_color++) {
		fg_color = 15 - bg_color;
		cls(fg_color, bg_color); // 화면 전체 배경색과 글자색을 정한다
		gotoxy(30,11);
		printf("배경색=%d 글자색=%d", bg_color, fg_color);
		ch = getch(); // Enter Key 대신 아무키나 누르면 진행된다
		if (ch==ESC) // ESC 누르면 중단
			break;
	}
	// 화면을 지우고
	cls(BLACK, WHITE);
	for (bg_color=0;bg_color<=0xf;bg_color++) {
		gotoxy(16,bg_color+4);
		for (fg_color=0;fg_color<=0xf;fg_color++) {
			textcolor(fg_color, bg_color); // 화면색과 관계없이 글자색을 변경한다
			printf(" %x ", fg_color);
		}
		ch = getch(); // Enter Key 대신 아무키나 누르면 진행된다
		if (ch==ESC) // ESC 누루면 중단
			break;
	}
	gotoxy(0,0);
	cls(BLACK, WHITE);
	getch();

	 //강의노트 5 page 참조해서 색상 사용해서 완성할 것
	gotoxy(21, 11); 
	textcolor(RED2, CYAN2);
	 printf("한성대학교 컴퓨터공학과 프로그래밍랩");
	 textcolor(YELLOW2, BLUE1);
	draw_box(0,0,79,22,'*'); // 화면에 box를 그린다.
}

