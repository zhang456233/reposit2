/*
  �γ����ݣ�̰����
  �ļ����ƣ�snake,cpp
  ֪ʶ�㣺�ṹ�� ������� ѭ�� +easyxͼ�ο�
*/
#include<stdio.h>
#include<conio.h>
#include <graphics.h>//ͼ�ο��ͷ�ļ�
#define WIN_WIDTH 640 //���ڿ��
#define WIN_HEIGHT 480
#define MAX_SNAKE 100
#include<mmsystem.h>//��������ͷ�ļ�
#pragma comment(lib,"winmm.lib")//���߱�����������winmm. lib���ļ�
//easyx��װ���ˣ�����ʾͼƬ���������������У�ֻ����������
//1.·��  2.ͼƬ���Լ��ĺ�׺

enum DIRECTION //��ö�ٷ���ʾ�ߵķ���
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
};

struct Snake //�ߵĽṹ��
{
	int num;//�ߵĽ���
	int direction;//����
	int score;//����
	int size;//�ߵĿ�Ⱥ͸߶�
	POINT coor[MAX_SNAKE];
}snake;

//����һ��ʳ��Ľṹ��
struct Food
{
	POINT fd;
	int flag;
	DWORD color;//�����ɫ
}food;

void GameInit()//���ݳ�ʼ��
{
	srand(GetTickCount());//������ʱ����Ϊ���������
	//��ʼ����
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

	//��ʼ��ʳ��
	food.fd.x = rand() % (WIN_WIDTH/10)*10;//��ʳ���������10�ı���
	food.fd.y = rand() % (WIN_HEIGHT/10)*10;
	food.flag = 1;//ʳ����
	food.color = RGB(rand() % 256, rand() % 256, rand() % 256);

	//������
	mciSendString("open ��־��-д��������һ����.mp3 alias start",0,0,0);//����Ƶ�ļ��������ļ�ȡ����
	mciSendString("play start repeat", 0, 0, 0);//�ظ���������
}

void GameDraw()//����
{
	setbkcolor(RGB(211, 190, 239));//���ñ�����ɫ
	cleardevice();// ����

	//����
	for (int i = 0; i < snake.num; i++)
	{
		setlinecolor(BLACK);//�����ߵ���ɫ
		setfillcolor(YELLOW);// ���õ�ǰ�����ɫ
		fillrectangle(snake.coor[i].x, snake.coor[i].y, snake.coor[i].x+snake.size, snake.coor[i].y + snake.size);// ��������(�б߿�)
		printf("%d  %d\n", snake.coor[i].x, snake.coor[i].y);
	}
	

	//��ʳ��
	setfillcolor(food.color);//���������ɫ
	fillellipse(food.fd.x, food.fd.y, food.fd.x+10, food.fd.y+10);//����������Բ

	//��ʾ����
	char temp[20] = "";
	sprintf(temp, "������%d", snake.score);//����ֵת��Ϊ�ַ���
	
	setbkmode(TRANSPARENT);// ���ñ�����ɫΪ͸��
	outtextxy(20, 20, temp);//����Ļ����ʾ����
}

void SnakeMove()//���˶�
{
	for (int i = snake.num - 1; i > 0;i--)//�����һ�ڿ�ʼ��ÿһ���߶�������һ���ߵ���һ������
	{
		snake.coor[i].x = snake.coor[i - 1].x;
		snake.coor[i].y = snake.coor[i - 1].y;
	}

	switch (snake.direction)//�����̷���ͬʱ��̰���߿����ü��̿���
	{
	case UP:
		snake.coor[0].y -= 10;//��ͷ�����ƶ�
		if (snake.coor[0].y+10 <= 0)
		{
			snake.coor[0].y = WIN_HEIGHT;//�������ϱ߽�ʱ�����±߽����
		}
		break;
	case DOWN:
		snake.coor[0].y += 10;
		if (snake.coor[0].y  >= WIN_HEIGHT)//�������±߽�ʱ�����ϱ߽����
		{
			snake.coor[0].y = 0;
		}
		break;
	case LEFT:
		snake.coor[0].x -= 10;
		if (snake.coor[0].x+10 <= 0)//��������߽�ʱ�����ұ߽����
		{
			snake.coor[0].x = WIN_WIDTH;
		}
		break;
	case RIGHT:
		snake.coor[0].x += 10;
		if (snake.coor[0].x >= WIN_WIDTH)//�������ұ߽�ʱ������߽����
		{
			snake.coor[0].x = 0;
		}
		break;
	default:
		break;
	}
	//snake.coor[0].x += 10;
}

