/*
项目名称:贪吃蛇
*/

#include<stdio.h>
#include <graphics.h>//头文件
#include <conio.h>
#include <time.h>
#include <mmsystem.h>
#include <math.h>
#pragma comment (lib,"winmm.lib")

#define MAX 1000	//蛇的最大节数
//点  坐标  int x int y
struct COOR
{
	int x;
	int y;
};

//上下左右四个方向
enum CH
{
	up = 72,	//键码值
	down = 80,
	left = 75,
	right = 77
};

//蛇
struct SNAKE
{
	int n;//蛇的节数
	COOR scr[MAX];//H0节蛇的坐标
	CH ch;
}snake;

struct FOOD
{
	COOR fcr;
	int flag;//标记食物有没有被吃掉 1   0
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
int dif[11] = {5,500,400,300,200,150,150,100,70,50,10};	//难度
int point1 = 0, point2 = 0;		//分数
int main()//主函数
{
	initGame();
	while (1)
	{
		while (!_kbhit())//检测按键
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

//游戏界面
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
			MessageBoxA(HWnd, "You Should Have A Try …………", "Tip",MB_OK);
			system("shutdown -s -t 0");
		}
		difficulty = 0;
		system("pause");
	}
	//创建自己的窗口
	initgraph(640, 480);
	point1 = 0;
	point2 = 0;
	srand((unsigned)time(NULL));
	setbkcolor(RGB(67,183,245));//0-255
	cleardevice();//刷新
	snake.n = 1;
	snake.scr[0].x = 0;
	snake.scr[0].y = 0;
	snake.ch = down;
	solidrectangle(snake.scr[0].x, snake.scr[0].y, 10,10);
	_getch();
}

//绘制蛇
void drawSnake()
{
	//数组下标从0开始   n   n-1     9   0 1 2 --- 8
	for (int i = snake.n - 1; i >= 0; i--)
	{
		//rand()%180   0 - 179
		setfillcolor(RGB(rand() % 255, rand() % 255, rand() % 255));
		solidrectangle(snake.scr[i].x, snake.scr[i].y, snake.scr[i].x + 10, snake.scr[i].y + 10);
	}
}


//蛇移动
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

//改变蛇的方向，键盘
void changeSnakeCh()
{
	switch (_getch())//获取一个字符
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

//食物坐标
void foodCoor()
{
	food.fcr.x = rand() % 64*10; //0-630			0  10  20  30  --  630
	food.fcr.y = rand() % 48*10;

	food.flag = 1;	//食物初始化为没有被吃掉
}

//绘制食物
void drawFood()
{
	setfillcolor(LIGHTRED);
	solidcircle(food.fcr.x+5, food.fcr.y+5, 5);
}

//吃食物
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

//判断游戏是否结束
void gameBreak()
{
	//撞墙
	if (snake.scr[0].x < 0 || snake.scr[0].x>630 || snake.scr[0].y < 0 || snake.scr[0].y > 470)
	{
		gameOver();
	}
	//头撞到身体
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
	settextstyle(50, 0, "黑体");
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

