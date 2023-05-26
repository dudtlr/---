// ���α׷��ַ� 10��
// Lab10-1 gotoxy �� �̿��� ȭ�� ������
// 
#include <stdio.h>
#include <conio.h> // Ű���� �Է� ���� �� ���
#include <Windows.h>
#include <stdlib.h> // rand �Լ��� ����ϱ� ���� �߰�
#include<time.h> 
#include<stdbool.h>     // ���� �ڷḦ ���� ���� ���̺귯��

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

#define WIDTH 148
#define HEIGHT 34

#define ESC 0x1b //  ESC ������ ����

#define SPECIAL1 0xe0 // Ư��Ű�� 0xe0 + key ������ �����ȴ�.
#define SPECIAL2 0x00 // keypad ��� 0x00 + key �� �����ȴ�.

#define UP  0x48 // Up key�� 0xe0 + 0x48 �ΰ��� ���� ���´�.
#define DOWN 0x50
#define LEFT 0x4b
#define RIGHT 0x4d
#define ENTER '\r'
#define SPACE 0x20

int Delay = 10;
int frame_count = 0; // game ���� frame count �� �ӵ� ���������� ���ȴ�.
int p1_frame_sync = 4; //�÷��̾��� �̵��ӵ�


#define UX 72  //���� ������ x��ǥ
#define UY 30  //���� ������ y��ǥ

static int called = 0;

static int oldx = UX, oldy = UY; // �÷��̾��� old ��ǥ
static int newx = UX, newy = UY; //�÷��̾��� new ��ǥ
static int keep_moving = 1;  //1:����̵�

#define MAXBULLET 100//�÷��̾��� �ִ� �Ѿ� ��
#define TRUE 1
#define FALSE 0

struct {
	int exist;
	int x, y;
}Bullet[MAXBULLET];

#define SPACE 0x20

void gotoxy(int x, int y) //���� ���ϴ� ��ġ�� Ŀ�� �̵�
{
	COORD pos; // Windows.h �� ����
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos); //Windows.h �� ���� �Ǿ��ִ�.
}

void textcolor(int fg_color, int bg_color)
{
	SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), fg_color | bg_color<<4);
}

void removeCursor(void){ // Ŀ���� �Ⱥ��̰� �Ѵ�
	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible=0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}
void showCursor(void){ // Ŀ���� ���̰� �Ѵ�
	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible=1;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}
void cls(int text_color, int bg_color) // ȭ�� �����
{
	char cmd[100];
	system("cls");
	sprintf(cmd, "COLOR %x%x", bg_color, text_color);
	system(cmd);
}

