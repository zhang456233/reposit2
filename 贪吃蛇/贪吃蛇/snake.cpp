/*
  课程内容：贪吃蛇
  文件名称：snake,cpp
  知识点：结构体 控制语句 循环 +easyx图形库
*/
#include<stdio.h>
#include<conio.h>
#include <graphics.h>//图形库的头文件
#define WIN_WIDTH 640 //窗口宽高
#define WIN_HEIGHT 480
#define MAX_SNAKE 100
#include<mmsystem.h>//播放音乐头文件
#pragma comment(lib,"winmm.lib")//告诉编译器，加载winmm. lib库文件
//easyx安装好了，不显示图片，程序能正常运行，只有两种问题
//1.路径  2.图片，自己改后缀

enum DIRECTION //用枚举法表示蛇的方向
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
};

struct Snake //蛇的结构体
{
	int num;//蛇的节数
	int direction;//方向
	int score;//分数
	int size;//蛇的宽度和高度
	POINT coor[MAX_SNAKE];
}snake;

//定义一个食物的结构体
struct Food
{
	POINT fd;
	int flag;
	DWORD color;//随机颜色
}food;

void GameInit()//数据初始化
{
	srand(GetTickCount());//将开机时间作为随机数种子
	//初始化蛇
	snake.num = 3;
	snake.direction = RIGHT;
	snake.score = 0;
	snake.size = 10;
	snake.coor[2].x = 0;
	snake.coor[2].y = 0;
	snake.coor[1].x = snake.size;
	snake.coor[1].y = 0;
	snake.coor[0].x = 2 * snake.size;
	snake.coor[0].y = 0;

	//初始化食物
	food.fd.x = rand() % (WIN_WIDTH/10)*10;//让食物的坐标是10的倍数
	food.fd.y = rand() % (WIN_HEIGHT/10)*10;
	food.flag = 1;//食物存活
	food.color = RGB(rand() % 256, rand() % 256, rand() % 256);

	//放音乐
	mciSendString("open 金志文-写给初恋的一封信.mp3 alias start",0,0,0);//打开音频文件，并给文件取别名
	mciSendString("play start repeat", 0, 0, 0);//重复播放音乐
}

void GameDraw()//画蛇
{
	setbkcolor(RGB(211, 190, 239));//设置背景颜色
	cleardevice();// 清屏

	//画蛇
	for (int i = 0; i < snake.num; i++)
	{
		setlinecolor(BLACK);//设置线的颜色
		setfillcolor(YELLOW);// 设置当前填充颜色
		fillrectangle(snake.coor[i].x, snake.coor[i].y, snake.coor[i].x+snake.size, snake.coor[i].y + snake.size);// 画填充矩形(有边框)
		printf("%d  %d\n", snake.coor[i].x, snake.coor[i].y);
	}
	

	//画食物
	setfillcolor(food.color);//设置填充颜色
	fillellipse(food.fd.x, food.fd.y, food.fd.x+10, food.fd.y+10);//画个填充的椭圆

	//显示分数
	char temp[20] = "";
	sprintf(temp, "分数：%d", snake.score);//将数值转化为字符串
	
	setbkmode(TRANSPARENT);// 设置背景颜色为透明
	outtextxy(20, 20, temp);//在屏幕上显示分数
}