void KeyControl()//ʹ�ü��̿���
{
	//ʹ��win32API��ȡ������Ϣ
	if (GetAsyncKeyState(VK_UP)&&snake.direction!=DOWN)//��ȡ����״̬
	{
		snake.direction = UP;
	}
	if (GetAsyncKeyState(VK_DOWN)&&snake.direction != UP)//���̰�����ܵ�ͷ������
	{
		snake.direction = DOWN;
	}
	if (GetAsyncKeyState(VK_LEFT) && snake.direction != RIGHT)//��ȡ����״̬
	{
		snake.direction = LEFT;
	}
	if (GetAsyncKeyState(VK_RIGHT) && snake.direction != LEFT)//��ȡ����״̬
	{
		snake.direction = RIGHT;
	}
}

void EatFood()
{
	//�����ͷ��������ʳ���������ͬ����ʳ�����
	if (snake.coor[0].x == food.fd.x&&snake.coor[0].y == food.fd.y&&food.flag == 1)
	{
		snake.num++;//�ߵĽ�����1
		snake.score++;//������1
		food.flag = 0;//ʳ�ﲻ����
	}
	if (food.flag == 0)
	{
		food.fd.x = rand() % (WIN_WIDTH / 10) * 10;
		food.fd.y = rand() % (WIN_HEIGHT / 10) * 10;
		food.flag = 1;//ʳ����
		food.color = RGB(rand() % 256, rand() % 256, rand() % 256);
	}
}

//�߲��ܳ��Լ���Ҳ����ʳ�ﲻ�ܲ������ߵ�����
//��Ϸ�����ж�
void DontEatSelf()
{
	for (int i = 1; i < snake.num; i++)
	{
		if (snake.coor[0].x == snake.coor[i].x&&snake.coor[0].y == snake.coor[i].y)
		{
			outtextxy(200, 200, "Game Over!");
			_getch();//ͷ�ļ�Ϊconio.h���޻��Ե��ڿ���̨���һ���ַ�
			exit(0);
		}
	}
}

//��Ϸ��ͣ    ������
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
	initgraph(WIN_WIDTH, WIN_HEIGHT,SHOWCONSOLE);//��ʼ��һ������,SHOWCONSOLE����ʾ����̨����˼
	GameInit();


	DWORD t1, t2;//����������ʱ��仯��ʱ�����
	t1 = t2 = GetTickCount();//GetTickCount()���ؿ������������ĺ�����

	//BeginBatchDraw();//����˫���弼����ֹ����  // ��ʼ��������
	while (1)
	{
		if (t2 - t1 > 100)
		{
			SnakeMove();
			t1 = t2;//��һ�ε���ֵֹ��Ϊ�µ���ʼֵ
		}
		t2 = GetTickCount();
		
		GameDraw();

		//FlushBatchDraw();//����˫���弼����ֹ����  // ִ��δ��ɵĻ�������
		KeyControl();
		EatFood();
		DontEatSelf();
		//PauseGame();
		//Sleep(50);
	}
	

	//����һ��������ô���£��������н������رմ��ڲ��Ǻ�������
	getchar();//�ô���ͣ��

	closegraph();//�ر�ͼ�δ���
	return 0;
}