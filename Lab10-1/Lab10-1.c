// 프로그래밍랩 10주
// Lab10-1 gotoxy 를 이용한 화면 디자인
// 
#include <stdio.h>
#include <conio.h> // 키보드 입력 받을 때 사용
#include <Windows.h>
#include <stdlib.h> // rand 함수를 사용하기 위해 추가
#include<time.h> 
#include<stdbool.h>     // 논리형 자료를 쓰기 위한 라이브러리

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

#define WIDTH 148
#define HEIGHT 34

#define ESC 0x1b //  ESC 누르면 종료

#define SPECIAL1 0xe0 // 특수키는 0xe0 + key 값으로 구성된다.
#define SPECIAL2 0x00 // keypad 경우 0x00 + key 로 구성된다.

#define UP  0x48 // Up key는 0xe0 + 0x48 두개의 값이 들어온다.
#define DOWN 0x50
#define LEFT 0x4b
#define RIGHT 0x4d
#define ENTER '\r'
#define SPACE 0x20

int Delay = 10;
int frame_count = 0; // game 진행 frame count 로 속도 조절용으로 사용된다.
int p1_frame_sync = 4; //플레이어의 이동속도


#define StartX 72  //시작 유저의 x좌표
#define StartY 30  //시작 유저의 y좌표

int remainingTime = 5; // 보스에서 제한 시간 2분
static int called = 0;

static int oldx = StartX, oldy = StartY; // 플레이어의 old 좌표
static int newx = StartX, newy = StartY; //플레이어의 new 좌표
static int keep_moving = 4;  //1:계속이동

#define MAXBULLET 30  //플레이어의 최대 총알 수
#define TRUE 1
#define FALSE 0

struct {
	int exist;
	int x, y;
}Bullet[MAXBULLET];

#define SPACE 0x20
#define MAXENEMY 15

struct BossEnemy {
	int exist;
	int x, y;
	int move;
}Boss;

//struct BossEnemy Boss;



//@@@@@@@@@적@@@@@

struct {
	int exist;
	int x, y;
	int number;
	int move;
	int type;
}Enemy[MAXENEMY];

char* EnemyType[] = {"★※★","♬★♬","㈜＠㈜","※＠※","▼★▼", "＃＃＃","＆＠＆","♠★♠","♣♣♣","◈★◈"};

//@@@@@@@@@적@@@@@


//@@@@@@@@@적 총알@@@@@@@@@@@
//#define MAXENEMYBULLET 20 // 적 최대 총알 수
#define MAXENEMYBULLET 10 // 적 최대 총알 수

static int enemybulletuse = 1; //적 총알 1초마다 생성
static int enemybullet_frame_sync = 60;  //적 총알 속도조절

struct {
	int exist;
	int x, y;
}EnemyBullet[MAXENEMYBULLET];

bool IsLevel1 = true; // 1단계
bool IsLevel2 = false; // 2단계
bool IsLevel3 = false; // 3단계
bool IsBoss = false; // 보스 단계


bool IsEasy = false; // 난이도 이지 
bool IsHard = false; // 난이도 하드



//@@@@@@@@@적 총알@@@@@@@@@@@


// 점수 판 들어갈 항목 들 전역 변수
static int score = 0; // 점수
static int heart = 5; // 생명
static int BossLife = 50; // 보스 생명
#define ITEM "<H>"
#define SPEEDITEM "<S>"
#define POWERITEM "<P>"
#define MISILEITEM "<M>"

int items[WIDTH][HEIGHT] = { 0 };
int items_count = 0;
int items_use = 35; //아이템 나타나는 시간(목숨 아이템)


int speeditems[WIDTH][HEIGHT] = { 0 };
int speeditems_count = 0;
int speeditems_use = 23; //아이템 나타나는 시간(스피드 아이템)


int MyJet = 1; // 제트기 고르는 변수

int PickMyLevel = 1; //난이도 기본 easy 1 / hard 2





void gotoxy(int x, int y) //내가 원하는 위치로 커서 이동
{
	COORD pos; // Windows.h 에 정의
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos); //Windows.h 에 정의 되어있다.
}

void textcolor(int fg_color, int bg_color)
{
	SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), fg_color | bg_color<<4);
}


// 시간을 표시하는 함수
void displayTime(int minutes, int seconds) {
	textcolor(CYAN1, BLACK);
	gotoxy(122, 3);
	printf("남은 시간: %02d:%02d", minutes, seconds);
}

//레벨 1 클리어 메시지
void Level1ClearMessage(int x, int y) {

	


	textcolor(GREEN1, BLACK);
	gotoxy(x, y );
	printf("축하합니다~~");
	gotoxy(x, y+1);
	printf("레벨 1을 클리어 하였습니다!!");
	gotoxy(x, y + 2);
	printf("레벨 2로 넘어갑니다!!");
	Sleep(1000);
}

//레벨 2 클리어 메시지
void Level2ClearMessage(int x, int y) {


	textcolor(GREEN1, BLACK);
	gotoxy(x, y);
	printf("축하합니다~~");
	gotoxy(x, y + 1);
	printf("레벨 2를 클리어 하였습니다!!");
	gotoxy(x, y + 2);
	printf("레벨 3으로 넘어갑니다!!");
	Sleep(1000);
}

//레벨 3 클리어 메시지
void Level3ClearMessage(int x, int y) {

	textcolor(GREEN1, BLACK);
	gotoxy(x, y);
	printf("축하합니다~~");
	gotoxy(x, y + 1);
	printf("레벨 3을 클리어 하였습니다!!");
	gotoxy(x, y + 2);
	printf("보스 스테이지로 넘어갑니다!!");
	Sleep(1000);
}


//레벨 보스 클리어 메시지
void BossClearMessage(int x, int y) {

	textcolor(GREEN1, BLACK);
	gotoxy(x, y);
	printf("축하합니다!!!!★★★★★★★★★★★★★★");
	gotoxy(x, y + 1);
	printf("보스 스테이지를 클리어 하였습니다!!");
	gotoxy(x, y + 2);
	printf("축하합니다★★★★★★★★★★★★★★★★");
	Sleep(1000);
}


//Boss 그림 !!
void BossDraw(int x, int y) {

	textcolor(RED1, BLACK);
	gotoxy(x, y);			printf("  ★★    ★★");
	gotoxy(x , y + 1);		printf("@@@@@@@@@@@@@@@@");
	gotoxy(x, y + 2);		printf("@@●@@@@@@@@●@@");
	gotoxy(x, y + 3);		printf("@@@@@@@@@@@@@@@@");
	gotoxy(x, y + 4);       printf("◈◈◈◈◈◈◈◈");
	gotoxy(x, y + 5);       printf("◈◈◈    ◈◈◈");
	gotoxy(x, y + 6);       printf("◈◈◈◈◈◈◈◈");


	
}

//Boss 맞았을 때 그림
void BossDraw2(int x, int y) {

	textcolor(GRAY1, BLACK);
	gotoxy(x, y);			printf("  ★★    ★★");
	gotoxy(x, y + 1);		printf("@@@@@@@@@@@@@@@@");
	textcolor(MAGENTA2, BLACK);
	gotoxy(x, y + 2);		printf("@@●@@@@@@@@●@@");
	gotoxy(x, y + 3);		printf("@@@@@@@@@@@@@@@@");
	textcolor(CYAN1, BLACK);
	gotoxy(x, y + 4);       printf("◈◈◈◈◈◈◈◈");
	gotoxy(x, y + 5);       printf("◈◈◈    ◈◈◈");
	gotoxy(x, y + 6);       printf("◈◈◈◈◈◈◈◈");

}

void BossErase(int x, int y) {

	textcolor(RED1, BLACK);
	gotoxy(x, y);			printf("                ");
	gotoxy(x, y+1);			printf("                ");
	gotoxy(x, y+2);			printf("                ");
	gotoxy(x, y+3);			printf("                ");
	gotoxy(x, y+4);			printf("                ");
	gotoxy(x, y+5);			printf("                ");
	gotoxy(x, y+6);			printf("                ");

}

//비행기 맞았을 때 
void Zet10(int x, int y) {

	textcolor(RED1, BLACK);
	gotoxy(x, y);
	printf("    ▲    ");
	gotoxy(x, y + 1);
	printf("★■♠■★");
}

//비행기 1 그림
void Zet1(int x, int y) {

	textcolor(BLUE2, BLACK);
	gotoxy(x, y);
	printf("    ▲    ");
	gotoxy(x, y + 1);
	printf("★■♠■★");
}


//비행기 2 그림
void Zet2(int x, int y) {
	
	textcolor(GREEN2, BLACK);
	gotoxy(x, y);
	printf("    ▲    ");
	gotoxy(x, y + 1);
	printf("◀■♥■▶");
}


//비행기 3 그림
void Zet3(int x, int y) {

	textcolor(CYAN2, BLACK);
	gotoxy(x, y);
	printf("    ▲    ");
	gotoxy(x, y + 1);
	printf("◀◆●◆▶");
}

//난이도 쉬운버젼 디자인
void Level1(int x, int y) {

	textcolor(GREEN1, BLACK);
	gotoxy(x, y);
	printf("★★★      ★       ★★★  ★      ★  \n");
	gotoxy(x, y+1);
	printf("★        ★  ★    ★         ★  ★    \n");
	gotoxy(x, y+2);
	printf("★★★   ★★★★    ★★★      ★      \n");
	gotoxy(x, y + 3);
	printf("★      ★      ★        ★     ★      \n");
	gotoxy(x, y + 4);
	printf("★★★ ★        ★  ★★★      ★      \n");
}
//난이도 어려운 버젼 디자인
void Level2(int x, int y) {

	textcolor(RED1, BLACK);
	gotoxy(x, y);
	printf("★  ★      ★      ★★★★  ★★★  \n");
	gotoxy(x, y + 1);
	printf("★  ★    ★  ★    ★    ★  ★   ★ \n");
	gotoxy(x, y + 2);
	printf("★★★   ★★★★   ★★★    ★    ★\n");
	gotoxy(x, y + 3);
	printf("★  ★  ★      ★  ★  ★    ★   ★ \n");
	gotoxy(x, y + 4);
	printf("★  ★ ★        ★ ★    ★  ★★★  \n");
}


void Zet1Draw(int x, int y) {
	
	if (MyJet == 1) {
		Zet1(x, y);

	}

	if (MyJet == 2) {
		Zet2(x, y);
	}

	if (MyJet == 3) {
		Zet3(x, y);
	}
}