// box �׸��� �Լ�, ch ���ڷ� (x1,y1) ~ (x2,y2) box�� �׸���.
void draw_box(int x1, int y1, int x2, int y2, char ch)
{
	int x, y;
	//�ϼ��Ұ�

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
// box �׸��� �Լ�, ch ���ڿ��� (x1,y1) ~ (x2,y2) box�� �׸���.
// �ѱ� ���ڸ� �׸��� �뵵�� ��� (��, "��" �� �� ���� �׸��� ���)
void draw_box2(int x1, int y1, int x2, int y2, char* ch)    //x1,y1 0.0 �̰� x2,y2�� ������ �ϴ� �ǳ�
{
	int x, y;
	// �ϼ��� �� ������ �� ��
	int fg_color, bg_color; // ���� ���� �߰�


	// �ڽ��� 1ĭ�� �ƴ϶� 2ĭ�� �����Ѵ� �׷��� ��ǥ�� +1 �� �ƴ϶� +2 ����� �Ѵ�!!!!
	// Ư�����ڴ� 2ĭ�̶� x��ǥ 2ĭ�����Ѵ�!!! y��ǥ�� 1ĭ�̶� �Ű� �Ƚᵵ �ȴ�. 
	for (x = x1; x <= x2; x += 2) {
		//Sleep(1);
		fg_color = rand() % 14+1; // ������ ���� ����
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
	int fg_color = rand() % 14 + 1; // ������ ���� ����
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
			//draw_box2(i * 2, i, 78 - i * 2, 22 - i, "��");
			draw_box2(i * 2, i, WIDTH  - i * 2, HEIGHT - 2 - i, "��");
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
	// Ư��Ű 0xe0 �� �Է¹������� unsigned char �� �����ؾ� ��
	unsigned char ch;
	bool StartPage = TRUE;
	char key;
	while (StartPage) {
		changeTextColor();
		printf("\n");
		printf("           �ڡڡڡڡ�      ��        ��    �ڡڡڡڡڡ�    �ڡڡڡڡڡ�    �ڡڡڡڡڡ�    �ڡڡڡڡڡ�    �ڡ�         ��     �ڡڡڡڡڡ�          \n");
		printf("         ��                ��        ��    ��        ��    ��        ��         ��              ��         �� ��        ��   ��                      \n");
		printf("         ��                ��        ��    ��        ��    ��        ��         ��              ��         ��   ��      ��   ��                      \n");
		printf("           �ڡڡڡڡ�      �ڡڡڡڡڡ�    ��        ��    ��        ��         ��              ��         ��     ��    ��   ��      �ڡڡ�          \n");
		changeTextColor();
		printf("                     ��    ��        ��    ��        ��    ��        ��         ��              ��         ��       ��  ��   ��       ��             \n");
		printf("                     ��    ��        ��    ��        ��    ��        ��         ��              ��         ��         �ڡ�   ��       ��             \n");
		printf("           �ڡڡڡڡ�      ��        ��    �ڡڡڡڡڡ�    �ڡڡڡڡڡ�         ��         �ڡڡڡڡڡ�    ��           ��     �ڡڡڡڡڡ�          \n");
		printf("\n");
		changeTextColor();
		printf("                                           �ڡڡڡڡڡ�          ��        ��             ��    �ڡڡڡڡڡ�            \n");
		printf("                                         ��                     �ڡ�       �ڡ�         �ڡ�    ��                      \n");
		printf("                                         ��                    ��  ��      �� ��       �� ��	��			            \n");
		printf("                                         ��     �ڡڡ�        �ڡڡڡ�     ��  ��     ��  ��    �ڡڡڡڡڡ�            \n");
		changeTextColor();
		printf("                                         ��       ��         ��      ��	   ��	��   ��   ��    ��				        \n");
		printf("                                         ��       ��        ��        ��   �� 	 �� ��    ��	��			            \n");
		printf("                                           �ڡڡڡڡڡ�    ��          ��  ��	  ��      ��    �ڡڡڡڡڡ�			\n");
		textcolor(11, 0);
		printf("\n");
		printf("\n");
		printf("                                                                                       made by 1891093 ������			\n");
		textcolor(15, 0);
		printf("\n");

		if (selectedOption == 1) {
			printf("                                         �ڡڡڡڡڡڡڡڡڡڡڡڡڡ�   MENU   �ڡڡڡڡڡڡڡڡڡڡڡڡڡڡ�\n");
			printf("                                         ��                                                                ��\n");
			printf("                                         ��                                                                ��\n");
			printf("                                         ��                    ->   1.���� ����                            ��\n");
			printf("                                         ��                                                                ��\n");
			printf("                                         ��                                                                ��\n");
			printf("                                         ��                         2.���� ����                            ��\n");
			printf("                                         ��                                                                ��\n");
			printf("                                         ��                                                                ��\n");
			printf("                                         ��                         3.���� ����                            ��\n");
			printf("                                         ��                                                                ��\n");
			printf("                                         ��                                                                ��\n");
			printf("                                         �ڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡ�\n");
		}

		else if (selectedOption == 2) {
			printf("                                         �ڡڡڡڡڡڡڡڡڡڡڡڡڡ�   MENU   �ڡڡڡڡڡڡڡڡڡڡڡڡڡڡ�\n");
			printf("                                         ��                                                                ��\n");
			printf("                                         ��                                                                ��\n");
			printf("                                         ��                         1.���� ����                            ��\n");
			printf("                                         ��                                                                ��\n");
			printf("                                         ��                                                                ��\n");
			printf("                                         ��                    ->   2.���� ����                            ��\n");
			printf("                                         ��                                                                ��\n");
			printf("                                         ��                                                                ��\n");
			printf("                                         ��                         3.���� ����                            ��\n");
			printf("                                         ��                                                                ��\n");
			printf("                                         ��                                                                ��\n");
			printf("                                         �ڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡ�\n");



		}
		else {
			printf("                                         �ڡڡڡڡڡڡڡڡڡڡڡڡڡ�   MENU   �ڡڡڡڡڡڡڡڡڡڡڡڡڡڡ�\n");
			printf("                                         ��                                                                ��\n");
			printf("                                         ��                                                                ��\n");
			printf("                                         ��                         1.���� ����                            ��\n");
			printf("                                         ��                                                                ��\n");
			printf("                                         ��                                                                ��\n");
			printf("                                         ��                         2.���� ����                            ��\n");
			printf("                                         ��                                                                ��\n");
			printf("                                         ��                                                                ��\n");
			printf("                                         ��                    ->   3.���� ����                            ��\n");
			printf("                                         ��                                                                ��\n");
			printf("                                         ��                                                                ��\n");
			printf("                                         �ڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡ�\n");


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
					// ù ��° �ɼ� ó��
					StartPage = false;
					cls(WHITE, BLACK); // ���� ȭ�� �����
					break;
				case 2:
					StartPage = false;
					cls(WHITE, BLACK); // ���� ȭ�� �����
					break;
				case 3:
					StartPage = false;
					cls(WHITE, BLACK); // ���� ȭ�� �����
					break;
				default:
					// �߸��� �ɼ� ó��
					break;
				}
			}
		}

	}

	return selectedOption;
}

