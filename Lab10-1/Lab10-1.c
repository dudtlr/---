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

int remainingTime = 5; // �������� ���� �ð� 2��
static int called = 0;

static int oldx = StartX, oldy = StartY; // �÷��̾��� old ��ǥ
static int newx = StartX, newy = StartY; //�÷��̾��� new ��ǥ
static int keep_moving = 4;  //1:����̵�

#define MAXBULLET 30  //�÷��̾��� �ִ� �Ѿ� ��
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
#define MAXENEMYBULLET 10 // �� �ִ� �Ѿ� ��

static int enemybulletuse = 1; //�� �Ѿ� 1�ʸ��� ����
static int enemybullet_frame_sync = 60;  //�� �Ѿ� �ӵ�����

struct {
	int exist;
	int x, y;
}EnemyBullet[MAXENEMYBULLET];

bool IsLevel1 = true; // 1�ܰ�
bool IsLevel2 = false; // 2�ܰ�
bool IsLevel3 = false; // 3�ܰ�
bool IsBoss = false; // ���� �ܰ�


bool IsEasy = false; // ���̵� ���� 
bool IsHard = false; // ���̵� �ϵ�



//@@@@@@@@@�� �Ѿ�@@@@@@@@@@@


// ���� �� �� �׸� �� ���� ����
static int score = 0; // ����
static int heart = 5; // ����
static int BossLife = 50; // ���� ����
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


// �ð��� ǥ���ϴ� �Լ�
void displayTime(int minutes, int seconds) {
	textcolor(CYAN1, BLACK);
	gotoxy(122, 3);
	printf("���� �ð�: %02d:%02d", minutes, seconds);
}

//���� 1 Ŭ���� �޽���
void Level1ClearMessage(int x, int y) {

	


	textcolor(GREEN1, BLACK);
	gotoxy(x, y );
	printf("�����մϴ�~~");
	gotoxy(x, y+1);
	printf("���� 1�� Ŭ���� �Ͽ����ϴ�!!");
	gotoxy(x, y + 2);
	printf("���� 2�� �Ѿ�ϴ�!!");
	Sleep(1000);
}

//���� 2 Ŭ���� �޽���
void Level2ClearMessage(int x, int y) {


	textcolor(GREEN1, BLACK);
	gotoxy(x, y);
	printf("�����մϴ�~~");
	gotoxy(x, y + 1);
	printf("���� 2�� Ŭ���� �Ͽ����ϴ�!!");
	gotoxy(x, y + 2);
	printf("���� 3���� �Ѿ�ϴ�!!");
	Sleep(1000);
}

//���� 3 Ŭ���� �޽���
void Level3ClearMessage(int x, int y) {

	textcolor(GREEN1, BLACK);
	gotoxy(x, y);
	printf("�����մϴ�~~");
	gotoxy(x, y + 1);
	printf("���� 3�� Ŭ���� �Ͽ����ϴ�!!");
	gotoxy(x, y + 2);
	printf("���� ���������� �Ѿ�ϴ�!!");
	Sleep(1000);
}


//���� ���� Ŭ���� �޽���
void BossClearMessage(int x, int y) {

	textcolor(GREEN1, BLACK);
	gotoxy(x, y);
	printf("�����մϴ�!!!!�ڡڡڡڡڡڡڡڡڡڡڡڡڡ�");
	gotoxy(x, y + 1);
	printf("���� ���������� Ŭ���� �Ͽ����ϴ�!!");
	gotoxy(x, y + 2);
	printf("�����մϴ١ڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡ�");
	Sleep(1000);
}


//Boss �׸� !!
void BossDraw(int x, int y) {

	textcolor(RED1, BLACK);
	gotoxy(x, y);			printf("  �ڡ�    �ڡ�");
	gotoxy(x , y + 1);		printf("@@@@@@@@@@@@@@@@");
	gotoxy(x, y + 2);		printf("@@��@@@@@@@@��@@");
	gotoxy(x, y + 3);		printf("@@@@@@@@@@@@@@@@");
	gotoxy(x, y + 4);       printf("�¢¢¢¢¢¢¢�");
	gotoxy(x, y + 5);       printf("�¢¢�    �¢¢�");
	gotoxy(x, y + 6);       printf("�¢¢¢¢¢¢¢�");


	
}

