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


#define UX 72  //시작 유저의 x좌표
#define UY 30  //시작 유저의 y좌표

static int called = 0;

static int oldx = UX, oldy = UY; // 플레이어의 old 좌표
static int newx = UX, newy = UY; //플레이어의 new 좌표
static int keep_moving = 1;  //1:계속이동

#define MAXBULLET 100//플레이어의 최대 총알 수
#define TRUE 1
#define FALSE 0

struct {
	int exist;
	int x, y;
}Bullet[MAXBULLET];

#define SPACE 0x20

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

void centerConsoleWindow()
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

void IntroPage() {
	int i;
	while (1) {


		for (i = 0; i <= 20; i++) {
			Sleep(20);
			//draw_box2(i * 2, i, 78 - i * 2, 22 - i, "□");
			draw_box2(i * 2, i, WIDTH  - i * 2, HEIGHT - 2 - i, "★");
		}

		for (i = 20; i >= 0; i--) {
			Sleep(20);
			draw_box2(i * 2, i, WIDTH  - i * 2, HEIGHT - 2 - i, "  ");
		}
		cls(WHITE, BLACK);
		break;
	}
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
		if (_kbhit()) {
			key = _getch();

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
		if (_kbhit()) {
			key = _getch();

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

void playerdraw(int x, int y) {
	textcolor(GREEN1, BLACK);
	gotoxy(x, y);
	printf("<=★=>");
}

void playererase(int x, int y) {
	gotoxy(x, y);
	printf("      ");
}


void player1(unsigned char ch) {

	int move_flag = 0; //플레이어의 움직임 여부를 나타내는 변수
	static unsigned char last_ch = 0;


	if (called == 0) {
		removeCursor();
		playerdraw(oldx, oldy);
		called = 1;
	}
	if (keep_moving && ch == 0)
		ch = last_ch;
	last_ch = ch;

	switch (ch) {
	case UP:
		if (oldy > 0)
			newy = oldy - 1;
		move_flag = 1;
		break;
	case DOWN:
		if (oldy < HEIGHT - 3)
			newy = oldy + 1;
		move_flag = 1;
		break;

	case LEFT:
		if (oldx > 2)
			newx = oldx - 1;
		move_flag = 1;
		break;
	case RIGHT:
		if (oldx < WIDTH - 6)
			newx = oldx + 1;
		move_flag = 1;
		break;

	}
	if (move_flag) {
		playererase(oldx, oldy);
		playerdraw(newx, newy); // 새로운 위치에서 플레이어 표시
		oldx = newx; // 마지막 위치를 기억한다.
		oldy = newy;

	}
}
//총알 그리기
void DrawBullet(int i) {
	textcolor(GREEN1, BLACK);
	gotoxy(Bullet[i].x, Bullet[i].y); printf("│");
	

	
}
//총알 지우기
void EraseBullet(int i) {
	gotoxy(Bullet[i].x, Bullet[i].y); printf("  ");
	
}

//총알 움직임
void bulletmove() {
	int i;

	for (i = 0; i < MAXBULLET; i++) {
		if (Bullet[i].exist == TRUE) {
			EraseBullet(i);
			if (Bullet[i].y == 0) {
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

// 우선 게임을 진행이 되면 우리는 계속 키보드를 활용할 것이니 while문을 통해 구현해보자

void gamestart() {
	unsigned char ch;
	int i;
	init_game();
	playerdraw(oldx, oldy);
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
						Bullet[i].x = newx + 1;
						Bullet[i].y = newy - 1;
						Bullet[i].exist = TRUE;
					}
				}
			}
			else {
				//플레이어 무브 속도조절 함수
			}
			//총알 속도조절 함수
			bulletmove();
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
		if (isGameRunning) {
			// 게임 실행 중인 경우
			
			gamestart();
		
		
			
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