void SnakeMove()//蛇运动
{
	for (int i = snake.num - 1; i > 0;i--)//从最后一节开始，每一节蛇都等于上一节蛇的上一次坐标
	{
		snake.coor[i].x = snake.coor[i - 1].x;
		snake.coor[i].y = snake.coor[i - 1].y;
	}

	switch (snake.direction)//当键盘方向不同时，贪吃蛇可以用键盘控制
	{
	case UP:
		snake.coor[0].y -= 10;//蛇头往上移动
		if (snake.coor[0].y+10 <= 0)
		{
			snake.coor[0].y = WIN_HEIGHT;//当到达上边界时，从下边界出来
		}
		break;
	case DOWN:
		snake.coor[0].y += 10;
		if (snake.coor[0].y  >= WIN_HEIGHT)//当到达下边界时，从上边界出来
		{
			snake.coor[0].y = 0;
		}
		break;
	case LEFT:
		snake.coor[0].x -= 10;
		if (snake.coor[0].x+10 <= 0)//当到达左边界时，从右边界出来
		{
			snake.coor[0].x = WIN_WIDTH;
		}
		break;
	case RIGHT:
		snake.coor[0].x += 10;
		if (snake.coor[0].x >= WIN_WIDTH)//当到达右边界时，从左边界出来
		{
			snake.coor[0].x = 0;
		}
		break;
	default:
		break;
	}
	//snake.coor[0].x += 10;
}

void KeyControl()//使用键盘控制
{
	//使用win32API获取键盘消息
	if (GetAsyncKeyState(VK_UP)&&snake.direction!=DOWN)//获取键盘状态
	{
		snake.direction = UP;
	}
	if (GetAsyncKeyState(VK_DOWN)&&snake.direction != UP)//解决贪吃蛇能掉头的问题
	{
		snake.direction = DOWN;
	}
	if (GetAsyncKeyState(VK_LEFT) && snake.direction != RIGHT)//获取键盘状态
	{
		snake.direction = LEFT;
	}
	if (GetAsyncKeyState(VK_RIGHT) && snake.direction != LEFT)//获取键盘状态
	{
		snake.direction = RIGHT;
	}
}

void EatFood()
{
	//如果蛇头的坐标与食物的坐标相同，且食物存在
	if (snake.coor[0].x == food.fd.x&&snake.coor[0].y == food.fd.y&&food.flag == 1)
	{
		snake.num++;//蛇的节数加1
		snake.score++;//分数加1
		food.flag = 0;//食物不存在
	}
	if (food.flag == 0)
	{
		food.fd.x = rand() % (WIN_WIDTH / 10) * 10;
		food.fd.y = rand() % (WIN_HEIGHT / 10) * 10;
		food.flag = 1;//食物存活
		food.color = RGB(rand() % 256, rand() % 256, rand() % 256);
	}
}

//蛇不能吃自己，也就是食物不能产生到蛇的身上
//游戏结束判断
void DontEatSelf()
{
	for (int i = 1; i < snake.num; i++)
	{
		if (snake.coor[0].x == snake.coor[i].x&&snake.coor[0].y == snake.coor[i].y)
		{
			outtextxy(200, 200, "Game Over!");
			_getch();//头文件为conio.h，无回显的在控制台获得一个字符
			exit(0);
		}
	}
}

//游戏暂停    有问题
//void PauseGame()
//{
//	char ch = _getch();
//	if (ch == 32)
//	{
//		while (1)
//		{
//			ch = _getch();
//			if (ch != 32)
//				break;
//		}
//	}
//}
int main()
{
	initgraph(WIN_WIDTH, WIN_HEIGHT,SHOWCONSOLE);//初始化一个窗口,SHOWCONSOLE是显示控制台的意思
	GameInit();


	DWORD t1, t2;//定义两个随时间变化的时间变量
	t1 = t2 = GetTickCount();//GetTickCount()返回开机以来经过的毫秒数

	//BeginBatchDraw();//利用双缓冲技术防止卡顿  // 开始批量绘制
	while (1)
	{
		if (t2 - t1 > 100)
		{
			SnakeMove();
			t1 = t2;//上一次的终止值变为新的起始值
		}
		t2 = GetTickCount();
		
		GameDraw();

		//FlushBatchDraw();//利用双缓冲技术防止卡顿  // 执行未完成的绘制任务
		KeyControl();
		EatFood();
		DontEatSelf();
		//PauseGame();
		//Sleep(50);
	}
	

	//程序一闪而过怎么回事？程序运行结束，关闭窗口不是很正常吗？
	getchar();//让窗口停留

	closegraph();//关闭图形窗口
	return 0;
}