void Zet1Erase(int x, int y) {
	gotoxy(x, y);
	printf("          ");
	gotoxy(x, y + 1);
	printf("          ");
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
void cls(int text_color, int bg_color) // 화면 지우기
{
	char cmd[100];
	system("cls");
	sprintf(cmd, "COLOR %x%x", bg_color, text_color);
	system(cmd);
}

// box 그리기 함수, ch 문자로 (x1,y1) ~ (x2,y2) box를 그린다.
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
// box 그리기 함수, ch 문자열로 (x1,y1) ~ (x2,y2) box를 그린다.
// 한글 문자를 그리는 용도로 사용 (예, "□" 로 ㅁ 벽을 그리는 경우)
void draw_box2(int x1, int y1, int x2, int y2, char* ch)    //x1,y1 0.0 이고 x2,y2가 오른쪽 하단 맨끝
{
	int x, y;
	// 완성할 수 있으면 할 것
	int fg_color, bg_color; // 색깔 변수 추가


	// 박스가 1칸이 아니라 2칸을 차지한다 그래서 좌표를 +1 이 아니라 +2 해줘야 한다!!!!
	// 특수문자는 2칸이라 x좌표 2칸차지한다!!! y좌표는 1칸이라 신경 안써도 된다. 
	for (x = x1; x <= x2; x += 2) {
		//Sleep(1);
		fg_color = rand() % 14+1; // 랜덤한 색깔 선택
		bg_color = rand() % 3;
		textcolor(fg_color, 0);
		gotoxy(x, y1);
		printf("%s", ch);
		gotoxy(x, y2);
		printf("%s", ch);
	}


	for (y = y1; y <= y2; y++) {
		gotoxy(x1, y);
		printf("%s", ch);
		gotoxy(x2, y);
		printf("%s", ch);

	}



}

void changeTextColor()
{
	int fg_color = rand() % 14 + 1; // 랜덤한 색깔 선택
	textcolor(fg_color, 0);
}

void centerConsoleWindow() //콘솔화면 정중앙에 보여주기!
{
	HWND consoleHandle = GetConsoleWindow();
	RECT consoleRect;

	GetWindowRect(consoleHandle, &consoleRect);

	int consoleWidth = consoleRect.right - consoleRect.left;
	int consoleHeight = consoleRect.bottom - consoleRect.top;

	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	//int consoleX = (screenWidth - consoleWidth) / 2;
	//int consoleY = (screenHeight - consoleHeight) / 2;
	int consoleX = (screenWidth ) / 2;
	int consoleY = (screenHeight) / 2;

	SetWindowPos(consoleHandle, 0, 40, 20, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
}


void navigateMenu(int* selectedOption) {
	int key;
	while (1) {
		key = getch();
		if (key == 224) {
			key = getch();
			switch (key) {
			case 72: // Up arrow
				if (*selectedOption > 1) {
					*selectedOption -= 1;
				}
				break;
			case 80: // Down arrow
				if (*selectedOption < 3) {
					*selectedOption += 1;
				}
				break;
			}
		}
		else if (key == 13) { // Enter key
			break;
		}
	}
}

void IntroPage() {  // 시작 인트로 화면
	int i;
	while (1) {


		for (i = 0; i <= 20; i++) {
			Sleep(10);
			//draw_box2(i * 2, i, 78 - i * 2, 22 - i, "□");
			draw_box2(i * 2, i, WIDTH  - i * 2, HEIGHT - 2 - i, "★");
		}

		for (i = 20; i >= 0; i--) {
			Sleep(10);
			draw_box2(i * 2, i, WIDTH  - i * 2, HEIGHT - 2 - i, "  ");
		}
		cls(WHITE, BLACK);
		break;
	}
}

int FinishPage() {
	int selectedOption = 1;
	// 특수키 0xe0 을 입력받으려면 unsigned char 로 선언해야 함
	unsigned char ch;
	bool StartPage = TRUE;
	char key;
	while (StartPage) {
		changeTextColor();
		printf("                                           ★★★★★★          ★        ★             ★    ★★★★★★            \n");
		printf("                                         ★                     ★★       ★★         ★★    ★                      \n");
		printf("                                         ★                    ★  ★      ★ ★       ★ ★	★			            \n");
		printf("                                         ★     ★★★        ★★★★     ★  ★     ★  ★    ★★★★★★            \n");
		changeTextColor();
		printf("                                         ★       ★         ★      ★	   ★	★   ★   ★    ★				        \n");
		printf("                                         ★       ★        ★        ★   ★ 	 ★ ★    ★	★			            \n");
		printf("                                           ★★★★★★    ★          ★  ★	  ★      ★    ★★★★★★			\n");

		changeTextColor();
		printf("\n"); 
		printf("                       ★★★★★★     ★               ★★★★★★★            ★           ★★★★★★★           ★       ★     \n");
		printf("                     ★                 ★               ★                       ★★          ★          ★           ★       ★     \n");
		printf("                     ★                 ★               ★                      ★  ★         ★          ★           ★       ★     \n");
		printf("                     ★                 ★               ★★★★★★★         ★★★★        ★★★★★★★           ★       ★     \n");
		changeTextColor();
		printf("                     ★                 ★               ★                    ★      ★       ★  ★                   ★       ★     \n");
		printf("                     ★                 ★               ★                   ★        ★      ★      ★                               \n");
		printf("                       ★★★★★★     ★★★★★★★   ★★★★★★★      ★          ★     ★          ★           ★       ★     \n");
		printf("\n");
		textcolor(11, 0);
		printf("\n");
		printf("\n");
		printf("                                                                                       made by 1891093 전영식			\n");
		textcolor(15, 0);
		printf("\n");

		if (selectedOption == 1) {
			printf("                                         ★★★★★★★★★★★★★★   MENU   ★★★★★★★★★★★★★★★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                      ->   1.돌아가기                           ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                           2.게임종료                           ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★\n");



		}

		
		else {
			printf("                                         ★★★★★★★★★★★★★★   MENU   ★★★★★★★★★★★★★★★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                         1.돌아가기                             ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                      -> 2.게임종료                             ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★\n");



		}



		Sleep(100);
		gotoxy(0, 0);
		if (kbhit()) {
			key = getch();

			if (key == UP) {
				selectedOption--;
				if (selectedOption == 0) { selectedOption = 2; }
			}
			if (key == DOWN) {
				selectedOption++;
				if (selectedOption == 3) { selectedOption = 1; }
			}

			if (key == ENTER) {
				switch (selectedOption) {
				case 1:
					// 첫 번째 옵션 처리
					StartPage = false;
					cls(WHITE, BLACK); // 기존 화면 지우기
					break;
				case 2:
					StartPage = false;
					cls(WHITE, BLACK); // 기존 화면 지우기
					break;
				case 3:
					StartPage = false;
					cls(WHITE, BLACK); // 기존 화면 지우기
					break;
				default:
					// 잘못된 옵션 처리
					break;
				}
			}
		}

	}

	return selectedOption;
}

int MenuPage() {
	int selectedOption = 1;
	// 특수키 0xe0 을 입력받으려면 unsigned char 로 선언해야 함
	unsigned char ch;
	bool StartPage = TRUE;
	char key;
	while (StartPage) {
		changeTextColor();
		printf("\n");
		printf("           ★★★★★      ★        ★    ★★★★★★    ★★★★★★    ★★★★★★    ★★★★★★    ★★         ★     ★★★★★★          \n");
		printf("         ★                ★        ★    ★        ★    ★        ★         ★              ★         ★ ★        ★   ★                      \n");
		printf("         ★                ★        ★    ★        ★    ★        ★         ★              ★         ★   ★      ★   ★                      \n");
		printf("           ★★★★★      ★★★★★★    ★        ★    ★        ★         ★              ★         ★     ★    ★   ★      ★★★          \n");
		changeTextColor();
		printf("                     ★    ★        ★    ★        ★    ★        ★         ★              ★         ★       ★  ★   ★       ★             \n");
		printf("                     ★    ★        ★    ★        ★    ★        ★         ★              ★         ★         ★★   ★       ★             \n");
		printf("           ★★★★★      ★        ★    ★★★★★★    ★★★★★★         ★         ★★★★★★    ★           ★     ★★★★★★          \n");
		printf("\n");
		changeTextColor();
		printf("                                           ★★★★★★          ★        ★             ★    ★★★★★★            \n");
		printf("                                         ★                     ★★       ★★         ★★    ★                      \n");
		printf("                                         ★                    ★  ★      ★ ★       ★ ★	★			            \n");
		printf("                                         ★     ★★★        ★★★★     ★  ★     ★  ★    ★★★★★★            \n");
		changeTextColor();
		printf("                                         ★       ★         ★      ★	   ★	★   ★   ★    ★				        \n");
		printf("                                         ★       ★        ★        ★   ★ 	 ★ ★    ★	★			            \n");
		printf("                                           ★★★★★★    ★          ★  ★	  ★      ★    ★★★★★★			\n");
		textcolor(11, 0);
		printf("\n");
		printf("\n");
		printf("                                                                                       made by 1891093 전영식			\n");
		textcolor(15, 0);
		printf("\n");

		if (selectedOption == 1) {
			printf("                                         ★★★★★★★★★★★★★★   MENU   ★★★★★★★★★★★★★★★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                    ->   1.게임 시작                            ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                         2.게임 설명                            ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                         3.게임 종료                            ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★\n");



			}

		else if (selectedOption == 2) {
			printf("                                         ★★★★★★★★★★★★★★   MENU   ★★★★★★★★★★★★★★★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                         1.게임 시작                            ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                    ->   2.게임 설명                            ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                         3.게임 종료                            ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★\n");



		}
		else {
			printf("                                         ★★★★★★★★★★★★★★   MENU   ★★★★★★★★★★★★★★★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                         1.게임 시작                            ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                         2.게임 설명                            ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                    ->   3.게임 종료                            ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★\n");


		}



		Sleep(100);
		gotoxy(0, 0);
		if (kbhit()) {
			key = getch();

			if (key == UP) {
				selectedOption--;
				if (selectedOption == 0) { selectedOption = 3; }
			}
			if (key == DOWN) {
				selectedOption++;
				if (selectedOption == 4) { selectedOption = 1; }
			}

			if (key == ENTER) {
				switch (selectedOption) {
				case 1:
					// 첫 번째 옵션 처리
					StartPage = false;
					cls(WHITE, BLACK); // 기존 화면 지우기
					break;
				case 2:
					StartPage = false;
					cls(WHITE, BLACK); // 기존 화면 지우기
					break;
				case 3:
					StartPage = false;
					cls(WHITE, BLACK); // 기존 화면 지우기
					break;
				default:
					// 잘못된 옵션 처리
					break;
				}
			}
		}

	}

	return selectedOption;
}

int LoadingPage() {
	//cls(WHITE, BLACK);
	//gotoxy(0, 0);
	int selectedOption = 1;
	// 특수키 0xe0 을 입력받으려면 unsigned char 로 선언해야 함
	unsigned char ch;
	bool StartPage = TRUE;
	char key;
	while (StartPage) {
		textcolor(MAGENTA2, 0);
		printf("\n");
		printf("           ★★★★★      ★        ★    ★★★★★★    ★★★★★★    ★★★★★★    ★★★★★★    ★★         ★     ★★★★★★          \n");
		printf("         ★                ★        ★    ★        ★    ★        ★         ★              ★         ★ ★        ★   ★                      \n");
		printf("         ★                ★        ★    ★        ★    ★        ★         ★              ★         ★   ★      ★   ★                      \n");
		printf("           ★★★★★      ★★★★★★    ★        ★    ★        ★         ★              ★         ★     ★    ★   ★      ★★★          \n");
		
		printf("                     ★    ★        ★    ★        ★    ★        ★         ★              ★         ★       ★  ★   ★       ★             \n");
		printf("                     ★    ★        ★    ★        ★    ★        ★         ★              ★         ★         ★★   ★       ★             \n");
		printf("           ★★★★★      ★        ★    ★★★★★★    ★★★★★★         ★         ★★★★★★    ★           ★     ★★★★★★          \n");
		printf("\n");
		
		printf("                                           ★★★★★★          ★        ★             ★    ★★★★★★            \n");
		printf("                                         ★                     ★★       ★★         ★★    ★                      \n");
		printf("                                         ★                    ★  ★      ★ ★       ★ ★	★			            \n");
		printf("                                         ★     ★★★        ★★★★     ★  ★     ★  ★    ★★★★★★            \n");
		printf("                                         ★       ★         ★      ★	   ★	★   ★   ★    ★				        \n");
		printf("                                         ★       ★        ★        ★   ★ 	 ★ ★    ★	★			            \n");
		printf("                                           ★★★★★★    ★          ★  ★	  ★      ★    ★★★★★★			\n");
		textcolor(11, 0);
		printf("\n");
		printf("\n");
		printf("                                                                                       made by 1891093 전영식			\n");
		textcolor(15, 0);
		
		textcolor(BLUE1, 0);
		printf("	                                                        ★ 로딩 중입니다 !! ★\n");

		textcolor(YELLOW1, 0);
		if (selectedOption == 1) {
			printf("                                         ★★★★★★★★★★★★★   Loading  \n");
			printf("                                         ★                                                                \n");
			printf("                                         ★                                                                \n");
			printf("                                         ★                                                                \n");
			printf("                                         ★                                                                \n");
			printf("                                         ★                                                                \n");
			printf("                                         ★                        로딩 중 30%% !! .                          \n");
			printf("                                         ★                                                                \n");
			printf("                                         ★                                                                \n");
			printf("                                         ★                                                                \n");
			printf("                                         ★                                                                \n");
			printf("											\n");
		}

		else if (selectedOption == 2) {
			printf("                                         ★★★★★★★★★★★★★   Loading  \n");
			printf("                                         ★                                                                \n");
			printf("                                         ★                                                                \n");
			printf("                                         ★                                                                \n");
			printf("                                         ★                                                                \n");
			printf("                                         ★                                                                \n");
			printf("                                         ★                        로딩 중 70%% !! . .                      \n");
			printf("                                         ★                                                                \n");
			printf("                                         ★                                                                \n");
			printf("                                         ★                                                                \n");
			printf("                                         ★                                                                \n");
			printf("                                         ★                                                                \n");
			printf("                                         ★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★\n");



		}
		else if (selectedOption == 3) {
			printf("                                         ★★★★★★★★★★★★★   Loading  ★★★★★★★★★★★★★★★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                        로딩 중 100%% !! . . .                   ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★\n");



		}
		else {
			printf("                                         ★★★★★★★★★★★★★   Loading  ★★★★★★★★★★★★★★★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                         곧 시작 합니다 !!                      ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★\n");


		}
		Sleep(100);
		selectedOption++;
		Sleep(1000);

		gotoxy(0, 0);
		
		if (selectedOption == 5) {
			cls(WHITE, BLACK); // 기존 화면 지우기
			StartPage = FALSE;
			break;
		}


	}

	return selectedOption;
}

int Level2LoadingPage() {
	cls(WHITE, BLACK);
	//gotoxy(0, 0);
	int selectedOption = 1;
	// 특수키 0xe0 을 입력받으려면 unsigned char 로 선언해야 함
	unsigned char ch;
	bool StartPage = TRUE;
	char key;
	while (StartPage) {
		textcolor(MAGENTA2, 0);
		printf("\n");
		printf("           ★★★★★      ★        ★    ★★★★★★    ★★★★★★    ★★★★★★    ★★★★★★    ★★         ★     ★★★★★★          \n");
		printf("         ★                ★        ★    ★        ★    ★        ★         ★              ★         ★ ★        ★   ★                      \n");
		printf("         ★                ★        ★    ★        ★    ★        ★         ★              ★         ★   ★      ★   ★                      \n");
		printf("           ★★★★★      ★★★★★★    ★        ★    ★        ★         ★              ★         ★     ★    ★   ★      ★★★          \n");

		printf("                     ★    ★        ★    ★        ★    ★        ★         ★              ★         ★       ★  ★   ★       ★             \n");
		printf("                     ★    ★        ★    ★        ★    ★        ★         ★              ★         ★         ★★   ★       ★             \n");
		printf("           ★★★★★      ★        ★    ★★★★★★    ★★★★★★         ★         ★★★★★★    ★           ★     ★★★★★★          \n");
		printf("\n");

		printf("                                           ★★★★★★          ★        ★             ★    ★★★★★★            \n");
		printf("                                         ★                     ★★       ★★         ★★    ★                      \n");
		printf("                                         ★                    ★  ★      ★ ★       ★ ★	★			            \n");
		printf("                                         ★     ★★★        ★★★★     ★  ★     ★  ★    ★★★★★★            \n");
		printf("                                         ★       ★         ★      ★	   ★	★   ★   ★    ★				        \n");
		printf("                                         ★       ★        ★        ★   ★ 	 ★ ★    ★	★			            \n");
		printf("                                           ★★★★★★    ★          ★  ★	  ★      ★    ★★★★★★			\n");
		textcolor(11, 0);
		printf("\n");
		printf("\n");
		printf("                                                                                       made by 1891093 전영식			\n");
		textcolor(15, 0);

		textcolor(BLUE1, 0);
		printf("	                                                        ★ 2단계 화면으로 넘어가는 중입니다 !! ★\n");

		textcolor(YELLOW1, 0);
		if (selectedOption == 1) {
			printf("                                         ★★★★★★★★★★★★★   Loading  \n");
			printf("                                         ★                                                                \n");
			printf("                                         ★                                                                \n");
			printf("                                         ★                                                                \n");
			printf("                                         ★                                                                \n");
			printf("                                         ★                                                                \n");
			printf("                                         ★                        로딩 중 30%% !! .                          \n");
			printf("                                         ★                                                                \n");
			printf("                                         ★                                                                \n");
			printf("                                         ★                                                                \n");
			printf("                                         ★                                                                \n");
			printf("											\n");
		}

		else if (selectedOption == 2) {
			printf("                                         ★★★★★★★★★★★★★   Loading  \n");
			printf("                                         ★                                                                \n");
			printf("                                         ★                                                                \n");
			printf("                                         ★                                                                \n");
			printf("                                         ★                                                                \n");
			printf("                                         ★                                                                \n");
			printf("                                         ★                        로딩 중 70%% !! . .                      \n");
			printf("                                         ★                                                                \n");
			printf("                                         ★                                                                \n");
			printf("                                         ★                                                                \n");
			printf("                                         ★                                                                \n");
			printf("                                         ★                                                                \n");
			printf("                                         ★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★\n");



		}
		else if (selectedOption == 3) {
			printf("                                         ★★★★★★★★★★★★★   Loading  ★★★★★★★★★★★★★★★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                        로딩 중 100%% !! . . .                   ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★\n");



		}
		else {
			printf("                                         ★★★★★★★★★★★★★   Loading  ★★★★★★★★★★★★★★★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                         곧 시작 합니다 !!                      ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★\n");


		}
		Sleep(100);
		selectedOption++;
		Sleep(1000);

		gotoxy(0, 0);

		if (selectedOption == 5) {
			cls(WHITE, BLACK); // 기존 화면 지우기
			StartPage = FALSE;
			break;
		}


	}

	return selectedOption;
}

int Level3LoadingPage() {
	cls(WHITE, BLACK);
	//gotoxy(0, 0);
	int selectedOption = 1;
	// 특수키 0xe0 을 입력받으려면 unsigned char 로 선언해야 함
	unsigned char ch;
	bool StartPage = TRUE;
	char key;
	while (StartPage) {
		textcolor(MAGENTA2, 0);
		printf("\n");
		printf("           ★★★★★      ★        ★    ★★★★★★    ★★★★★★    ★★★★★★    ★★★★★★    ★★         ★     ★★★★★★          \n");
		printf("         ★                ★        ★    ★        ★    ★        ★         ★              ★         ★ ★        ★   ★                      \n");
		printf("         ★                ★        ★    ★        ★    ★        ★         ★              ★         ★   ★      ★   ★                      \n");
		printf("           ★★★★★      ★★★★★★    ★        ★    ★        ★         ★              ★         ★     ★    ★   ★      ★★★          \n");

		printf("                     ★    ★        ★    ★        ★    ★        ★         ★              ★         ★       ★  ★   ★       ★             \n");
		printf("                     ★    ★        ★    ★        ★    ★        ★         ★              ★         ★         ★★   ★       ★             \n");
		printf("           ★★★★★      ★        ★    ★★★★★★    ★★★★★★         ★         ★★★★★★    ★           ★     ★★★★★★          \n");
		printf("\n");

		printf("                                           ★★★★★★          ★        ★             ★    ★★★★★★            \n");
		printf("                                         ★                     ★★       ★★         ★★    ★                      \n");
		printf("                                         ★                    ★  ★      ★ ★       ★ ★	★			            \n");
		printf("                                         ★     ★★★        ★★★★     ★  ★     ★  ★    ★★★★★★            \n");
		printf("                                         ★       ★         ★      ★	   ★	★   ★   ★    ★				        \n");
		printf("                                         ★       ★        ★        ★   ★ 	 ★ ★    ★	★			            \n");
		printf("                                           ★★★★★★    ★          ★  ★	  ★      ★    ★★★★★★			\n");
		textcolor(11, 0);
		printf("\n");
		printf("\n");
		printf("                                                                                       made by 1891093 전영식			\n");
		textcolor(15, 0);

		textcolor(BLUE1, 0);
		printf("	                                                        ★ 3단계 화면으로 넘어가는 중입니다 !! ★\n");

		textcolor(YELLOW1, 0);
		if (selectedOption == 1) {
			printf("                                         ★★★★★★★★★★★★★   Loading  \n");
			printf("                                         ★                                                                \n");
			printf("                                         ★                                                                \n");
			printf("                                         ★                                                                \n");
			printf("                                         ★                                                                \n");
			printf("                                         ★                                                                \n");
			printf("                                         ★                        로딩 중 30%% !! .                          \n");
			printf("                                         ★                                                                \n");
			printf("                                         ★                                                                \n");
			printf("                                         ★                                                                \n");
			printf("                                         ★                                                                \n");
			printf("											\n");
		}

		else if (selectedOption == 2) {
			printf("                                         ★★★★★★★★★★★★★   Loading  \n");
			printf("                                         ★                                                                \n");
			printf("                                         ★                                                                \n");
			printf("                                         ★                                                                \n");
			printf("                                         ★                                                                \n");
			printf("                                         ★                                                                \n");
			printf("                                         ★                        로딩 중 70%% !! . .                      \n");
			printf("                                         ★                                                                \n");
			printf("                                         ★                                                                \n");
			printf("                                         ★                                                                \n");
			printf("                                         ★                                                                \n");
			printf("                                         ★                                                                \n");
			printf("                                         ★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★\n");



		}
		else if (selectedOption == 3) {
			printf("                                         ★★★★★★★★★★★★★   Loading  ★★★★★★★★★★★★★★★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                        로딩 중 100%% !! . . .                   ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★\n");



		}
		else {
			printf("                                         ★★★★★★★★★★★★★   Loading  ★★★★★★★★★★★★★★★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                         곧 시작 합니다 !!                      ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★\n");


		}
		Sleep(100);
		selectedOption++;
		Sleep(1000);

		gotoxy(0, 0);

		if (selectedOption == 5) {
			cls(WHITE, BLACK); // 기존 화면 지우기
			StartPage = FALSE;
			break;
		}


	}

	return selectedOption;
}


int BossLoadingPage() {
	cls(WHITE, BLACK);
	//gotoxy(0, 0);
	int selectedOption = 1;
	// 특수키 0xe0 을 입력받으려면 unsigned char 로 선언해야 함
	unsigned char ch;
	bool StartPage = TRUE;
	char key;
	while (StartPage) {
		textcolor(MAGENTA2, 0);
		printf("\n");
		printf("           ★★★★★      ★        ★    ★★★★★★    ★★★★★★    ★★★★★★    ★★★★★★    ★★         ★     ★★★★★★          \n");
		printf("         ★                ★        ★    ★        ★    ★        ★         ★              ★         ★ ★        ★   ★                      \n");
		printf("         ★                ★        ★    ★        ★    ★        ★         ★              ★         ★   ★      ★   ★                      \n");
		printf("           ★★★★★      ★★★★★★    ★        ★    ★        ★         ★              ★         ★     ★    ★   ★      ★★★          \n");

		printf("                     ★    ★        ★    ★        ★    ★        ★         ★              ★         ★       ★  ★   ★       ★             \n");
		printf("                     ★    ★        ★    ★        ★    ★        ★         ★              ★         ★         ★★   ★       ★             \n");
		printf("           ★★★★★      ★        ★    ★★★★★★    ★★★★★★         ★         ★★★★★★    ★           ★     ★★★★★★          \n");
		printf("\n");

		printf("                                           ★★★★★★          ★        ★             ★    ★★★★★★            \n");
		printf("                                         ★                     ★★       ★★         ★★    ★                      \n");
		printf("                                         ★                    ★  ★      ★ ★       ★ ★	★			            \n");
		printf("                                         ★     ★★★        ★★★★     ★  ★     ★  ★    ★★★★★★            \n");
		printf("                                         ★       ★         ★      ★	   ★	★   ★   ★    ★				        \n");
		printf("                                         ★       ★        ★        ★   ★ 	 ★ ★    ★	★			            \n");
		printf("                                           ★★★★★★    ★          ★  ★	  ★      ★    ★★★★★★			\n");
		textcolor(11, 0);
		printf("\n");
		printf("\n");
		printf("                                                                                       made by 1891093 전영식			\n");
		textcolor(15, 0);

		textcolor(BLUE1, 0);
		printf("	                                                        ★ 보스 스테이지로 넘어가는 중입니다 !! ★\n");

		textcolor(YELLOW1, 0);
		if (selectedOption == 1) {
			printf("                                         ★★★★★★★★★★★★★   Loading  \n");
			printf("                                         ★                                                                \n");
			printf("                                         ★                                                                \n");
			printf("                                         ★                                                                \n");
			printf("                                         ★                                                                \n");
			printf("                                         ★                                                                \n");
			printf("                                         ★                        로딩 중 30%% !! .                          \n");
			printf("                                         ★                                                                \n");
			printf("                                         ★                                                                \n");
			printf("                                         ★                                                                \n");
			printf("                                         ★                                                                \n");
			printf("											\n");
		}

		else if (selectedOption == 2) {
			printf("                                         ★★★★★★★★★★★★★   Loading  \n");
			printf("                                         ★                                                                \n");
			printf("                                         ★                                                                \n");
			printf("                                         ★                                                                \n");
			printf("                                         ★                                                                \n");
			printf("                                         ★                                                                \n");
			printf("                                         ★                        로딩 중 70%% !! . .                      \n");
			printf("                                         ★                                                                \n");
			printf("                                         ★                                                                \n");
			printf("                                         ★                                                                \n");
			printf("                                         ★                                                                \n");
			printf("                                         ★                                                                \n");
			printf("                                         ★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★\n");



		}
		else if (selectedOption == 3) {
			printf("                                         ★★★★★★★★★★★★★   Loading  ★★★★★★★★★★★★★★★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                        로딩 중 100%% !! . . .                   ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★\n");



		}
		else {
			printf("                                         ★★★★★★★★★★★★★   Loading  ★★★★★★★★★★★★★★★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                         곧 시작 합니다 !!                      ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★                                                                ★\n");
			printf("                                         ★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★\n");


		}
		Sleep(100);
		selectedOption++;
		Sleep(1000);

		gotoxy(0, 0);

		if (selectedOption == 5) {
			cls(WHITE, BLACK); // 기존 화면 지우기
			StartPage = FALSE;
			break;
		}


	}

	return selectedOption;
}
int pickMyJet() {
	int selectedOption = 1;
	// 특수키 0xe0 을 입력받으려면 unsigned char 로 선언해야 함
	unsigned char ch;
	bool pickMyJetPage = TRUE;
	char key;

	
	while (pickMyJetPage) {
		changeTextColor();
		printf("\n"); 
		printf("                      ★★★★★★   ★★★★★★    ★★★★★★   ★★★★★★    ★★★★★★      ★★★★★    ★        ★               \n");
		printf("                              ★     ★                   ★        ★        ★         ★         ★              ★     ★               \n");
		changeTextColor();
		printf("                            ★       ★★★★★★         ★        ★★★★★★         ★         ★              ★★★                  \n");
		printf("                          ★         ★                   ★        ★                   ★         ★              ★     ★             \n");
		printf("                        ★           ★                   ★        ★                   ★         ★              ★       ★           \n");
		changeTextColor();
		printf("                      ★★★★★★   ★★★★★★         ★        ★              ★★★★★★      ★★★★★    ★        ★           \n");

		printf("\n");
		textcolor(WHITE, BLACK);

		if (selectedOption == 1) {
			printf("                      ★★★★★★★★★★★★★★★★★        제트기를 선택하세요 !!      ★★★★★★★★★★★★★★★★★★ \n");
			printf("                      ★                                                                                                      ★\n");
			printf("                      ★                                                                                                      ★\n");
			printf("                      ★                                                                                                      ★\n");
			printf("                      ★                                                                                                      ★\n");
			printf("                      ★            1번 제트기                       2번 제트기                         3번 제트기            ★\n");
			printf("                      ★      ★★★★★★★★★★★           ★★★★★★★★★★★             ★★★★★★★★★★★      ★\n");
			printf("                      ★      ★                  ★           ★                  ★             ★                  ★      ★\n");
			printf("                      ★      ★                  ★           ★                  ★             ★                  ★      ★\n");
			printf("                      ★      ★                  ★           ★                  ★             ★                  ★      ★\n");
			printf("                      ★      ★                  ★           ★                  ★             ★                  ★      ★\n");
			printf("                      ★      ★                  ★           ★                  ★             ★                  ★      ★\n");
			printf("                      ★      ★                  ★           ★                  ★             ★                  ★      ★\n");
			printf("                      ★      ★                  ★           ★                  ★             ★                  ★      ★\n");
			printf("                      ★      ★                  ★           ★                  ★             ★                  ★      ★\n");
			printf("                      ★      ★★★★★★★★★★★           ★★★★★★★★★★★             ★★★★★★★★★★★      ★\n");
			printf("                      ★                                                                                                      ★\n");
			printf("                      ★                ↑                                                                                    ★\n");
			printf("                      ★                                                                                                      ★\n");
			printf("                      ★                                                                                                      ★\n");
			printf("                      ★                                                                                                      ★\n");
			printf("                      ★                                                                                                      ★\n");
			printf("                      ★                                                                                                      ★\n");
			printf("                      ★★★★★★★★★★★★★★★★★        제트기를 선택하세요 !!      ★★★★★★★★★★★★★★★★★★ \n");

			
			Zet1(36, 18);

			Zet2(69, 18);

			Zet3(104, 18);

		
		}

		else if (selectedOption == 2) {
			printf("                      ★★★★★★★★★★★★★★★★★        제트기를 선택하세요 !!      ★★★★★★★★★★★★★★★★★★ \n");
			printf("                      ★                                                                                                      ★\n");
			printf("                      ★                                                                                                      ★\n");
			printf("                      ★                                                                                                      ★\n");
			printf("                      ★                                                                                                      ★\n");
			printf("                      ★            1번 제트기                       2번 제트기                         3번 제트기            ★\n");
			printf("                      ★      ★★★★★★★★★★★           ★★★★★★★★★★★             ★★★★★★★★★★★      ★\n");
			printf("                      ★      ★                  ★           ★                  ★             ★                  ★      ★\n");
			printf("                      ★      ★                  ★           ★                  ★             ★                  ★      ★\n");
			printf("                      ★      ★                  ★           ★                  ★             ★                  ★      ★\n");
			printf("                      ★      ★                  ★           ★                  ★             ★                  ★      ★\n");
			printf("                      ★      ★                  ★           ★                  ★             ★                  ★      ★\n");
			printf("                      ★      ★                  ★           ★                  ★             ★                  ★      ★\n");
			printf("                      ★      ★                  ★           ★                  ★             ★                  ★      ★\n");
			printf("                      ★      ★                  ★           ★                  ★             ★                  ★      ★\n");
			printf("                      ★      ★★★★★★★★★★★           ★★★★★★★★★★★             ★★★★★★★★★★★      ★\n");
			printf("                      ★                                                                                                      ★\n");
			printf("                      ★                                                 ↑                                                   ★\n");
			printf("                      ★                                                                                                      ★\n");
			printf("                      ★                                                                                                      ★\n");
			printf("                      ★                                                                                                      ★\n");
			printf("                      ★                                                                                                      ★\n");
			printf("                      ★                                                                                                      ★\n");
			printf("                      ★★★★★★★★★★★★★★★★★        제트기를 선택하세요 !!      ★★★★★★★★★★★★★★★★★★ \n");


			Zet1(36, 18);

			Zet2(69, 18);

			Zet3(104, 18);


		}
		else {
			printf("                      ★★★★★★★★★★★★★★★★★        제트기를 선택하세요 !!      ★★★★★★★★★★★★★★★★★★ \n");
			printf("                      ★                                                                                                      ★\n");
			printf("                      ★                                                                                                      ★\n");
			printf("                      ★                                                                                                      ★\n");
			printf("                      ★                                                                                                      ★\n");
			printf("                      ★            1번 제트기                       2번 제트기                         3번 제트기            ★\n");
			printf("                      ★      ★★★★★★★★★★★           ★★★★★★★★★★★             ★★★★★★★★★★★      ★\n");
			printf("                      ★      ★                  ★           ★                  ★             ★                  ★      ★\n");
			printf("                      ★      ★                  ★           ★                  ★             ★                  ★      ★\n");
			printf("                      ★      ★                  ★           ★                  ★             ★                  ★      ★\n");
			printf("                      ★      ★                  ★           ★                  ★             ★                  ★      ★\n");
			printf("                      ★      ★                  ★           ★                  ★             ★                  ★      ★\n");
			printf("                      ★      ★                  ★           ★                  ★             ★                  ★      ★\n");
			printf("                      ★      ★                  ★           ★                  ★             ★                  ★      ★\n");
			printf("                      ★      ★                  ★           ★                  ★             ★                  ★      ★\n");
			printf("                      ★      ★★★★★★★★★★★           ★★★★★★★★★★★             ★★★★★★★★★★★      ★\n");
			printf("                      ★                                                                                                      ★\n");
			printf("                      ★                                                                                    ↑                ★\n");
			printf("                      ★                                                                                                      ★\n");
			printf("                      ★                                                                                                      ★\n");
			printf("                      ★                                                                                                      ★\n");
			printf("                      ★                                                                                                      ★\n");
			printf("                      ★                                                                                                      ★\n");
			printf("                      ★★★★★★★★★★★★★★★★★        제트기를 선택하세요 !!      ★★★★★★★★★★★★★★★★★★ \n");


			Zet1(36, 18);

			Zet2(69, 18);

			Zet3(104, 18);


		}



		
		gotoxy(0, 0);
		if (kbhit()) {
			key = getch();

			if (key == LEFT) {
				selectedOption--;
				if (selectedOption == 0) { selectedOption = 3; }
			}
			if (key == RIGHT) {
				selectedOption++;
				if (selectedOption == 4) { selectedOption = 1; }
			}

			if (key == ENTER) {
				switch (selectedOption) {
				case 1:
					// 첫 번째 옵션 처리
					pickMyJetPage = false;
					cls(WHITE, BLACK); // 기존 화면 지우기
					MyJet = 1;
					break;
				case 2:
					pickMyJetPage = false;
					cls(WHITE, BLACK); // 기존 화면 지우기
					MyJet = 2;
					break;
				case 3:
					pickMyJetPage = false;
					cls(WHITE, BLACK); // 기존 화면 지우기
					MyJet = 3;
					break;
				default:
					// 잘못된 옵션 처리
					break;
				}
			}
		}
		Sleep(300);
	}

	return selectedOption;
}

int pickGameLevel() {
	int selectedOption = 1;
	// 특수키 0xe0 을 입력받으려면 unsigned char 로 선언해야 함
	unsigned char ch;
	bool pickMyLevelPage = TRUE;
	char key;



	while (pickMyLevelPage) {
		changeTextColor();
		printf("\n");
		
		printf("        ★             ★★★★★★    ★        ★   ★★★★★★    ★             ★★★★★★   ★★★★★★      ★★★★★    ★        ★     \n");
		
		changeTextColor(); 
		printf("        ★             ★               ★      ★    ★              ★             ★        ★        ★         ★              ★     ★ \n");
		printf("        ★             ★★★★★★      ★    ★     ★★★★★★    ★             ★★★★★★        ★         ★              ★★★    \n");
		
		changeTextColor(); 
		printf("        ★             ★                 ★  ★      ★              ★             ★                  ★         ★              ★     ★   \n");
		printf("        ★             ★                   ★        ★              ★             ★                  ★         ★              ★       ★  \n");
		printf("        ★★★★★★   ★★★★★★         ★        ★★★★★★    ★★★★★★   ★             ★★★★★★      ★★★★★    ★        ★            \n");

	
		textcolor(WHITE, BLACK);
		printf("\n");

		if (selectedOption == 1) {
			printf("                      ★★★★★★★★★★★★★★★★★        난이도를 선택하세요 !!      ★★★★★★★★★★★★★★★★★★★ \n");
			printf("                      ★                                                                                                        ★\n");
			printf("                      ★                                                                                                        ★\n");
			printf("                      ★                                                                                                        ★\n");
			printf("                      ★                                                                                                        ★\n");
			printf("                      ★                                                                                                        ★\n");
			printf("                      ★  ★★★★★★★★★★★★★★★★★★★★★★★      ★★★★★★★★★★★★★★★★★★★★★★★    ★\n");
			printf("                      ★  ★                                          ★      ★                                          ★    ★\n");
			printf("                      ★  ★                                          ★      ★                                          ★    ★\n");
			printf("                      ★  ★                                          ★      ★                                          ★    ★\n");
			printf("                      ★  ★                                          ★      ★                                          ★    ★\n");
			printf("                      ★  ★                                          ★      ★                                          ★    ★\n");
			printf("                      ★  ★                                          ★      ★                                          ★    ★\n");
			printf("                      ★  ★                                          ★      ★                                          ★    ★\n");
			printf("                      ★  ★                                          ★      ★                                          ★    ★\n");
			printf("                      ★  ★★★★★★★★★★★★★★★★★★★★★★★      ★★★★★★★★★★★★★★★★★★★★★★★    ★\n");
			printf("                      ★                                                                                                        ★\n");
			printf("                      ★                        ↑                                                                              ★\n");
			printf("                      ★                                                                                                        ★\n");
			printf("                      ★                                                                                                        ★\n");
			printf("                      ★                                                                                                        ★\n");
			printf("                      ★                                                                                                        ★\n");
			printf("                      ★                                                                                                        ★\n");
			printf("                      ★★★★★★★★★★★★★★★★★        난이도를 선택하세요 !!      ★★★★★★★★★★★★★★★★★★★ \n");


			Level1(29, 17);

			Level2(82, 17);

			


		}

		
		else {
			printf("                      ★★★★★★★★★★★★★★★★★        난이도를 선택하세요 !!      ★★★★★★★★★★★★★★★★★★★ \n");
			printf("                      ★                                                                                                        ★\n");
			printf("                      ★                                                                                                        ★\n");
			printf("                      ★                                                                                                        ★\n");
			printf("                      ★                                                                                                        ★\n");
			printf("                      ★                                                                                                        ★\n");
			printf("                      ★  ★★★★★★★★★★★★★★★★★★★★★★★      ★★★★★★★★★★★★★★★★★★★★★★★    ★\n");
			printf("                      ★  ★                                          ★      ★                                          ★    ★\n");
			printf("                      ★  ★                                          ★      ★                                          ★    ★\n");
			printf("                      ★  ★                                          ★      ★                                          ★    ★\n");
			printf("                      ★  ★                                          ★      ★                                          ★    ★\n");
			printf("                      ★  ★                                          ★      ★                                          ★    ★\n");
			printf("                      ★  ★                                          ★      ★                                          ★    ★\n");
			printf("                      ★  ★                                          ★      ★                                          ★    ★\n");
			printf("                      ★  ★                                          ★      ★                                          ★    ★\n");
			printf("                      ★  ★★★★★★★★★★★★★★★★★★★★★★★      ★★★★★★★★★★★★★★★★★★★★★★★    ★\n");
			printf("                      ★                                                                                                        ★\n");
			printf("                      ★                                                                             ↑                         ★\n");
			printf("                      ★                                                                                                        ★\n");
			printf("                      ★                                                                                                        ★\n");
			printf("                      ★                                                                                                        ★\n");
			printf("                      ★                                                                                                        ★\n");
			printf("                      ★                                                                                                        ★\n");
			printf("                      ★★★★★★★★★★★★★★★★★        난이도를 선택하세요 !!      ★★★★★★★★★★★★★★★★★★★ \n");


			Level1(29, 17);

			Level2(82, 17);




		}



		Sleep(300);
		gotoxy(0, 0);
		if (kbhit()) {
			key = getch();

			if (key == LEFT) {
				selectedOption--;
				if (selectedOption == 0) { selectedOption = 2; }
			}
			if (key == RIGHT) {
				selectedOption++;
				if (selectedOption == 3) { selectedOption = 1; }
			}

			if (key == ENTER) {
				switch (selectedOption) {
				case 1:
					// 첫 번째 옵션 처리
					pickMyLevelPage = false;
					cls(WHITE, BLACK); // 기존 화면 지우기
					PickMyLevel = 1;
					IsEasy = true; // 난이도 이지 
					IsHard = false; // 난이도 하드
					break;
				case 2:
					pickMyLevelPage = false;
					cls(WHITE, BLACK); // 기존 화면 지우기
					PickMyLevel = 2;
					IsEasy = false; // 난이도 이지 
					IsHard = true; // 난이도 하드
					
					break;
				default:
					// 잘못된 옵션 처리
					break;
				}
			}
		}

	}

	return selectedOption;
}

void introducePage() {

	int selectedOption = 1;
	// 특수키 0xe0 을 입력받으려면 unsigned char 로 선언해야 함
	unsigned char ch;
	bool StartPage = TRUE;
	char key;
	while (StartPage) {
	


			printf("                                         ★★★★★★★★★★★★★★ 게임 룰 설명 ★★★★★★★★★★★★★★★★\n");
			printf("                                         ★                                                                      ★\n");
			printf("                                         ★                                                                      ★\n");
			printf("                                         ★                                                                      ★\n");
			printf("                                         ★                                                                      ★\n");
			printf("                                         ★                                                                      ★\n");
			printf("                                         ★                                                                      ★\n");
			printf("                                         ★                                                                      ★\n");
			printf("                                         ★                                                                      ★\n");
			printf("                                         ★                                                                      ★\n");
			printf("                                         ★                                                                      ★\n");
			printf("                                         ★                                                                      ★\n");
			printf("                                         ★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★\n");


		



		Sleep(100);
		gotoxy(0, 0);
		if (kbhit()) {
			key = getch();

			if (key == ENTER) {
				StartPage = FALSE;
			}
		}

	}


	cls(WHITE, BLACK); // 기존 화면 지우기

}

void init_game() {
	//나중에 게임이 종료되고 게임이 다시 시작될 때
	//변수 값들을 초기화 해주는 영역
	
	system("cls");  //게임이 시작되면 초기화면을 사라지게함
	removeCursor();
}




	//게임 정보창

void info() {
		//textcolor(YELLOW2, BLACK);
		textcolor(WHITE, BLACK);

		//int minutes = remainingTime / 60;
		//int seconds = remainingTime % 60;

		//displayTime(minutes, seconds);



		gotoxy(122, 5); printf("점수: %d", score);
		gotoxy(122, 10);
		printf("목숨:");
		textcolor(RED1, BLACK);
		if (heart >= 4) {
			heart = 4;
			gotoxy(130, 10); printf("       ");

			gotoxy(130, 10); printf("♥♥♥♥");
		}
		else if (heart == 3) {
			gotoxy(130, 10); printf("       ");
			gotoxy(130, 10); printf("♥♥♥");
		}
		else if (heart == 2) {
			gotoxy(130, 10); printf("       ");
			gotoxy(130, 10); printf("♥♥");
		}
		else if (heart == 1) {
			gotoxy(130, 10); printf("       ");
			gotoxy(130, 10); printf("♥");
		}
		else {
			gotoxy(130, 10);
			printf("  ");
		}

		if (IsBoss) {
			gotoxy(2, 2);
			printf("보스 HP:");


			textcolor(RED1, BLACK);
			if (BossLife >= 40) {

				gotoxy(11, 2); printf("       ");

				gotoxy(11, 2); printf("♥♥♥♥");
			}
			else if (BossLife == 30) {
				gotoxy(11, 2); printf("       ");
				gotoxy(11, 2); printf("♥♥♥");
			}
			else if (BossLife == 20) {
				gotoxy(11, 2); printf("       ");
				gotoxy(11, 2); printf("♥♥");
			}
			else if (BossLife == 1) {
				gotoxy(11, 2); printf("       ");
				gotoxy(11, 2); printf("♥");
			}
			else {
				gotoxy(11, 2);
				printf("  ");
				//게임 종료창 띄우게?
			}


		}


		if (IsEasy) {
			textcolor(GREEN1, BLACK);
			gotoxy(122, 13); printf("난이도: 이지모드");
		}

		if (IsHard) {
			textcolor(RED1, BLACK);
			gotoxy(122, 13); printf("난이도: 하드모드");
		}

		if (IsLevel1) {
			textcolor(YELLOW1, BLACK);
			gotoxy(122, 15); printf("현재단계: 1단계");
		}

		if (IsLevel2) {
			textcolor(YELLOW1, BLACK);
			gotoxy(122, 15); printf("현재단계: 2단계");
		}
		if (IsLevel3) {
			textcolor(YELLOW1, BLACK);
			gotoxy(122, 15); printf("현재단계: 3단계");
		}

		if (IsBoss) {
			textcolor(YELLOW1, BLACK);
			gotoxy(122, 15); printf("현재단계: 보스 스테이지");
		}

	}



void player1(unsigned char ch) {

	int move_flag = 0; //플레이어의 움직임 여부를 나타내는 변수
	static unsigned char last_ch = 0;


	if (called == 0) {
		removeCursor();
		Zet1Draw(oldx, oldy);
		called = 1;
	}
	if (keep_moving && ch == 0)
		ch = last_ch;
	last_ch = ch;

	switch (ch) {
	case UP:
		if (oldy > 2)
			newy = oldy - 1;
		move_flag = 1;
		break;
	case DOWN:
		if (oldy < HEIGHT - 5)
			newy = oldy + 1;
		move_flag = 1;
		break;

	case LEFT:
		if (oldx > 2)
			newx = oldx - 1;
		move_flag = 1;
		break;
	case RIGHT:
		if (oldx < WIDTH - 40)
			newx = oldx + 1;
		move_flag = 1;
		break;

	}
	if (move_flag) {
		Zet1Erase(oldx, oldy);
		Zet1Draw(newx, newy); // 새로운 위치에서 플레이어 표시
		oldx = newx; // 마지막 위치를 기억한다.
		oldy = newy;

	}
}
//총알 그리기
void DrawBullet(int i) {
	textcolor(YELLOW1, BLACK);
	gotoxy(Bullet[i].x, Bullet[i].y); printf("@@");	
	//textcolor(RED1, BLACK); // 상대 적 비행기 색깔 
}

//총알 지우기
void EraseBullet(int i) {
	gotoxy(Bullet[i].x, Bullet[i].y); printf("    ");
	
}

//총알 움직임 
void MoveBullet3() {
	int i;

	for (i = 0; i < MAXBULLET; i++) {
		if (Bullet[i].exist == TRUE) {
			EraseBullet(i);
			if (Bullet[i].y <= 2) { // y 값이 3 이하일 때 지웁니다.
				Bullet[i].exist = FALSE;
			}
			else {
				Bullet[i].y--;
				DrawBullet(i);
			}
		}
	}

	Sleep(30);
}

bool CheckBulletBossCollision(int bulletIndex) {
	if (abs(Boss.y - Bullet[bulletIndex].y) <=8 && abs(Boss.x - Bullet[bulletIndex].x) <= 15) {
		gotoxy(Bullet[bulletIndex].x, Bullet[bulletIndex].y);
		printf("   ");
		return true;
	}
	return false;
}

void MoveBullet() {
	int i;

	for (i = 0; i < MAXBULLET; i++) {
		if (Bullet[i].exist == TRUE) {
			EraseBullet(i);
			Bullet[i].y--;
			if (Bullet[i].y <= 2) { // y 값이 3 이하일 때
				Bullet[i].y = -1; // 총알이 사라진 상태를 나타내기 위해 y 값을 -1로 설정
				Bullet[i].exist = FALSE;
			}
			else {
				DrawBullet(i);
				if (CheckBulletBossCollision(i)) {
					Bullet[i].exist = FALSE;
					score += 20;
					//Boss 그림 !!
					BossDraw2(Boss.x, Boss.y);
					BossLife--;
					info();
				}
			}
		}
	}

	Sleep(30);
}






// 적생성!!
void CreateBasicEnemy(int count) {
	int i, location, x, y;
	location = rand() % 2;
	x = 5 + rand() % 90;  //x 5 , y3
	if (IsLevel3) {
		y = 3 + rand() % 3;
	}
	else{ y = 2;}
	
	for (i = 0; i < MAXENEMY && Enemy[i].exist == TRUE; i++) {}
	if (i != MAXENEMY) {
		if (location == 1) {
			Enemy[i].x = x;
			Enemy[i].y = y;
			Enemy[i].move = 1;
		}
		else {
			Enemy[i].x = x;
			Enemy[i].y = y;
			Enemy[i].move = -1;
		}
		Enemy[i].type = rand() % count;
		Enemy[i].exist = TRUE;
	}
}

// 적생성!!
void CreateBasicEnemy2(int count) {
	int i, location, x, y;
	location = rand() % 2;
	x = 5 + rand() % 90;  //x 5 , y3
	
		y = 10 + rand() % 3;
	


	for (i = 0; i < MAXENEMY && Enemy[i].exist == TRUE; i++) {}
	if (i != MAXENEMY) {
		if (location == 1) {
			Enemy[i].x = x;
			Enemy[i].y = y;
			Enemy[i].move = 1;
		}
		else {
			Enemy[i].x = x;
			Enemy[i].y = y;
			Enemy[i].move = -1;
		}
		Enemy[i].type = rand() % count;
		Enemy[i].exist = TRUE;
	}
}

//적움직임 
void MoveEnemy() {
	int i;
	for (i = 0; i < MAXENEMY; i++) {
		if (Enemy[i].exist == TRUE) {
			if (Enemy[i].type == -1) {
				gotoxy(Enemy[i].x - 1, Enemy[i].y);
				printf("          ");
				Enemy[i].exist = FALSE;
				continue;
			}
			if (Enemy[i].x > WIDTH - 40) {
				Enemy[i].x -= 1;
				Enemy[i].move = -1;


			}
			else if (Enemy[i].x <= 3) {

				Enemy[i].move = 1;
				Enemy[i].x += 1;
				gotoxy(Enemy[i].x, Enemy[i].y);
				printf(" ");
			}
			else {
				Enemy[i].x += Enemy[i].move;
				gotoxy(Enemy[i].x, Enemy[i].y);
				textcolor(RED1, BLACK);
				printf(EnemyType[Enemy[i].type]);
				printf(" ");
			}
		}
	}
}

//basic이 쓰는 움직임 1단계!!
void MoveEnemy2() {
	int i;
	for (i = 0; i < MAXENEMY; i++) {
		if (Enemy[i].exist == TRUE) {
			if (Enemy[i].type == -1) {
				gotoxy(Enemy[i].x, Enemy[i].y);
				printf("          ");
				Enemy[i].exist = FALSE;
				continue;
			}

			gotoxy(Enemy[i].x, Enemy[i].y);
			printf("      "); // 현재 위치의 적 삭제

			Enemy[i].y += 1; // 아래로 이동

			if (Enemy[i].y >= HEIGHT - 4) {
				Enemy[i].exist = FALSE; // 화면을 벗어난 적 삭제
			}
			else {
				gotoxy(Enemy[i].x, Enemy[i].y);
				textcolor(RED1, BLACK);
				printf(EnemyType[Enemy[i].type]); // 새로운 위치에 적 그리기
			}
		}
	}
}

// 2단계 적이 쓰는 움직임 
void MoveEnemy3() {
	int i;
	for (i = 0; i < MAXENEMY; i++) {
		if (Enemy[i].exist == TRUE) {
			if (Enemy[i].type == -1) {
				gotoxy(Enemy[i].x, Enemy[i].y);
				printf("          ");
				Enemy[i].exist = FALSE;
				continue;
			}

			gotoxy(Enemy[i].x, Enemy[i].y);
			printf("      "); // 현재 위치의 적 삭제

			// 좌우로 움직이기 위한 랜덤값 생성
			int moveDirection = rand() % 3; // 0, 1, 2 중 하나의 값을 랜덤으로 생성

			switch (moveDirection) {
			case 0:
				Enemy[i].x -= 1; // 왼쪽으로 이동
				break;
			case 1:
				Enemy[i].x += 1; // 오른쪽으로 이동
				break;
			default:
				// 움직이지 않음
				break;
			}

			Enemy[i].y += 1; // 아래로 이동

			if (Enemy[i].y >= HEIGHT - 4) {
				Enemy[i].exist = FALSE; // 화면을 벗어난 적 삭제
			}
			else {
				gotoxy(Enemy[i].x, Enemy[i].y);
				textcolor(RED1, BLACK);
				printf(EnemyType[Enemy[i].type]); // 새로운 위치에 적 그리기
			}
		}
	}
}

void MoveBoss() {
	if (Boss.exist == TRUE) {
		int randomMove = rand() % 3; // 0, 1, 2 중에서 랜덤으로 선택
		switch (randomMove) {
		case 0:
			Boss.move = -1; // 왼쪽으로 이동
			break;
		case 1:
			Boss.move = 0; // 가만히 있음
			break;
		case 2:
			Boss.move = 1; // 오른쪽으로 이동
			break;
		}

		Boss.x += Boss.move;

		// 보스가 왼쪽 벽에 닿았을 때
		if (Boss.x <= 3) {
			Boss.x = 3;
		}

		// 보스가 오른쪽 벽에 닿았을 때
		if (Boss.x >= WIDTH - 40) {
			Boss.x = WIDTH - 40;
		}

		gotoxy(Boss.x - Boss.move, Boss.y);
		printf("            "); // 이전 위치 지우기

		gotoxy(Boss.x, Boss.y);
		textcolor(RED1, BLACK);
		BossDraw(Boss.x, Boss.y); // 새로운 위치에 보스 그리기
	}
}

//적총알
void EnemyBulletshow() {
	if (PickMyLevel == 1) {
		textcolor(GREEN1, BLACK);
		gotoxy(0, 0); printf("■");
	}
	if (PickMyLevel == 2) {
		textcolor(RED1, BLACK);
		gotoxy(0, 0); printf("■");
	}
	
	int j;
	int random = rand() % MAXENEMY;
	for (j = 0; j < MAXENEMYBULLET && EnemyBullet[j].exist == TRUE; j++) {}
	if (j != MAXENEMYBULLET && Enemy[random].exist == TRUE) {
		EnemyBullet[j].x = Enemy[random].x + 2;
		EnemyBullet[j].y = Enemy[random].y + 1;
		EnemyBullet[j].exist = TRUE;
	}
}
void BossBulletshow() {
	if (PickMyLevel == 1) {
		textcolor(GREEN1, BLACK);
		gotoxy(0, 0); printf("■");
	}
	if (PickMyLevel == 2) {
		textcolor(RED1, BLACK);
		gotoxy(0, 0); printf("■");
	}

	int j;
	int random = rand() % MAXENEMY;
	for (j = 0; j < MAXENEMYBULLET && EnemyBullet[j].exist == TRUE; j++) {}
	if (j != MAXENEMYBULLET) {
		EnemyBullet[j].x = Boss.x + 6;
		EnemyBullet[j].y = Boss.y + 7;
		EnemyBullet[j].exist = TRUE;
	}
}


//총알 그리기
void EnemyBulletdraw(int i) {
	textcolor(RED2, BLACK);
	gotoxy(EnemyBullet[i].x, EnemyBullet[i].y);
	printf("◈");
}

// 적총알 지우기
void EnemyBulleterase(int i) {
	gotoxy(EnemyBullet[i].x, EnemyBullet[i].y);
	printf("  ");
}

// 적총알 움직임
void EnemyBulletMove() {
	for (int i = 0; i < MAXENEMYBULLET; i++) {
		if (EnemyBullet[i].exist == TRUE) {
			EnemyBulleterase(i);
			if (EnemyBullet[i].y >= HEIGHT - 4) {
				EnemyBullet[i].exist = FALSE;
			}
			else {
				EnemyBullet[i].y++;
				EnemyBulletdraw(i);
			}
		}
	}

	// 적 총알이 화면 밖으로 나가면 지우기
	for (int i = 0; i < MAXENEMYBULLET; i++) {
		if (EnemyBullet[i].exist == FALSE) {
			EnemyBulleterase(i);
		}
	}


}




// 내가 적 총알에 맞았을 때
void playerTouch() {
	int i;
	for (i = 0; i < MAXENEMYBULLET; i++) {
		if (EnemyBullet[i].exist == FALSE)
			continue;
		if (EnemyBullet[i].y == newy && abs(EnemyBullet[i].x - newx) <= 3) {
			EnemyBullet[i].exist = FALSE;
			gotoxy(EnemyBullet[i].x, EnemyBullet[i].y);
			printf("    ");
			heart--;
			Zet10(newx, newy);
			Sleep(20);

			
			info();
		}
	}
	

}

// 적과 내가 만났을 때!
void EnemyTouch() {
	int i;
	for (i = 0; i < MAXENEMY; i++) {
		if (Enemy[i].exist == FALSE)
			continue;
		if (Enemy[i].y == newy && abs(Enemy[i].x - newx) <= 5) {
			Enemy[i].exist = FALSE;
			gotoxy(Enemy[i].x, Enemy[i].y);
			printf("      ");
			Zet10(newx, newy);
			Sleep(20);

			heart--;
			info();
		}
	}

}



// 적이 내 총알에 맞았을 때 상황 구현하기
void DeleteEnemy() {
	int i;
	for (i = 0; i < MAXENEMY; i++) {
		if (Enemy[i].exist == FALSE || Enemy[i].type == -1)
			continue;
		if (Enemy[i].y == Bullet[i].y && abs(Enemy[i].x - Bullet[i].x) <= 15) {
			gotoxy(Bullet[i].x, Bullet[i].y);
			printf("   ");
			Bullet[i].exist = FALSE;
			Enemy[i].type = -1;
			gotoxy(Enemy[i].x+2, Bullet[i].y);
			textcolor(BLUE2, BLACK);
			printf("★+20");
			Sleep(20);
			printf("  ");
			score += 20;
			info();
			break;
		}
	}
	
}

// 보스가 내 총알에 맞았을 때 상황 구현하기
void DeleteBoss() {
	int i;

	for (i = 0; i < MAXENEMY; i++) {
		if (Enemy[i].exist == FALSE || Enemy[i].type == -1)
			continue;
		if (abs(Boss.y - Bullet[i].y) <= 17 && abs(Boss.x - Bullet[i].x) <= 17) {
			gotoxy(Bullet[i].x, Bullet[i].y);
			printf("   ");
			Bullet[i].exist = FALSE;
			score += 20;
			BossLife--;
			info();
			break;
		}
	}
}


//게임화면에서 맵 만들기!!!
void GameMap() {
	int map[33][60];
	int x, y;

	// 맵 초기화
	for (y = 0; y < 33; y++) {
		for (x = 0; x < 60; x++) {
			if (y == 0 || y == 32 || x == 0 || x == 59) {
				map[y][x] = 1;  // 테두리
			}
			else {
				map[y][x] = 0;  // 내부
			}
		}
	}

	// 맵 출력  // 난이도에 따라 색깔을 바꾸자!!!!@@@@@@@@@@@@@@@@@

	
	if (PickMyLevel == 1) {
		textcolor(GREEN1, BLACK);
	}
	//난이도가 hard 라면
	if (PickMyLevel == 2) {
		textcolor(RED1, BLACK);
	}
	for (y = 0; y < 33; y++) {
		for (x = 0; x < 60; x++) {
			if (map[y][x] == 1) {
				printf("■");
			}
			else {
				printf("  ");
			}
		}
		printf("\n");
	}

}







// 우선 게임을 진행이 되면 우리는 계속 키보드를 활용할 것이니 while문을 통해 구현해보자
//게임 LEVEL 1 단계 화면 
void Level1gamestart() {
	unsigned char ch;
	int i;
	
	//난이도가 easy라면
	if (PickMyLevel == 1) {
		init_game(); // 게임 설정 초기화 해주기
	}
	//난이도가 hard 라면
	if (PickMyLevel == 2) {
		init_game(); // 게임 설정 초기화 해주기
	}
	
	GameMap(); // 게임 맵 그려주기!!! easy는 흰색 HARD는 빨간색 
	Zet1Draw(oldx, oldy); // 처음에 비행기 그려주기!!  (내가 선택한 비행기가 그려진다.)
	info(); // 내 현재 게임 진행상황 !! (while문안에 넣으면 끊겨보이기 때문에 변경사항이 있을 때만 호출하게 해놓으면서 끊김 현상을 멈추었다.)
	// 적 관련 변수 선언
	int enemySpawnTimer = 0;
	int enemySpawnInterval = 10;  // 적 생성 간격 조절  2단계 난이도 조절해주기
		while (1) {
			if (kbhit() == 1) {
				ch = getch();
				if (ch == SPECIAL1 || ch == SPECIAL2) {
					ch = getch();
					switch (ch) {
					case UP: case DOWN: case LEFT: case RIGHT:
						player1(ch);
						if (frame_count % p1_frame_sync == 0)
							player1(0);
						break;
					default:
						if (frame_count % p1_frame_sync == 0)
							player1(0);
					}
				}
				if (ch == SPACE) {
					for (i = 0; i < MAXBULLET && Bullet[i].exist == TRUE; i++) {
						//DrawBullet(i); // 수정: 총알을 화면에 그림
					}
					if (i != MAXBULLET) {
						Bullet[i].x = newx + 4;
						Bullet[i].y = newy - 1;
						Bullet[i].exist = TRUE;
					}
				}
			}
			else {
				//플레이어 무브 속도조절 함수
			}

		// 점수가 200 보다 크면 다음 단계로 넘어가기 위해서 막아놓기
			if (score < 100) {
				//총알 속도조절 함수
				MoveBullet();


				// 적 생성
				if (enemySpawnTimer >= enemySpawnInterval) {
					CreateBasicEnemy(3);  // 1단계니까 적 종류 다양하게 주지 않는다. 
					enemySpawnTimer = 0;
				}
				else {
					enemySpawnTimer++;
				}

				// 적 움직임 처리
				MoveEnemy2();



				// 적 총알 생성 및 움직임 처리
				//EnemyBulletMove();
				//EnemyBulletshow();

				// 적 총알에 맞았을 때 처리
				//playerTouch();


				DeleteEnemy();// 적이 내 총알에 맞아 죽는 것을 처리
				EnemyTouch(); // 적과 내가 만났을 때  생명력 -1 
			}
			
			if (score >= 100) {

				Level1ClearMessage(60,10);
				break;
			}
		
			
			// 게임 정보 표시
			


			Sleep(Delay); // Delay 값을 줄이고
			frame_count++;// frame_count 값으로 속도 조절을 한다.
		}
}


// 우선 게임을 진행이 되면 우리는 계속 키보드를 활용할 것이니 while문을 통해 구현해보자
//게임 LEVEL 2 단계 화면 
void Level2gamestart() {
	cls(WHITE, BLACK);
	unsigned char ch;
	int i;

	//난이도가 easy라면
	if (PickMyLevel == 1) {
		init_game(); // 게임 설정 초기화 해주기
	}
	//난이도가 hard 라면
	if (PickMyLevel == 2) {
		init_game(); // 게임 설정 초기화 해주기
	}

	GameMap(); // 게임 맵 그려주기!!! easy는 흰색 HARD는 빨간색 
	Zet1Draw(oldx, oldy); // 처음에 비행기 그려주기!!  (내가 선택한 비행기가 그려진다.)
	info(); // 내 현재 게임 진행상황 !! (while문안에 넣으면 끊겨보이기 때문에 변경사항이 있을 때만 호출하게 해놓으면서 끊김 현상을 멈추었다.)
	// 적 관련 변수 선언
	int enemySpawnTimer = 0;
	int enemySpawnInterval = 5;  // 적 생성 간격 조절
	while (1) {
		if (kbhit() == 1) {
			ch = getch();
			if (ch == SPECIAL1 || ch == SPECIAL2) {
				ch = getch();
				switch (ch) {
				case UP: case DOWN: case LEFT: case RIGHT:
					player1(ch);
					if (frame_count % p1_frame_sync == 0)
						player1(0);
					break;
				default:
					if (frame_count % p1_frame_sync == 0)
						player1(0);
				}
			}
			if (ch == SPACE) {
				for (i = 0; i < MAXBULLET && Bullet[i].exist == TRUE; i++) {
					//DrawBullet(i); // 수정: 총알을 화면에 그림
				}
				if (i != MAXBULLET) {
					Bullet[i].x = newx + 4;
					Bullet[i].y = newy - 1;
					Bullet[i].exist = TRUE;
				}
			}
		}
		else {
			//플레이어 무브 속도조절 함수
		}

		// 점수가 200 보다 크면 다음 단계로 넘어가기 위해서 막아놓기
		if (score < 200) {
			//총알 속도조절 함수
			MoveBullet();


			// 적 생성
			if (enemySpawnTimer >= enemySpawnInterval) {
				CreateBasicEnemy(7); // 2단계니까 적 종류 다양하게 해주기 
				enemySpawnTimer = 0;
			}
			else {
				enemySpawnTimer++;
			}

			// 적 움직임 2단계로 처리 하자!
			MoveEnemy3();



			// 적 총알 생성 및 움직임 처리
			//EnemyBulletMove();
			//EnemyBulletshow();

			// 적 총알에 맞았을 때 처리
			//playerTouch();


			DeleteEnemy();// 적이 내 총알에 맞아 죽는 것을 처리
			EnemyTouch(); // 적과 내가 만났을 때  생명력 -1 
		}

		// 3단계로 넘어가기 
		if (score >= 200) {

			Level2ClearMessage(60, 10); // 레벨 2단계 클리어 메시지 남기기
			break;
		}


		// 게임 정보 표시



		Sleep(Delay); // Delay 값을 줄이고
		frame_count++;// frame_count 값으로 속도 조절을 한다.
	}
}


// 우선 게임을 진행이 되면 우리는 계속 키보드를 활용할 것이니 while문을 통해 구현해보자
//게임 LEVEL 3 단계 화면 
void Level3gamestart() {
	cls(WHITE, BLACK);
	unsigned char ch;
	int i;

	//난이도가 easy라면
	if (PickMyLevel == 1) {
		init_game(); // 게임 설정 초기화 해주기
	}
	//난이도가 hard 라면
	if (PickMyLevel == 2) {
		init_game(); // 게임 설정 초기화 해주기
	}

	GameMap(); // 게임 맵 그려주기!!! easy는 흰색 HARD는 빨간색 
	Zet1Draw(oldx, oldy); // 처음에 비행기 그려주기!!  (내가 선택한 비행기가 그려진다.)
	info(); // 내 현재 게임 진행상황 !! (while문안에 넣으면 끊겨보이기 때문에 변경사항이 있을 때만 호출하게 해놓으면서 끊김 현상을 멈추었다.)
	// 적 관련 변수 선언
	int enemySpawnTimer = 0;
	int enemySpawnInterval = 5;  // 적 생성 간격 조절
	while (1) {
		if (kbhit() == 1) {
			ch = getch();
			if (ch == SPECIAL1 || ch == SPECIAL2) {
				ch = getch();
				switch (ch) {
				case UP: case DOWN: case LEFT: case RIGHT:
					player1(ch);
					if (frame_count % p1_frame_sync == 0)
						player1(0);
					break;
				default:
					if (frame_count % p1_frame_sync == 0)
						player1(0);
				}
			}
			if (ch == SPACE) {
				for (i = 0; i < MAXBULLET && Bullet[i].exist == TRUE; i++) {
					//DrawBullet(i); // 수정: 총알을 화면에 그림
				}
				if (i != MAXBULLET) {
					Bullet[i].x = newx + 4;
					Bullet[i].y = newy - 1;
					Bullet[i].exist = TRUE;
				}
			}
		}
		else {
			//플레이어 무브 속도조절 함수
		}

		// 점수가 200 보다 크면 다음 단계로 넘어가기 위해서 막아놓기
		if (score < 360) {
			//총알 속도조절 함수
			MoveBullet();


			// 적 생성
			if (enemySpawnTimer >= enemySpawnInterval) {
				CreateBasicEnemy(10); // 3단계니까 적 종류 다양하게 해주기 
				enemySpawnTimer = 0;
			}
			else {
				enemySpawnTimer++;
			}

			// 적 움직임 3단계로 처리 하자!
			MoveEnemy();



			// 적 총알 생성 및 움직임 처리
			EnemyBulletMove();
			EnemyBulletshow();

			// 적 총알에 맞았을 때 처리
			playerTouch();


			DeleteEnemy();// 적이 내 총알에 맞아 죽는 것을 처리
			EnemyTouch(); // 적과 내가 만났을 때  생명력 -1 
		}

		// 3단계로 넘어가기 
		if (score >= 360) {

			Level3ClearMessage(60, 10); // 레벨 2단계 클리어 메시지 남기기
			break;
		}


		// 게임 정보 표시



		Sleep(Delay); // Delay 값을 줄이고
		frame_count++;// frame_count 값으로 속도 조절을 한다.
	}
}


// 우선 게임을 진행이 되면 우리는 계속 키보드를 활용할 것이니 while문을 통해 구현해보자
//게임 보스 단계 화면 
void Bossgamestart() {
	cls(WHITE, BLACK);
	textcolor(GREEN1, BLACK);
	Boss.x = 60;
	Boss.y = 3;
	gotoxy(60, 17);

	printf("보스가 곧 출현해요!!!!!!!!!!!!!!!");
	
	Sleep(1000);
	cls(WHITE, BLACK);

	unsigned char ch;
	int i;
	int startTime = time(NULL); // 게임 시작 시간 (초)
	//난이도가 easy라면
	if (PickMyLevel == 1) {
		init_game(); // 게임 설정 초기화 해주기
	}
	//난이도가 hard 라면
	if (PickMyLevel == 2) {
		init_game(); // 게임 설정 초기화 해주기
	}

	GameMap(); // 게임 맵 그려주기!!! easy는 흰색 HARD는 빨간색 
	Zet1Draw(oldx, oldy); // 처음에 비행기 그려주기!!  (내가 선택한 비행기가 그려진다.)
	//BossDraw(60,11);
	info(); // 내 현재 게임 진행상황 !! (while문안에 넣으면 끊겨보이기 때문에 변경사항이 있을 때만 호출하게 해놓으면서 끊김 현상을 멈추었다.)
	// 적 관련 변수 선언
	int enemySpawnTimer = 0;
	int enemySpawnInterval = 5;  // 적 생성 간격 조절
	while (1) {
		if (kbhit() == 1) {
			ch = getch();
			if (ch == SPECIAL1 || ch == SPECIAL2) {
				ch = getch();
				switch (ch) {
				case UP: case DOWN: case LEFT: case RIGHT:
					player1(ch);
					if (frame_count % p1_frame_sync == 0)
						player1(0);
					break;
				default:
					if (frame_count % p1_frame_sync == 0)
						player1(0);
				}
			}
			if (ch == SPACE) {
				for (i = 0; i < MAXBULLET && Bullet[i].exist == TRUE; i++) {
					//DrawBullet(i); // 수정: 총알을 화면에 그림
				}
				if (i != MAXBULLET) {
					Bullet[i].x = newx + 4;
					Bullet[i].y = newy - 1;
					Bullet[i].exist = TRUE;
				}
			}
		}
		else {
			//플레이어 무브 속도조절 함수
		}

		// 점수가 200 보다 크면 다음 단계로 넘어가기 위해서 막아놓기
		if (BossLife != 0 ) {
			//총알 속도조절 함수
			MoveBullet();
			//MoveBoss();

			// 보스 움직임
			if (frame_count % 10 == 0) {
				int moveDirection = rand() % 2; // 0 또는 1 중 하나의 값을 랜덤으로 생성

				switch (moveDirection) {
				case 0:
					BossErase(Boss.x, Boss.y); // 이전 위치 지우기
					Boss.x--; // 왼쪽으로 이동
					BossDraw(Boss.x, Boss.y); // 새로운 위치에 보스 그리기
					break;
				case 1:
					BossErase(Boss.x, Boss.y); // 이전 위치 지우기
					Boss.x++; // 오른쪽으로 이동
					BossDraw(Boss.x, Boss.y); // 새로운 위치에 보스 그리기
					break;
				}
			}
			
			//DeleteBoss();
			//// 적 생성
			if (enemySpawnTimer >= enemySpawnInterval) {
				CreateBasicEnemy2(10); // 3단계니까 적 종류 다양하게 해주기 
				enemySpawnTimer = 0;
			}
			else {
				enemySpawnTimer++;
			}

			//// 적 움직임 3단계로 처리 하자!
			MoveEnemy();

			//

			//// 적 총알 생성 및 움직임 처리
			

			EnemyBulletshow(); // 적 총알 좌표 설정

			EnemyBulletMove(); // 적 총알 그리고 지우기 

			BossBulletshow(); // 보스 총알 좌표 설정

			//BossBulletMove(); // 보스 총알 그리고 지우기 
			//// 적 총알에 맞았을 때 처리
			playerTouch();


			DeleteEnemy();// 적이 내 총알에 맞아 죽는 것을 처리
			EnemyTouch(); // 적과 내가 만났을 때  생명력 -1 
		}

		// 보스가 피가 없다면
		if (BossLife <= 0) {

			BossClearMessage(60, 10); // 보스단계 클리어 메시지!!
			break;
		}

		int currentTime = time(NULL); // 현재 시간
		int elapsedSeconds = currentTime - startTime; // 경과 시간 (초)
		int remainingSeconds = remainingTime - elapsedSeconds; // 남은 시간 (초)
		int minutes = remainingSeconds / 60; // 분
		int seconds = remainingSeconds % 60; // 초

		displayTime(minutes, seconds);

		// 제한시간이 지나면 
		if (remainingSeconds <= 0) {
			BossClearMessage(60, 10); // 보스단계 클리어 메시지!!
			break;
		}

		// 게임 정보 표시

	
		Sleep(Delay); // Delay 값을 줄이고
		frame_count++;// frame_count 값으로 속도 조절을 한다.
		
	}
}











void main()
{
	// 난이도 창 + 비행기 설정 창 만들어야한다. 하지만 일단 게임 화면부터 먼저 만들자!!
	srand(time(NULL)); 
	//cls(BLACK, WHITE);
	cls(WHITE, BLACK);
	centerConsoleWindow();
	system("mode con cols=150 lines=34 | title 1891093 전영식 슈팅게임"); // 콘솔창 크기 및 제목 설정
	int i, x, y;
	char buf[100];
	system("cls");   //화면 지우기
	removeCursor(); // 커서를 안보이게 한다
	int selectedOption = 1;
	// 특수키 0xe0 을 입력받으려면 unsigned char 로 선언해야 함
	unsigned char ch;
	char key;
	bool isFinish = false;
	bool isGameRunning = false;  // 게임 실행 여부를 나타내는 부울 변수
	IntroPage();

	
		
	while (isFinish == false) {
		if (isGameRunning) {// 게임 실행 중인 경우
			
		
			//pickMyJet(); // 비행기를 고른다!!
			//
			//pickGameLevel();// 난이도 조절 화면 
			//LoadingPage();// 로딩 화면 !!

			//IsLevel1 = true; // 1단계
			//Level1gamestart(); // 1단계 게임 시작!!
			//IsLevel1 = false; // 1단계

			//Level2LoadingPage();	// 2단계 로딩 페이지 
			//IsLevel2 = true; // 2단계
			//Level2gamestart();      // 2단계 게임 시작
			//IsLevel2 = false; // 2단계
			//Level3LoadingPage();	// 3단계 로딩 페이지 
			//IsLevel3 = true; // 3단계
			//Level3gamestart();      // 3단계 게임 시작
			//IsLevel3 = false; // 3단계
			//BossLoadingPage(); // 보스로 가는 로딩 페이지
			IsBoss = true;
			cls(WHITE, BLACK);
			Bossgamestart(); // 보스 단계 게임 시작!!
			IsBoss = false;
			IntroPage();
			if (FinishPage() == 2) {
				//isGameRunning = false;
				isFinish = TRUE;
			};
			isGameRunning = false;
			//클리어 창!! 다시할 지 말지 정하는 창 


		
		
			
		}
		else {
			// 게임 실행 중이 아닌 경우
			
			// 메뉴 선택에 따라 다른 화면으로 이동하도록 처리
			switch (MenuPage()) {
			case 1:
				isGameRunning = true;  // 첫 번째 게임 화면으로 이동
				break;
			case 2:
				//@@@@@ 게임에 대한 설명을 보여주는 화면 @@@@ 
			
				introducePage();
				break;
			default:
				isFinish = true;
				break;
			}
		}
	}

	
	




}

