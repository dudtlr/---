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


#define StartX 72  //���� ������ x��ǥ
#define StartY 30  //���� ������ y��ǥ

static int called = 0;

static int oldx = StartX, oldy = StartY; // �÷��̾��� old ��ǥ
static int newx = StartX, newy = StartY; //�÷��̾��� new ��ǥ
static int keep_moving = 4;  //1:����̵�

#define MAXBULLET 6  //�÷��̾��� �ִ� �Ѿ� ��
#define TRUE 1
#define FALSE 0

struct {
	int exist;
	int x, y;
}Bullet[MAXBULLET];

#define SPACE 0x20
#define MAXENEMY 15

//@@@@@@@@@��@@@@@

struct {
	int exist;
	int x, y;
	int number;
	int move;
	int type;
}Enemy[MAXENEMY];

char* EnemyType[] = {"�ڡء�","�ݡڢ�","�ߣ���","�أ���","��ڡ�", "������","������","���ڢ�","������","�¡ڢ�"};

//@@@@@@@@@��@@@@@


//@@@@@@@@@�� �Ѿ�@@@@@@@@@@@
//#define MAXENEMYBULLET 20 // �� �ִ� �Ѿ� ��
#define MAXENEMYBULLET 5 // �� �ִ� �Ѿ� ��

static int enemybulletuse = 2; //�� �Ѿ� 1�ʸ��� ����
static int enemybullet_frame_sync = 50;  //�� �Ѿ� �ӵ�����

struct {
	int exist;
	int x, y;
}EnemyBullet[MAXENEMYBULLET];

//@@@@@@@@@�� �Ѿ�@@@@@@@@@@@


// ���� �� �� �׸� �� ���� ����
static int score = 0; // ����
static int heart = 5; // ����

#define ITEM "<H>"
#define SPEEDITEM "<S>"
#define POWERITEM "<P>"
#define MISILEITEM "<M>"

int items[WIDTH][HEIGHT] = { 0 };
int items_count = 0;
int items_use = 35; //������ ��Ÿ���� �ð�(��� ������)


int speeditems[WIDTH][HEIGHT] = { 0 };
int speeditems_count = 0;
int speeditems_use = 23; //������ ��Ÿ���� �ð�(���ǵ� ������)


int MyJet = 1; // ��Ʈ�� ���� ����

int PickMyLevel = 1; //���̵� �⺻ easy 1 / hard 2





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


//����� 1 �׸�
void Zet1(int x, int y) {

	textcolor(BLUE2, BLACK);
	gotoxy(x, y);
	printf("    ��    ");
	gotoxy(x, y + 1);
	printf("�ڡᢼ���");
}


//����� 2 �׸�
void Zet2(int x, int y) {
	
	textcolor(GREEN2, BLACK);
	gotoxy(x, y);
	printf("    ��    ");
	gotoxy(x, y + 1);
	printf("���ᢾ�ᢺ");
}


//����� 3 �׸�
void Zet3(int x, int y) {

	textcolor(CYAN2, BLACK);
	gotoxy(x, y);
	printf("    ��    ");
	gotoxy(x, y + 1);
	printf("���ߡܡߢ�");
}