void introducePage() {

	int selectedOption = 1;
	// Ư��Ű 0xe0 �� �Է¹������� unsigned char �� �����ؾ� ��
	unsigned char ch;
	bool StartPage = TRUE;
	char key;
	while (StartPage) {
	


			printf("                                         �ڡڡڡڡڡڡڡڡڡڡڡڡڡ� ���� �� ���� �ڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡ�\n");
			printf("                                         ��                                                                      ��\n");
			printf("                                         ��                                                                      ��\n");
			printf("                                         ��                                                                      ��\n");
			printf("                                         ��                                                                      ��\n");
			printf("                                         ��                                                                      ��\n");
			printf("                                         ��                                                                      ��\n");
			printf("                                         ��                                                                      ��\n");
			printf("                                         ��                                                                      ��\n");
			printf("                                         ��                                                                      ��\n");
			printf("                                         ��                                                                      ��\n");
			printf("                                         ��                                                                      ��\n");
			printf("                                         �ڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡ�\n");


		



		Sleep(100);
		gotoxy(0, 0);
		if (_kbhit()) {
			key = _getch();

			if (key == ENTER) {
				StartPage = FALSE;
			}
		}

	}


	cls(WHITE, BLACK); // ���� ȭ�� �����

}

void init_game() {
	//���߿� ������ ����ǰ� ������ �ٽ� ���۵� ��
	//���� ������ �ʱ�ȭ ���ִ� ����
	
	system("cls");  //������ ���۵Ǹ� �ʱ�ȭ���� ���������
	removeCursor();
}

void playerdraw(int x, int y) {
	textcolor(GREEN1, BLACK);
	gotoxy(x, y);
	printf("<=��=>");
}

void playererase(int x, int y) {
	gotoxy(x, y);
	printf("      ");
}


void player1(unsigned char ch) {

	int move_flag = 0; //�÷��̾��� ������ ���θ� ��Ÿ���� ����
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
		playerdraw(newx, newy); // ���ο� ��ġ���� �÷��̾� ǥ��
		oldx = newx; // ������ ��ġ�� ����Ѵ�.
		oldy = newy;

	}
}
//�Ѿ� �׸���
void DrawBullet(int i) {
	textcolor(GREEN1, BLACK);
	gotoxy(Bullet[i].x, Bullet[i].y); printf("��");
	

	
}
//�Ѿ� �����
void EraseBullet(int i) {
	gotoxy(Bullet[i].x, Bullet[i].y); printf("  ");
	
}

//�Ѿ� ������
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

// �켱 ������ ������ �Ǹ� �츮�� ��� Ű���带 Ȱ���� ���̴� while���� ���� �����غ���

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
						//DrawBullet(i); // ����: �Ѿ��� ȭ�鿡 �׸�
					}
					if (i != MAXBULLET) {
						Bullet[i].x = newx + 1;
						Bullet[i].y = newy - 1;
						Bullet[i].exist = TRUE;
					}
				}
			}
			else {
				//�÷��̾� ���� �ӵ����� �Լ�
			}
			//�Ѿ� �ӵ����� �Լ�
			bulletmove();
			Sleep(Delay); // Delay ���� ���̰�
			frame_count++;// frame_count ������ �ӵ� ������ �Ѵ�.
		}
}






void main()
{
	// ���̵� â + ����� ���� â �������Ѵ�. ������ �ϴ� ���� ȭ����� ���� ������!!
	srand(time(NULL)); 
	//cls(BLACK, WHITE);
	cls(WHITE, BLACK);
	centerConsoleWindow();
	system("mode con cols=150 lines=34 | title 1891093 ������ ���ð���"); // �ܼ�â ũ�� �� ���� ����
	int i, x, y;
	char buf[100];
	system("cls");   //ȭ�� �����
	removeCursor(); // Ŀ���� �Ⱥ��̰� �Ѵ�
	int selectedOption = 1;
	// Ư��Ű 0xe0 �� �Է¹������� unsigned char �� �����ؾ� ��
	unsigned char ch;
	char key;
	bool isFinish = false;
	bool isGameRunning = false;  // ���� ���� ���θ� ��Ÿ���� �ο� ����
	IntroPage();

	
		
	while (isFinish == false) {
		if (isGameRunning) {
			// ���� ���� ���� ���
			
			gamestart();
		
		
			
		}
		else {
			// ���� ���� ���� �ƴ� ���
			
			// �޴� ���ÿ� ���� �ٸ� ȭ������ �̵��ϵ��� ó��
			switch (MenuPage()) {
			case 1:
				isGameRunning = true;  // ù ��° ���� ȭ������ �̵�
				break;
			case 2:
				//@@@@@ ���ӿ� ���� ������ �����ִ� ȭ�� @@@@ 
			
				introducePage();
				break;
			default:
				isFinish = true;
				break;
			}
		}
	}

	
	




}