//Boss �¾��� �� �׸�
void BossDraw2(int x, int y) {

	textcolor(GRAY1, BLACK);
	gotoxy(x, y);			printf("  �ڡ�    �ڡ�");
	gotoxy(x, y + 1);		printf("@@@@@@@@@@@@@@@@");
	textcolor(MAGENTA2, BLACK);
	gotoxy(x, y + 2);		printf("@@��@@@@@@@@��@@");
	gotoxy(x, y + 3);		printf("@@@@@@@@@@@@@@@@");
	textcolor(CYAN1, BLACK);
	gotoxy(x, y + 4);       printf("�¢¢¢¢¢¢¢�");
	gotoxy(x, y + 5);       printf("�¢¢�    �¢¢�");
	gotoxy(x, y + 6);       printf("�¢¢¢¢¢¢¢�");

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

//����� �¾��� �� 
void Zet10(int x, int y) {

	textcolor(RED1, BLACK);
	gotoxy(x, y);
	printf("    ��    ");
	gotoxy(x, y + 1);
	printf("�ڡᢼ���");
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

int FinishPage() {
	int selectedOption = 1;
	// Ư��Ű 0xe0 �� �Է¹������� unsigned char �� �����ؾ� ��
	unsigned char ch;
	bool StartPage = TRUE;
	char key;
	while (StartPage) {
		changeTextColor();
		printf("                                           �ڡڡڡڡڡ�          ��        ��             ��    �ڡڡڡڡڡ�            \n");
		printf("                                         ��                     �ڡ�       �ڡ�         �ڡ�    ��                      \n");
		printf("                                         ��                    ��  ��      �� ��       �� ��	��			            \n");
		printf("                                         ��     �ڡڡ�        �ڡڡڡ�     ��  ��     ��  ��    �ڡڡڡڡڡ�            \n");
		changeTextColor();
		printf("                                         ��       ��         ��      ��	   ��	��   ��   ��    ��				        \n");
		printf("                                         ��       ��        ��        ��   �� 	 �� ��    ��	��			            \n");
		printf("                                           �ڡڡڡڡڡ�    ��          ��  ��	  ��      ��    �ڡڡڡڡڡ�			\n");

		changeTextColor();
		printf("\n"); 
		printf("                       �ڡڡڡڡڡ�     ��               �ڡڡڡڡڡڡ�            ��           �ڡڡڡڡڡڡ�           ��       ��     \n");
		printf("                     ��                 ��               ��                       �ڡ�          ��          ��           ��       ��     \n");
		printf("                     ��                 ��               ��                      ��  ��         ��          ��           ��       ��     \n");
		printf("                     ��                 ��               �ڡڡڡڡڡڡ�         �ڡڡڡ�        �ڡڡڡڡڡڡ�           ��       ��     \n");
		changeTextColor();
		printf("                     ��                 ��               ��                    ��      ��       ��  ��                   ��       ��     \n");
		printf("                     ��                 ��               ��                   ��        ��      ��      ��                               \n");
		printf("                       �ڡڡڡڡڡ�     �ڡڡڡڡڡڡ�   �ڡڡڡڡڡڡ�      ��          ��     ��          ��           ��       ��     \n");
		printf("\n");
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
			printf("                                         ��                                                                ��\n");
			printf("                                         ��                                                                ��\n");
			printf("                                         ��                      ->   1.���ư���                           ��\n");
			printf("                                         ��                                                                ��\n");
			printf("                                         ��                                                                ��\n");
			printf("                                         ��                           2.��������                           ��\n");
			printf("                                         ��                                                                ��\n");
			printf("                                         ��                                                                ��\n");
			printf("                                         ��                                                                ��\n");
			printf("                                         �ڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡ�\n");



		}

		
		else {
			printf("                                         �ڡڡڡڡڡڡڡڡڡڡڡڡڡ�   MENU   �ڡڡڡڡڡڡڡڡڡڡڡڡڡڡ�\n");
			printf("                                         ��                                                                ��\n");
			printf("                                         ��                                                                ��\n");
			printf("                                         ��                                                                ��\n");
			printf("                                         ��                                                                ��\n");
			printf("                                         ��                         1.���ư���                             ��\n");
			printf("                                         ��                                                                ��\n");
			printf("                                         ��                                                                ��\n");
			printf("                                         ��                      -> 2.��������                             ��\n");
			printf("                                         ��                                                                ��\n");
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
				if (selectedOption == 0) { selectedOption = 2; }
			}
			if (key == DOWN) {
				selectedOption++;
				if (selectedOption == 3) { selectedOption = 1; }
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
			printf("                                         ��                        �ε� �� 30%% !! .                          \n");
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
			printf("                                         ��                        �ε� �� 70%% !! . .                      \n");
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
			printf("                                         ��                        �ε� �� 100%% !! . . .                   ��\n");
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

int Level2LoadingPage() {
	cls(WHITE, BLACK);
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
		printf("	                                                        �� 2�ܰ� ȭ������ �Ѿ�� ���Դϴ� !! ��\n");

		textcolor(YELLOW1, 0);
		if (selectedOption == 1) {
			printf("                                         �ڡڡڡڡڡڡڡڡڡڡڡڡ�   Loading  \n");
			printf("                                         ��                                                                \n");
			printf("                                         ��                                                                \n");
			printf("                                         ��                                                                \n");
			printf("                                         ��                                                                \n");
			printf("                                         ��                                                                \n");
			printf("                                         ��                        �ε� �� 30%% !! .                          \n");
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
			printf("                                         ��                        �ε� �� 70%% !! . .                      \n");
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
			printf("                                         ��                        �ε� �� 100%% !! . . .                   ��\n");
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

int Level3LoadingPage() {
	cls(WHITE, BLACK);
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
		printf("	                                                        �� 3�ܰ� ȭ������ �Ѿ�� ���Դϴ� !! ��\n");

		textcolor(YELLOW1, 0);
		if (selectedOption == 1) {
			printf("                                         �ڡڡڡڡڡڡڡڡڡڡڡڡ�   Loading  \n");
			printf("                                         ��                                                                \n");
			printf("                                         ��                                                                \n");
			printf("                                         ��                                                                \n");
			printf("                                         ��                                                                \n");
			printf("                                         ��                                                                \n");
			printf("                                         ��                        �ε� �� 30%% !! .                          \n");
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
			printf("                                         ��                        �ε� �� 70%% !! . .                      \n");
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
			printf("                                         ��                        �ε� �� 100%% !! . . .                   ��\n");
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


int BossLoadingPage() {
	cls(WHITE, BLACK);
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
		printf("	                                                        �� ���� ���������� �Ѿ�� ���Դϴ� !! ��\n");

		textcolor(YELLOW1, 0);
		if (selectedOption == 1) {
			printf("                                         �ڡڡڡڡڡڡڡڡڡڡڡڡ�   Loading  \n");
			printf("                                         ��                                                                \n");
			printf("                                         ��                                                                \n");
			printf("                                         ��                                                                \n");
			printf("                                         ��                                                                \n");
			printf("                                         ��                                                                \n");
			printf("                                         ��                        �ε� �� 30%% !! .                          \n");
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
			printf("                                         ��                        �ε� �� 70%% !! . .                      \n");
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
			printf("                                         ��                        �ε� �� 100%% !! . . .                   ��\n");
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
		Sleep(300);
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
					// ù ��° �ɼ� ó��
					pickMyLevelPage = false;
					cls(WHITE, BLACK); // ���� ȭ�� �����
					PickMyLevel = 1;
					IsEasy = true; // ���̵� ���� 
					IsHard = false; // ���̵� �ϵ�
					break;
				case 2:
					pickMyLevelPage = false;
					cls(WHITE, BLACK); // ���� ȭ�� �����
					PickMyLevel = 2;
					IsEasy = false; // ���̵� ���� 
					IsHard = true; // ���̵� �ϵ�
					
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




	//���� ����â

void info() {
		//textcolor(YELLOW2, BLACK);
		textcolor(WHITE, BLACK);

		//int minutes = remainingTime / 60;
		//int seconds = remainingTime % 60;

		//displayTime(minutes, seconds);



		gotoxy(122, 5); printf("����: %d", score);
		gotoxy(122, 10);
		printf("���:");
		textcolor(RED1, BLACK);
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

		if (IsBoss) {
			gotoxy(2, 2);
			printf("���� HP:");


			textcolor(RED1, BLACK);
			if (BossLife >= 40) {

				gotoxy(11, 2); printf("       ");

				gotoxy(11, 2); printf("��������");
			}
			else if (BossLife == 30) {
				gotoxy(11, 2); printf("       ");
				gotoxy(11, 2); printf("������");
			}
			else if (BossLife == 20) {
				gotoxy(11, 2); printf("       ");
				gotoxy(11, 2); printf("����");
			}
			else if (BossLife == 1) {
				gotoxy(11, 2); printf("       ");
				gotoxy(11, 2); printf("��");
			}
			else {
				gotoxy(11, 2);
				printf("  ");
				//���� ����â ����?
			}


		}


		if (IsEasy) {
			textcolor(GREEN1, BLACK);
			gotoxy(122, 13); printf("���̵�: �������");
		}

		if (IsHard) {
			textcolor(RED1, BLACK);
			gotoxy(122, 13); printf("���̵�: �ϵ���");
		}

		if (IsLevel1) {
			textcolor(YELLOW1, BLACK);
			gotoxy(122, 15); printf("����ܰ�: 1�ܰ�");
		}

		if (IsLevel2) {
			textcolor(YELLOW1, BLACK);
			gotoxy(122, 15); printf("����ܰ�: 2�ܰ�");
		}
		if (IsLevel3) {
			textcolor(YELLOW1, BLACK);
			gotoxy(122, 15); printf("����ܰ�: 3�ܰ�");
		}

		if (IsBoss) {
			textcolor(YELLOW1, BLACK);
			gotoxy(122, 15); printf("����ܰ�: ���� ��������");
		}

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
void MoveBullet3() {
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
			if (Bullet[i].y <= 2) { // y ���� 3 ������ ��
				Bullet[i].y = -1; // �Ѿ��� ����� ���¸� ��Ÿ���� ���� y ���� -1�� ����
				Bullet[i].exist = FALSE;
			}
			else {
				DrawBullet(i);
				if (CheckBulletBossCollision(i)) {
					Bullet[i].exist = FALSE;
					score += 20;
					//Boss �׸� !!
					BossDraw2(Boss.x, Boss.y);
					BossLife--;
					info();
				}
			}
		}
	}

	Sleep(30);
}






// ������!!
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

// ������!!
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

//basic�� ���� ������ 1�ܰ�!!
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

// 2�ܰ� ���� ���� ������ 
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
			printf("      "); // ���� ��ġ�� �� ����

			// �¿�� �����̱� ���� ������ ����
			int moveDirection = rand() % 3; // 0, 1, 2 �� �ϳ��� ���� �������� ����

			switch (moveDirection) {
			case 0:
				Enemy[i].x -= 1; // �������� �̵�
				break;
			case 1:
				Enemy[i].x += 1; // ���������� �̵�
				break;
			default:
				// �������� ����
				break;
			}

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

void MoveBoss() {
	if (Boss.exist == TRUE) {
		int randomMove = rand() % 3; // 0, 1, 2 �߿��� �������� ����
		switch (randomMove) {
		case 0:
			Boss.move = -1; // �������� �̵�
			break;
		case 1:
			Boss.move = 0; // ������ ����
			break;
		case 2:
			Boss.move = 1; // ���������� �̵�
			break;
		}

		Boss.x += Boss.move;

		// ������ ���� ���� ����� ��
		if (Boss.x <= 3) {
			Boss.x = 3;
		}

		// ������ ������ ���� ����� ��
		if (Boss.x >= WIDTH - 40) {
			Boss.x = WIDTH - 40;
		}

		gotoxy(Boss.x - Boss.move, Boss.y);
		printf("            "); // ���� ��ġ �����

		gotoxy(Boss.x, Boss.y);
		textcolor(RED1, BLACK);
		BossDraw(Boss.x, Boss.y); // ���ο� ��ġ�� ���� �׸���
	}
}

//���Ѿ�
void EnemyBulletshow() {
	if (PickMyLevel == 1) {
		textcolor(GREEN1, BLACK);
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
void BossBulletshow() {
	if (PickMyLevel == 1) {
		textcolor(GREEN1, BLACK);
		gotoxy(0, 0); printf("��");
	}
	if (PickMyLevel == 2) {
		textcolor(RED1, BLACK);
		gotoxy(0, 0); printf("��");
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


//�Ѿ� �׸���
void EnemyBulletdraw(int i) {
	textcolor(RED2, BLACK);
	gotoxy(EnemyBullet[i].x, EnemyBullet[i].y);
	printf("��");
}

// ���Ѿ� �����
void EnemyBulleterase(int i) {
	gotoxy(EnemyBullet[i].x, EnemyBullet[i].y);
	printf("  ");
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




// ���� �� �Ѿ˿� �¾��� ��
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
			Zet10(newx, newy);
			Sleep(20);

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
		if (Enemy[i].y == Bullet[i].y && abs(Enemy[i].x - Bullet[i].x) <= 15) {
			gotoxy(Bullet[i].x, Bullet[i].y);
			printf("   ");
			Bullet[i].exist = FALSE;
			Enemy[i].type = -1;
			gotoxy(Enemy[i].x+2, Bullet[i].y);
			textcolor(BLUE2, BLACK);
			printf("��+20");
			Sleep(20);
			printf("  ");
			score += 20;
			info();
			break;
		}
	}
	
}

// ������ �� �Ѿ˿� �¾��� �� ��Ȳ �����ϱ�
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
		textcolor(GREEN1, BLACK);
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
//���� LEVEL 1 �ܰ� ȭ�� 
void Level1gamestart() {
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
	int enemySpawnInterval = 10;  // �� ���� ���� ����  2�ܰ� ���̵� �������ֱ�
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

		// ������ 200 ���� ũ�� ���� �ܰ�� �Ѿ�� ���ؼ� ���Ƴ���
			if (score < 100) {
				//�Ѿ� �ӵ����� �Լ�
				MoveBullet();


				// �� ����
				if (enemySpawnTimer >= enemySpawnInterval) {
					CreateBasicEnemy(3);  // 1�ܰ�ϱ� �� ���� �پ��ϰ� ���� �ʴ´�. 
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
				//playerTouch();


				DeleteEnemy();// ���� �� �Ѿ˿� �¾� �״� ���� ó��
				EnemyTouch(); // ���� ���� ������ ��  ����� -1 
			}
			
			if (score >= 100) {

				Level1ClearMessage(60,10);
				break;
			}
		
			
			// ���� ���� ǥ��
			


			Sleep(Delay); // Delay ���� ���̰�
			frame_count++;// frame_count ������ �ӵ� ������ �Ѵ�.
		}
}


// �켱 ������ ������ �Ǹ� �츮�� ��� Ű���带 Ȱ���� ���̴� while���� ���� �����غ���
//���� LEVEL 2 �ܰ� ȭ�� 
void Level2gamestart() {
	cls(WHITE, BLACK);
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
	int enemySpawnInterval = 5;  // �� ���� ���� ����
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

		// ������ 200 ���� ũ�� ���� �ܰ�� �Ѿ�� ���ؼ� ���Ƴ���
		if (score < 200) {
			//�Ѿ� �ӵ����� �Լ�
			MoveBullet();


			// �� ����
			if (enemySpawnTimer >= enemySpawnInterval) {
				CreateBasicEnemy(7); // 2�ܰ�ϱ� �� ���� �پ��ϰ� ���ֱ� 
				enemySpawnTimer = 0;
			}
			else {
				enemySpawnTimer++;
			}

			// �� ������ 2�ܰ�� ó�� ����!
			MoveEnemy3();



			// �� �Ѿ� ���� �� ������ ó��
			//EnemyBulletMove();
			//EnemyBulletshow();

			// �� �Ѿ˿� �¾��� �� ó��
			//playerTouch();


			DeleteEnemy();// ���� �� �Ѿ˿� �¾� �״� ���� ó��
			EnemyTouch(); // ���� ���� ������ ��  ����� -1 
		}

		// 3�ܰ�� �Ѿ�� 
		if (score >= 200) {

			Level2ClearMessage(60, 10); // ���� 2�ܰ� Ŭ���� �޽��� �����
			break;
		}


		// ���� ���� ǥ��



		Sleep(Delay); // Delay ���� ���̰�
		frame_count++;// frame_count ������ �ӵ� ������ �Ѵ�.
	}
}


// �켱 ������ ������ �Ǹ� �츮�� ��� Ű���带 Ȱ���� ���̴� while���� ���� �����غ���
//���� LEVEL 3 �ܰ� ȭ�� 
void Level3gamestart() {
	cls(WHITE, BLACK);
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
	int enemySpawnInterval = 5;  // �� ���� ���� ����
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

		// ������ 200 ���� ũ�� ���� �ܰ�� �Ѿ�� ���ؼ� ���Ƴ���
		if (score < 360) {
			//�Ѿ� �ӵ����� �Լ�
			MoveBullet();


			// �� ����
			if (enemySpawnTimer >= enemySpawnInterval) {
				CreateBasicEnemy(10); // 3�ܰ�ϱ� �� ���� �پ��ϰ� ���ֱ� 
				enemySpawnTimer = 0;
			}
			else {
				enemySpawnTimer++;
			}

			// �� ������ 3�ܰ�� ó�� ����!
			MoveEnemy();



			// �� �Ѿ� ���� �� ������ ó��
			EnemyBulletMove();
			EnemyBulletshow();

			// �� �Ѿ˿� �¾��� �� ó��
			playerTouch();


			DeleteEnemy();// ���� �� �Ѿ˿� �¾� �״� ���� ó��
			EnemyTouch(); // ���� ���� ������ ��  ����� -1 
		}

		// 3�ܰ�� �Ѿ�� 
		if (score >= 360) {

			Level3ClearMessage(60, 10); // ���� 2�ܰ� Ŭ���� �޽��� �����
			break;
		}


		// ���� ���� ǥ��



		Sleep(Delay); // Delay ���� ���̰�
		frame_count++;// frame_count ������ �ӵ� ������ �Ѵ�.
	}
}


// �켱 ������ ������ �Ǹ� �츮�� ��� Ű���带 Ȱ���� ���̴� while���� ���� �����غ���
//���� ���� �ܰ� ȭ�� 
void Bossgamestart() {
	cls(WHITE, BLACK);
	textcolor(GREEN1, BLACK);
	Boss.x = 60;
	Boss.y = 3;
	gotoxy(60, 17);

	printf("������ �� �����ؿ�!!!!!!!!!!!!!!!");
	
	Sleep(1000);
	cls(WHITE, BLACK);

	unsigned char ch;
	int i;
	int startTime = time(NULL); // ���� ���� �ð� (��)
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
	//BossDraw(60,11);
	info(); // �� ���� ���� �����Ȳ !! (while���ȿ� ������ ���ܺ��̱� ������ ��������� ���� ���� ȣ���ϰ� �س����鼭 ���� ������ ���߾���.)
	// �� ���� ���� ����
	int enemySpawnTimer = 0;
	int enemySpawnInterval = 5;  // �� ���� ���� ����
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

		// ������ 200 ���� ũ�� ���� �ܰ�� �Ѿ�� ���ؼ� ���Ƴ���
		if (BossLife != 0 ) {
			//�Ѿ� �ӵ����� �Լ�
			MoveBullet();
			//MoveBoss();

			// ���� ������
			if (frame_count % 10 == 0) {
				int moveDirection = rand() % 2; // 0 �Ǵ� 1 �� �ϳ��� ���� �������� ����

				switch (moveDirection) {
				case 0:
					BossErase(Boss.x, Boss.y); // ���� ��ġ �����
					Boss.x--; // �������� �̵�
					BossDraw(Boss.x, Boss.y); // ���ο� ��ġ�� ���� �׸���
					break;
				case 1:
					BossErase(Boss.x, Boss.y); // ���� ��ġ �����
					Boss.x++; // ���������� �̵�
					BossDraw(Boss.x, Boss.y); // ���ο� ��ġ�� ���� �׸���
					break;
				}
			}
			
			//DeleteBoss();
			//// �� ����
			if (enemySpawnTimer >= enemySpawnInterval) {
				CreateBasicEnemy2(10); // 3�ܰ�ϱ� �� ���� �پ��ϰ� ���ֱ� 
				enemySpawnTimer = 0;
			}
			else {
				enemySpawnTimer++;
			}

			//// �� ������ 3�ܰ�� ó�� ����!
			MoveEnemy();

			//

			//// �� �Ѿ� ���� �� ������ ó��
			

			EnemyBulletshow(); // �� �Ѿ� ��ǥ ����

			EnemyBulletMove(); // �� �Ѿ� �׸��� ����� 

			BossBulletshow(); // ���� �Ѿ� ��ǥ ����

			//BossBulletMove(); // ���� �Ѿ� �׸��� ����� 
			//// �� �Ѿ˿� �¾��� �� ó��
			playerTouch();


			DeleteEnemy();// ���� �� �Ѿ˿� �¾� �״� ���� ó��
			EnemyTouch(); // ���� ���� ������ ��  ����� -1 
		}

		// ������ �ǰ� ���ٸ�
		if (BossLife <= 0) {

			BossClearMessage(60, 10); // �����ܰ� Ŭ���� �޽���!!
			break;
		}

		int currentTime = time(NULL); // ���� �ð�
		int elapsedSeconds = currentTime - startTime; // ��� �ð� (��)
		int remainingSeconds = remainingTime - elapsedSeconds; // ���� �ð� (��)
		int minutes = remainingSeconds / 60; // ��
		int seconds = remainingSeconds % 60; // ��

		displayTime(minutes, seconds);

		// ���ѽð��� ������ 
		if (remainingSeconds <= 0) {
			BossClearMessage(60, 10); // �����ܰ� Ŭ���� �޽���!!
			break;
		}

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
			
		
			//pickMyJet(); // ����⸦ ����!!
			//
			//pickGameLevel();// ���̵� ���� ȭ�� 
			//LoadingPage();// �ε� ȭ�� !!

			//IsLevel1 = true; // 1�ܰ�
			//Level1gamestart(); // 1�ܰ� ���� ����!!
			//IsLevel1 = false; // 1�ܰ�

			//Level2LoadingPage();	// 2�ܰ� �ε� ������ 
			//IsLevel2 = true; // 2�ܰ�
			//Level2gamestart();      // 2�ܰ� ���� ����
			//IsLevel2 = false; // 2�ܰ�
			//Level3LoadingPage();	// 3�ܰ� �ε� ������ 
			//IsLevel3 = true; // 3�ܰ�
			//Level3gamestart();      // 3�ܰ� ���� ����
			//IsLevel3 = false; // 3�ܰ�
			//BossLoadingPage(); // ������ ���� �ε� ������
			IsBoss = true;
			cls(WHITE, BLACK);
			Bossgamestart(); // ���� �ܰ� ���� ����!!
			IsBoss = false;
			IntroPage();
			if (FinishPage() == 2) {
				//isGameRunning = false;
				isFinish = TRUE;
			};
			isGameRunning = false;
			//Ŭ���� â!! �ٽ��� �� ���� ���ϴ� â 


		
		
			
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