//���̵� ������� ������
void Level1(int x, int y) {

	textcolor(GREEN1, BLACK);
	gotoxy(x, y);
	printf("�ڡڡ�      ��       �ڡڡ�  ��      ��  \n");
	gotoxy(x, y+1);
	printf("��        ��  ��    ��         ��  ��    \n");
	gotoxy(x, y+2);
	printf("�ڡڡ�   �ڡڡڡ�    �ڡڡ�      ��      \n");
	gotoxy(x, y + 3);
	printf("��      ��      ��        ��     ��      \n");
	gotoxy(x, y + 4);
	printf("�ڡڡ� ��        ��  �ڡڡ�      ��      \n");
}
//���̵� ����� ���� ������
void Level2(int x, int y) {

	textcolor(RED1, BLACK);
	gotoxy(x, y);
	printf("��  ��      ��      �ڡڡڡ�  �ڡڡ�  \n");
	gotoxy(x, y + 1);
	printf("��  ��    ��  ��    ��    ��  ��   �� \n");
	gotoxy(x, y + 2);
	printf("�ڡڡ�   �ڡڡڡ�   �ڡڡ�    ��    ��\n");
	gotoxy(x, y + 3);
	printf("��  ��  ��      ��  ��  ��    ��   �� \n");
	gotoxy(x, y + 4);
	printf("��  �� ��        �� ��    ��  �ڡڡ�  \n");
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

void centerConsoleWindow() //�ܼ�ȭ�� ���߾ӿ� �����ֱ�!
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

void IntroPage() {  // ���� ��Ʈ�� ȭ��
	int i;
	while (1) {


		for (i = 0; i <= 20; i++) {
			Sleep(10);
			//draw_box2(i * 2, i, 78 - i * 2, 22 - i, "��");
			draw_box2(i * 2, i, WIDTH  - i * 2, HEIGHT - 2 - i, "��");
		}

		for (i = 20; i >= 0; i--) {
			Sleep(10);
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

int LoadingPage() {
	//cls(WHITE, BLACK);
	//gotoxy(0, 0);
	int selectedOption = 1;
	// Ư��Ű 0xe0 �� �Է¹������� unsigned char �� �����ؾ� ��
	unsigned char ch;
	bool StartPage = TRUE;
	char key;
	while (StartPage) {
		textcolor(MAGENTA2, 0);
		printf("\n");
		printf("           �ڡڡڡڡ�      ��        ��    �ڡڡڡڡڡ�    �ڡڡڡڡڡ�    �ڡڡڡڡڡ�    �ڡڡڡڡڡ�    �ڡ�         ��     �ڡڡڡڡڡ�          \n");
		printf("         ��                ��        ��    ��        ��    ��        ��         ��              ��         �� ��        ��   ��                      \n");
		printf("         ��                ��        ��    ��        ��    ��        ��         ��              ��         ��   ��      ��   ��                      \n");
		printf("           �ڡڡڡڡ�      �ڡڡڡڡڡ�    ��        ��    ��        ��         ��              ��         ��     ��    ��   ��      �ڡڡ�          \n");
		
		printf("                     ��    ��        ��    ��        ��    ��        ��         ��              ��         ��       ��  ��   ��       ��             \n");
		printf("                     ��    ��        ��    ��        ��    ��        ��         ��              ��         ��         �ڡ�   ��       ��             \n");
		printf("           �ڡڡڡڡ�      ��        ��    �ڡڡڡڡڡ�    �ڡڡڡڡڡ�         ��         �ڡڡڡڡڡ�    ��           ��     �ڡڡڡڡڡ�          \n");
		printf("\n");
		
		printf("                                           �ڡڡڡڡڡ�          ��        ��             ��    �ڡڡڡڡڡ�            \n");
		printf("                                         ��                     �ڡ�       �ڡ�         �ڡ�    ��                      \n");
		printf("                                         ��                    ��  ��      �� ��       �� ��	��			            \n");
		printf("                                         ��     �ڡڡ�        �ڡڡڡ�     ��  ��     ��  ��    �ڡڡڡڡڡ�            \n");
		printf("                                         ��       ��         ��      ��	   ��	��   ��   ��    ��				        \n");
		printf("                                         ��       ��        ��        ��   �� 	 �� ��    ��	��			            \n");
		printf("                                           �ڡڡڡڡڡ�    ��          ��  ��	  ��      ��    �ڡڡڡڡڡ�			\n");
		textcolor(11, 0);
		printf("\n");
		printf("\n");
		printf("                                                                                       made by 1891093 ������			\n");
		textcolor(15, 0);
		
		textcolor(BLUE1, 0);
		printf("	                                                        �� �ε� ���Դϴ� !! ��\n");

		textcolor(YELLOW1, 0);
		if (selectedOption == 1) {
			printf("                                         �ڡڡڡڡڡڡڡڡڡڡڡڡ�   Loading  \n");
			printf("                                         ��                                                                \n");
			printf("                                         ��                                                                \n");
			printf("                                         ��                                                                \n");
			printf("                                         ��                                                                \n");
			printf("                                         ��                                                                \n");
			printf("                                         ��                        �ε� �� 30% !! .                          \n");
			printf("                                         ��                                                                \n");
			printf("                                         ��                                                                \n");
			printf("                                         ��                                                                \n");
			printf("                                         ��                                                                \n");
			printf("											\n");
		}

		else if (selectedOption == 2) {
			printf("                                         �ڡڡڡڡڡڡڡڡڡڡڡڡ�   Loading  \n");
			printf("                                         ��                                                                \n");
			printf("                                         ��                                                                \n");
			printf("                                         ��                                                                \n");
			printf("                                         ��                                                                \n");
			printf("                                         ��                                                                \n");
			printf("                                         ��                        �ε� �� 70% !! . .                      \n");
			printf("                                         ��                                                                \n");
			printf("                                         ��                                                                \n");
			printf("                                         ��                                                                \n");
			printf("                                         ��                                                                \n");
			printf("                                         ��                                                                \n");
			printf("                                         �ڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡ�\n");



		}
		else if (selectedOption == 3) {
			printf("                                         �ڡڡڡڡڡڡڡڡڡڡڡڡ�   Loading  �ڡڡڡڡڡڡڡڡڡڡڡڡڡڡ�\n");
			printf("                                         ��                                                                ��\n");
			printf("                                         ��                                                                ��\n");
			printf("                                         ��                                                                ��\n");
			printf("                                         ��                                                                ��\n");
			printf("                                         ��                                                                ��\n");
			printf("                                         ��                        �ε� �� 100% !! . . .                     ��\n");
			printf("                                         ��                                                                ��\n");
			printf("                                         ��                                                                ��\n");
			printf("                                         ��                                                                ��\n");
			printf("                                         ��                                                                ��\n");
			printf("                                         ��                                                                ��\n");
			printf("                                         �ڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡ�\n");



		}
		else {
			printf("                                         �ڡڡڡڡڡڡڡڡڡڡڡڡ�   Loading  �ڡڡڡڡڡڡڡڡڡڡڡڡڡڡ�\n");
			printf("                                         ��                                                                ��\n");
			printf("                                         ��                                                                ��\n");
			printf("                                         ��                                                                ��\n");
			printf("                                         ��                                                                ��\n");
			printf("                                         ��                                                                ��\n");
			printf("                                         ��                         �� ���� �մϴ� !!                      ��\n");
			printf("                                         ��                                                                ��\n");
			printf("                                         ��                                                                ��\n");
			printf("                                         ��                                                                ��\n");
			printf("                                         ��                                                                ��\n");
			printf("                                         ��                                                                ��\n");
			printf("                                         �ڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡ�\n");


		}
		Sleep(100);
		selectedOption++;
		Sleep(1000);

		gotoxy(0, 0);
		
		if (selectedOption == 5) {
			cls(WHITE, BLACK); // ���� ȭ�� �����
			StartPage = FALSE;
			break;
		}


	}

	return selectedOption;
}


int pickMyJet() {
	int selectedOption = 1;
	// Ư��Ű 0xe0 �� �Է¹������� unsigned char �� �����ؾ� ��
	unsigned char ch;
	bool pickMyJetPage = TRUE;
	char key;

	
	while (pickMyJetPage) {
		changeTextColor();
		printf("\n"); 
		printf("                      �ڡڡڡڡڡ�   �ڡڡڡڡڡ�    �ڡڡڡڡڡ�   �ڡڡڡڡڡ�    �ڡڡڡڡڡ�      �ڡڡڡڡ�    ��        ��               \n");
		printf("                              ��     ��                   ��        ��        ��         ��         ��              ��     ��               \n");
		changeTextColor();
		printf("                            ��       �ڡڡڡڡڡ�         ��        �ڡڡڡڡڡ�         ��         ��              �ڡڡ�                  \n");
		printf("                          ��         ��                   ��        ��                   ��         ��              ��     ��             \n");
		printf("                        ��           ��                   ��        ��                   ��         ��              ��       ��           \n");
		changeTextColor();
		printf("                      �ڡڡڡڡڡ�   �ڡڡڡڡڡ�         ��        ��              �ڡڡڡڡڡ�      �ڡڡڡڡ�    ��        ��           \n");

		printf("\n");
		textcolor(WHITE, BLACK);

		if (selectedOption == 1) {
			printf("                      �ڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡ�        ��Ʈ�⸦ �����ϼ��� !!      �ڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡ� \n");
			printf("                      ��                                                                                                      ��\n");
			printf("                      ��                                                                                                      ��\n");
			printf("                      ��                                                                                                      ��\n");
			printf("                      ��                                                                                                      ��\n");
			printf("                      ��            1�� ��Ʈ��                       2�� ��Ʈ��                         3�� ��Ʈ��            ��\n");
			printf("                      ��      �ڡڡڡڡڡڡڡڡڡڡ�           �ڡڡڡڡڡڡڡڡڡڡ�             �ڡڡڡڡڡڡڡڡڡڡ�      ��\n");
			printf("                      ��      ��                  ��           ��                  ��             ��                  ��      ��\n");
			printf("                      ��      ��                  ��           ��                  ��             ��                  ��      ��\n");
			printf("                      ��      ��                  ��           ��                  ��             ��                  ��      ��\n");
			printf("                      ��      ��                  ��           ��                  ��             ��                  ��      ��\n");
			printf("                      ��      ��                  ��           ��                  ��             ��                  ��      ��\n");
			printf("                      ��      ��                  ��           ��                  ��             ��                  ��      ��\n");
			printf("                      ��      ��                  ��           ��                  ��             ��                  ��      ��\n");
			printf("                      ��      ��                  ��           ��                  ��             ��                  ��      ��\n");
			printf("                      ��      �ڡڡڡڡڡڡڡڡڡڡ�           �ڡڡڡڡڡڡڡڡڡڡ�             �ڡڡڡڡڡڡڡڡڡڡ�      ��\n");
			printf("                      ��                                                                                                      ��\n");
			printf("                      ��                ��                                                                                    ��\n");
			printf("                      ��                                                                                                      ��\n");
			printf("                      ��                                                                                                      ��\n");
			printf("                      ��                                                                                                      ��\n");
			printf("                      ��                                                                                                      ��\n");
			printf("                      ��                                                                                                      ��\n");
			printf("                      �ڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡ�        ��Ʈ�⸦ �����ϼ��� !!      �ڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡ� \n");

			
			Zet1(36, 18);

			Zet2(69, 18);

			Zet3(104, 18);

		
		}

		else if (selectedOption == 2) {
			printf("                      �ڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡ�        ��Ʈ�⸦ �����ϼ��� !!      �ڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡ� \n");
			printf("                      ��                                                                                                      ��\n");
			printf("                      ��                                                                                                      ��\n");
			printf("                      ��                                                                                                      ��\n");
			printf("                      ��                                                                                                      ��\n");
			printf("                      ��            1�� ��Ʈ��                       2�� ��Ʈ��                         3�� ��Ʈ��            ��\n");
			printf("                      ��      �ڡڡڡڡڡڡڡڡڡڡ�           �ڡڡڡڡڡڡڡڡڡڡ�             �ڡڡڡڡڡڡڡڡڡڡ�      ��\n");
			printf("                      ��      ��                  ��           ��                  ��             ��                  ��      ��\n");
			printf("                      ��      ��                  ��           ��                  ��             ��                  ��      ��\n");
			printf("                      ��      ��                  ��           ��                  ��             ��                  ��      ��\n");
			printf("                      ��      ��                  ��           ��                  ��             ��                  ��      ��\n");
			printf("                      ��      ��                  ��           ��                  ��             ��                  ��      ��\n");
			printf("                      ��      ��                  ��           ��                  ��             ��                  ��      ��\n");
			printf("                      ��      ��                  ��           ��                  ��             ��                  ��      ��\n");
			printf("                      ��      ��                  ��           ��                  ��             ��                  ��      ��\n");
			printf("                      ��      �ڡڡڡڡڡڡڡڡڡڡ�           �ڡڡڡڡڡڡڡڡڡڡ�             �ڡڡڡڡڡڡڡڡڡڡ�      ��\n");
			printf("                      ��                                                                                                      ��\n");
			printf("                      ��                                                 ��                                                   ��\n");
			printf("                      ��                                                                                                      ��\n");
			printf("                      ��                                                                                                      ��\n");
			printf("                      ��                                                                                                      ��\n");
			printf("                      ��                                                                                                      ��\n");
			printf("                      ��                                                                                                      ��\n");
			printf("                      �ڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡ�        ��Ʈ�⸦ �����ϼ��� !!      �ڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡ� \n");


			Zet1(36, 18);

			Zet2(69, 18);

			Zet3(104, 18);


		}
		else {
			printf("                      �ڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡ�        ��Ʈ�⸦ �����ϼ��� !!      �ڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡ� \n");
			printf("                      ��                                                                                                      ��\n");
			printf("                      ��                                                                                                      ��\n");
			printf("                      ��                                                                                                      ��\n");
			printf("                      ��                                                                                                      ��\n");
			printf("                      ��            1�� ��Ʈ��                       2�� ��Ʈ��                         3�� ��Ʈ��            ��\n");
			printf("                      ��      �ڡڡڡڡڡڡڡڡڡڡ�           �ڡڡڡڡڡڡڡڡڡڡ�             �ڡڡڡڡڡڡڡڡڡڡ�      ��\n");
			printf("                      ��      ��                  ��           ��                  ��             ��                  ��      ��\n");
			printf("                      ��      ��                  ��           ��                  ��             ��                  ��      ��\n");
			printf("                      ��      ��                  ��           ��                  ��             ��                  ��      ��\n");
			printf("                      ��      ��                  ��           ��                  ��             ��                  ��      ��\n");
			printf("                      ��      ��                  ��           ��                  ��             ��                  ��      ��\n");
			printf("                      ��      ��                  ��           ��                  ��             ��                  ��      ��\n");
			printf("                      ��      ��                  ��           ��                  ��             ��                  ��      ��\n");
			printf("                      ��      ��                  ��           ��                  ��             ��                  ��      ��\n");
			printf("                      ��      �ڡڡڡڡڡڡڡڡڡڡ�           �ڡڡڡڡڡڡڡڡڡڡ�             �ڡڡڡڡڡڡڡڡڡڡ�      ��\n");
			printf("                      ��                                                                                                      ��\n");
			printf("                      ��                                                                                    ��                ��\n");
			printf("                      ��                                                                                                      ��\n");
			printf("                      ��                                                                                                      ��\n");
			printf("                      ��                                                                                                      ��\n");
			printf("                      ��                                                                                                      ��\n");
			printf("                      ��                                                                                                      ��\n");
			printf("                      �ڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡ�        ��Ʈ�⸦ �����ϼ��� !!      �ڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡ� \n");


			Zet1(36, 18);

			Zet2(69, 18);

			Zet3(104, 18);


		}



		Sleep(100);
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
					// ù ��° �ɼ� ó��
					pickMyJetPage = false;
					cls(WHITE, BLACK); // ���� ȭ�� �����
					MyJet = 1;
					break;
				case 2:
					pickMyJetPage = false;
					cls(WHITE, BLACK); // ���� ȭ�� �����
					MyJet = 2;
					break;
				case 3:
					pickMyJetPage = false;
					cls(WHITE, BLACK); // ���� ȭ�� �����
					MyJet = 3;
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

int pickGameLevel() {
	int selectedOption = 1;
	// Ư��Ű 0xe0 �� �Է¹������� unsigned char �� �����ؾ� ��
	unsigned char ch;
	bool pickMyLevelPage = TRUE;
	char key;



	while (pickMyLevelPage) {
		changeTextColor();
		printf("\n");
		
		printf("        ��             �ڡڡڡڡڡ�    ��        ��   �ڡڡڡڡڡ�    ��             �ڡڡڡڡڡ�   �ڡڡڡڡڡ�      �ڡڡڡڡ�    ��        ��     \n");
		
		changeTextColor(); 
		printf("        ��             ��               ��      ��    ��              ��             ��        ��        ��         ��              ��     �� \n");
		printf("        ��             �ڡڡڡڡڡ�      ��    ��     �ڡڡڡڡڡ�    ��             �ڡڡڡڡڡ�        ��         ��              �ڡڡ�    \n");
		
		changeTextColor(); 
		printf("        ��             ��                 ��  ��      ��              ��             ��                  ��         ��              ��     ��   \n");
		printf("        ��             ��                   ��        ��              ��             ��                  ��         ��              ��       ��  \n");
		printf("        �ڡڡڡڡڡ�   �ڡڡڡڡڡ�         ��        �ڡڡڡڡڡ�    �ڡڡڡڡڡ�   ��             �ڡڡڡڡڡ�      �ڡڡڡڡ�    ��        ��            \n");

	
		textcolor(WHITE, BLACK);
		printf("\n");

		if (selectedOption == 1) {
			printf("                      �ڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡ�        ���̵��� �����ϼ��� !!      �ڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡ� \n");
			printf("                      ��                                                                                                        ��\n");
			printf("                      ��                                                                                                        ��\n");
			printf("                      ��                                                                                                        ��\n");
			printf("                      ��                                                                                                        ��\n");
			printf("                      ��                                                                                                        ��\n");
			printf("                      ��  �ڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡ�      �ڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡ�    ��\n");
			printf("                      ��  ��                                          ��      ��                                          ��    ��\n");
			printf("                      ��  ��                                          ��      ��                                          ��    ��\n");
			printf("                      ��  ��                                          ��      ��                                          ��    ��\n");
			printf("                      ��  ��                                          ��      ��                                          ��    ��\n");
			printf("                      ��  ��                                          ��      ��                                          ��    ��\n");
			printf("                      ��  ��                                          ��      ��                                          ��    ��\n");
			printf("                      ��  ��                                          ��      ��                                          ��    ��\n");
			printf("                      ��  ��                                          ��      ��                                          ��    ��\n");
			printf("                      ��  �ڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡ�      �ڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡ�    ��\n");
			printf("                      ��                                                                                                        ��\n");
			printf("                      ��                        ��                                                                              ��\n");
			printf("                      ��                                                                                                        ��\n");
			printf("                      ��                                                                                                        ��\n");
			printf("                      ��                                                                                                        ��\n");
			printf("                      ��                                                                                                        ��\n");
			printf("                      ��                                                                                                        ��\n");
			printf("                      �ڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡ�        ���̵��� �����ϼ��� !!      �ڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡ� \n");


			Level1(29, 17);

			Level2(82, 17);

			


		}

		
		else {
			printf("                      �ڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡ�        ���̵��� �����ϼ��� !!      �ڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡ� \n");
			printf("                      ��                                                                                                        ��\n");
			printf("                      ��                                                                                                        ��\n");
			printf("                      ��                                                                                                        ��\n");
			printf("                      ��                                                                                                        ��\n");
			printf("                      ��                                                                                                        ��\n");
			printf("                      ��  �ڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡ�      �ڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡ�    ��\n");
			printf("                      ��  ��                                          ��      ��                                          ��    ��\n");
			printf("                      ��  ��                                          ��      ��                                          ��    ��\n");
			printf("                      ��  ��                                          ��      ��                                          ��    ��\n");
			printf("                      ��  ��                                          ��      ��                                          ��    ��\n");
			printf("                      ��  ��                                          ��      ��                                          ��    ��\n");
			printf("                      ��  ��                                          ��      ��                                          ��    ��\n");
			printf("                      ��  ��                                          ��      ��                                          ��    ��\n");
			printf("                      ��  ��                                          ��      ��                                          ��    ��\n");
			printf("                      ��  �ڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡ�      �ڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡ�    ��\n");
			printf("                      ��                                                                                                        ��\n");
			printf("                      ��                                                                             ��                         ��\n");
			printf("                      ��                                                                                                        ��\n");
			printf("                      ��                                                                                                        ��\n");
			printf("                      ��                                                                                                        ��\n");
			printf("                      ��                                                                                                        ��\n");
			printf("                      ��                                                                                                        ��\n");
			printf("                      �ڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡ�        ���̵��� �����ϼ��� !!      �ڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡ� \n");


			Level1(29, 17);

			Level2(82, 17);




		}



		Sleep(100);
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
					// ù ��° �ɼ� ó��
					pickMyLevelPage = false;
					cls(WHITE, BLACK); // ���� ȭ�� �����
					PickMyLevel = 1;
					break;
				case 2:
					pickMyLevelPage = false;
					cls(WHITE, BLACK); // ���� ȭ�� �����
					PickMyLevel = 2;
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
		if (kbhit()) {
			key = getch();

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








void player1(unsigned char ch) {

	int move_flag = 0; //�÷��̾��� ������ ���θ� ��Ÿ���� ����
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
		Zet1Draw(newx, newy); // ���ο� ��ġ���� �÷��̾� ǥ��
		oldx = newx; // ������ ��ġ�� ����Ѵ�.
		oldy = newy;

	}
}
//�Ѿ� �׸���
void DrawBullet(int i) {
	textcolor(YELLOW1, BLACK);
	gotoxy(Bullet[i].x, Bullet[i].y); printf("@@");	
	//textcolor(RED1, BLACK); // ��� �� ����� ���� 
}

//�Ѿ� �����
void EraseBullet(int i) {
	gotoxy(Bullet[i].x, Bullet[i].y); printf("    ");
	
}

//�Ѿ� ������ 
void MoveBullet() {
	int i;

	for (i = 0; i < MAXBULLET; i++) {
		if (Bullet[i].exist == TRUE) {
			EraseBullet(i);
			if (Bullet[i].y <= 2) { // y ���� 3 ������ �� ����ϴ�.
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



// ������!!
void CreateBasicEnemy() {
	int i, location, direct, hieght;
	location = rand() % 2;
	direct = 5 + rand() % 90;  //x 5 , y3
	//hieght = 3 + rand() % 10;
	hieght = 2;
	for (i = 0; i < MAXENEMY && Enemy[i].exist == TRUE; i++) {}
	if (i != MAXENEMY) {
		if (location == 1) {
			Enemy[i].x = direct;
			Enemy[i].y = hieght;
			Enemy[i].move = 1;
		}
		else {
			Enemy[i].x = direct;
			Enemy[i].y = hieght;
			Enemy[i].move = -1;
		}
		Enemy[i].type = rand() % 10;
		Enemy[i].exist = TRUE;
	}
}

//�������� 
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
			printf("      "); // ���� ��ġ�� �� ����

			Enemy[i].y += 1; // �Ʒ��� �̵�

			if (Enemy[i].y >= HEIGHT - 4) {
				Enemy[i].exist = FALSE; // ȭ���� ��� �� ����
			}
			else {
				gotoxy(Enemy[i].x, Enemy[i].y);
				textcolor(RED1, BLACK);
				printf(EnemyType[Enemy[i].type]); // ���ο� ��ġ�� �� �׸���
			}
		}
	}
}


//���Ѿ�
void EnemyBulletshow() {
	if (PickMyLevel == 1) {
		textcolor(WHITE, BLACK);
		gotoxy(0, 0); printf("��");
	}
	if (PickMyLevel == 2) {
		textcolor(RED1, BLACK);
		gotoxy(0, 0); printf("��");
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

//�Ѿ� �׸���
void EnemyBulletdraw(int i) {
	textcolor(RED2, BLACK);
	gotoxy(EnemyBullet[i].x, EnemyBullet[i].y);
	printf("*");
}

// ���Ѿ� �����
void EnemyBulleterase(int i) {
	gotoxy(EnemyBullet[i].x, EnemyBullet[i].y);
	printf(" ");
}

// ���Ѿ� ������
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

	// �� �Ѿ��� ȭ�� ������ ������ �����
	for (int i = 0; i < MAXENEMYBULLET; i++) {
		if (EnemyBullet[i].exist == FALSE) {
			EnemyBulleterase(i);
		}
	}


}

//���� ����â
void info() {
	//textcolor(YELLOW2, BLACK);
	textcolor(WHITE, BLACK);
	gotoxy(122, 5); printf("����: %d", score);
	gotoxy(122, 10);
	printf("���:");
	if (heart >= 4) {
		heart = 4;
		gotoxy(130, 10); printf("       ");
		gotoxy(130, 10); printf("��������");
	}
	else if (heart == 3) {
		gotoxy(130, 10); printf("       ");
		gotoxy(130, 10); printf("������");
	}
	else if (heart == 2) {
		gotoxy(130, 10); printf("       ");
		gotoxy(130, 10); printf("����");
	}
	else if (heart == 1) {
		gotoxy(130, 10); printf("       ");
		gotoxy(130, 10); printf("��");
	}
	else {
		gotoxy(130, 10);
		printf("  ");
	}

}

// ���� �� �Ѿ˿� �¾��� ��
void playerfall() {
	int i;
	for (i = 0; i < MAXENEMYBULLET; i++) {
		if (EnemyBullet[i].exist == FALSE)
			continue;
		if (EnemyBullet[i].y == newy && abs(EnemyBullet[i].x - newx) <= 5) {
			EnemyBullet[i].exist = FALSE;
			gotoxy(EnemyBullet[i].x, EnemyBullet[i].y);
			printf("    ");
			heart--;
			info();
		}
	}
	

}

// ���� ���� ������ ��!
void EnemyTouch() {
	int i;
	for (i = 0; i < MAXENEMY; i++) {
		if (Enemy[i].exist == FALSE)
			continue;
		if (Enemy[i].y == newy && abs(Enemy[i].x - newx) <= 5) {
			Enemy[i].exist = FALSE;
			gotoxy(Enemy[i].x, Enemy[i].y);
			printf("      ");
			heart--;
			info();
		}
	}

}



// ���� �� �Ѿ˿� �¾��� �� ��Ȳ �����ϱ�
void DeleteEnemy() {
	int i;
	for (i = 0; i < MAXENEMY; i++) {
		if (Enemy[i].exist == FALSE || Enemy[i].type == -1)
			continue;
		if (Enemy[i].y == Bullet[i].y && abs(Enemy[i].x - Bullet[i].x) <= 12) {
			gotoxy(Bullet[i].x, Bullet[i].y);
			printf("   ");
			Bullet[i].exist = FALSE;
			Enemy[i].type = -1;
			score += 20;
			info();
			break;
		}
	}
	
}


//����ȭ�鿡�� �� �����!!!
void GameMap() {
	int map[33][60];
	int x, y;

	// �� �ʱ�ȭ
	for (y = 0; y < 33; y++) {
		for (x = 0; x < 60; x++) {
			if (y == 0 || y == 32 || x == 0 || x == 59) {
				map[y][x] = 1;  // �׵θ�
			}
			else {
				map[y][x] = 0;  // ����
			}
		}
	}

	// �� ���  // ���̵��� ���� ������ �ٲ���!!!!@@@@@@@@@@@@@@@@@

	
	if (PickMyLevel == 1) {
		textcolor(WHITE, BLACK);
	}
	//���̵��� hard ���
	if (PickMyLevel == 2) {
		textcolor(RED1, BLACK);
	}
	for (y = 0; y < 33; y++) {
		for (x = 0; x < 60; x++) {
			if (map[y][x] == 1) {
				printf("��");
			}
			else {
				printf("  ");
			}
		}
		printf("\n");
	}

}







// �켱 ������ ������ �Ǹ� �츮�� ��� Ű���带 Ȱ���� ���̴� while���� ���� �����غ���
//���� ���� ȭ��
void gamestart() {
	unsigned char ch;
	int i;
	//���̵��� easy���
	if (PickMyLevel == 1) {
		init_game(); // ���� ���� �ʱ�ȭ ���ֱ�
	}
	//���̵��� hard ���
	if (PickMyLevel == 2) {
		init_game(); // ���� ���� �ʱ�ȭ ���ֱ�
	}
	
	GameMap(); // ���� �� �׷��ֱ�!!! easy�� ��� HARD�� ������ 
	Zet1Draw(oldx, oldy); // ó���� ����� �׷��ֱ�!!  (���� ������ ����Ⱑ �׷�����.)
	info(); // �� ���� ���� �����Ȳ !! (while���ȿ� ������ ���ܺ��̱� ������ ��������� ���� ���� ȣ���ϰ� �س����鼭 ���� ������ ���߾���.)
	// �� ���� ���� ����
	int enemySpawnTimer = 0;
	int enemySpawnInterval = 10;  // �� ���� ���� ����
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
						Bullet[i].x = newx + 4;
						Bullet[i].y = newy - 1;
						Bullet[i].exist = TRUE;
					}
				}
			}
			else {
				//�÷��̾� ���� �ӵ����� �Լ�
			}

		

			//�Ѿ� �ӵ����� �Լ�
			MoveBullet();
			

			// �� ����
			if (enemySpawnTimer >= enemySpawnInterval) {
				CreateBasicEnemy();
				enemySpawnTimer = 0;
			}
			else {
				enemySpawnTimer++;
			}

			 // �� ������ ó��
			MoveEnemy2();



			// �� �Ѿ� ���� �� ������ ó��
			//EnemyBulletMove();
			//EnemyBulletshow();
			
			// �� �Ѿ˿� �¾��� �� ó��
			//playerfall();

			
			DeleteEnemy();// ���� �� �Ѿ˿� �¾� �״� ���� ó��
			EnemyTouch(); // ���� ���� ������ ��  ����� -1 
		
			
			
			

			


			
			// ���� ���� ǥ��
			


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
		if (isGameRunning) {// ���� ���� ���� ���
			
			pickMyJet(); // ����⸦ ����!!
			
			pickGameLevel();// ���̵� ���� ȭ�� 
			LoadingPage();// �ε� ȭ�� !!

			gamestart(); // ���� ����!!
		
		
			
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

