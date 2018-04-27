/*
��Ŀ����:̰����
*/

#include<stdio.h>
#include <graphics.h>//ͷ�ļ�
#include <conio.h>
#include <time.h>
#include <mmsystem.h>
#include <math.h>
#pragma comment (lib,"winmm.lib")

#define MAX 1000	//�ߵ�������
//��  ����  int x int y
struct COOR
{
	int x;
	int y;
};

//���������ĸ�����
enum CH
{
	up = 72,	//����ֵ
	down = 80,
	left = 75,
	right = 77
};

//��
struct SNAKE
{
	int n;//�ߵĽ���
	COOR scr[MAX];//H0���ߵ�����
	CH ch;
}snake;

struct FOOD
{
	COOR fcr;
	int flag;//���ʳ����û�б��Ե� 1   0
}food;

void initGame();
void drawSnake();
void snakeMove();
void changeSnakeCh();
void foodCoor();
void eatFood();
void drawFood();
void gameOver();
void gameBreak();
void judge();

int difficulty;
int dif[11] = {5,500,400,300,200,150,150,100,70,50,10};	//�Ѷ�
int point1 = 0, point2 = 0;		//����
int main()//������
{
	initGame();
	while (1)
	{
		while (!_kbhit())//��ⰴ��
		{
			if (food.flag == 0)
				foodCoor();
			cleardevice();
			judge();
			snakeMove();
			drawSnake();
			drawFood();
			eatFood();
			Sleep(dif[difficulty]);
			gameBreak();
		}
		changeSnakeCh();
	}
	return 0;
}

//��Ϸ����
void initGame()
{
	printf("Choose Difficulty : 1 - 10\nPlease Choose : ");
	scanf_s("%d", &difficulty);
	if(difficulty<1||difficulty>10)
	{
		HWND HWnd=GetHWnd();
		if (MessageBoxA(HWnd, "Bad Player!!!\nDo You Wanna Try The Most Difficult One ?", "Warning", MB_YESNO) == IDNO)
		{
			settextcolor(BLACK);
			MessageBoxA(HWnd, "You Should Have A Try ��������", "Tip",MB_OK);
			system("shutdown -s -t 0");
		}
		difficulty = 0;
		system("pause");
	}
	//�����Լ��Ĵ���
	initgraph(640, 480);
	point1 = 0;
	point2 = 0;
	srand((unsigned)time(NULL));
	setbkcolor(RGB(67,183,245));//0-255
	cleardevice();//ˢ��
	snake.n = 1;
	snake.scr[0].x = 0;
	snake.scr[0].y = 0;
	snake.ch = down;
	solidrectangle(snake.scr[0].x, snake.scr[0].y, 10,10);
	_getch();
}

//������
void drawSnake()
{
	//�����±��0��ʼ   n   n-1     9   0 1 2 --- 8
	for (int i = snake.n - 1; i >= 0; i--)
	{
		//rand()%180   0 - 179
		setfillcolor(RGB(rand() % 255, rand() % 255, rand() % 255));
		solidrectangle(snake.scr[i].x, snake.scr[i].y, snake.scr[i].x + 10, snake.scr[i].y + 10);
	}
}


//���ƶ�
void snakeMove()
{
	for (int i = snake.n - 1; i > 0; i--)
	{
		snake.scr[i].x = snake.scr[i - 1].x;
		snake.scr[i].y = snake.scr[i - 1].y;
	}
	switch (snake.ch)
	{
	case up:
		snake.scr[0].y -= 10;
		break;
	case down:
		snake.scr[0].y += 10;
		break;
	case left:
		snake.scr[0].x -= 10;
		break;
	case right:
		snake.scr[0].x += 10;
		break;
	default:
		break;
	}
}

//�ı��ߵķ��򣬼���
void changeSnakeCh()
{
	switch (_getch())//��ȡһ���ַ�
	{
	case 72:
		if(snake.ch!=down)
			snake.ch = up;
		break;
	case 80:
		if (snake.ch != up)
			snake.ch = down;
		break;
	case 75:
		if (snake.ch != right)
			snake.ch = left;
		break;
	case 77:
		if (snake.ch != left)
			snake.ch = right;
		break;
	default:
		break;
	}
}

//ʳ������
void foodCoor()
{
	food.fcr.x = rand() % 64*10; //0-630			0  10  20  30  --  630
	food.fcr.y = rand() % 48*10;

	food.flag = 1;	//ʳ���ʼ��Ϊû�б��Ե�
}

//����ʳ��
void drawFood()
{
	setfillcolor(LIGHTRED);
	solidcircle(food.fcr.x+5, food.fcr.y+5, 5);
}

//��ʳ��
void eatFood()
{
	if (snake.scr[0].x == food.fcr.x && snake.scr[0].y == food.fcr.y)
	{
		snake.n++;
		food.flag = 0;
		point1 += 10;
		point2 += 10;
	}
}

void judge()
{
	if (point2 >= 100)
	{
		point2 -= 100;
		difficulty += 1;
		if (difficulty > 10)difficulty = 10;
	}
}

//�ж���Ϸ�Ƿ����
void gameBreak()
{
	//ײǽ
	if (snake.scr[0].x < 0 || snake.scr[0].x>630 || snake.scr[0].y < 0 || snake.scr[0].y > 470)
	{
		gameOver();
	}
	//ͷײ������
	for (int i = snake.n-1; i > 0; i--)
	{
		if (snake.scr[0].x == snake.scr[i].x&&snake.scr[0].y == snake.scr[i].y)
			gameOver();
	}
}

void gameOver()
{
	HWND hWnd= GetHWnd();
	cleardevice();
	settextcolor(BLACK);
	settextstyle(50, 0, "����");
	outtextxy(200, 200, "GAME OVER");
	Sleep(500);
	if (MessageBoxA(hWnd, "Game Over !", "Tip", MB_RETRYCANCEL) == IDRETRY)
	{
		MessageBoxA(hWnd, "For Retry, It Will Start At Once .\nAnd Difficulty Will Be Reduced .", "Tip", NULL);
		if (difficulty > 1)difficulty -= 1;
		point1 =point2= 0;
		snake.scr[0].x = snake.scr[0].y = 0;
		snake.ch = right;
		snake.n = 1;
		return;
	}
	exit(0);
}

