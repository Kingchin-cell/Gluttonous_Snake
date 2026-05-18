#include<iostream>
#include<Windows.h>//延时
#include<vector>
#include<stdlib.h>//随机数
#include<time.h>//用时间初始化随机数
#include<thread>//多线程
#include<conio.h>//按键监听
#include<fstream>//文件读写

#define Red "\033[31m"
#define Green "\033[32m"
#define White "\033[0m"
#define Row 20
#define Col 20

#define Space 0
#define Body 1
#define Apple 2
#define Wall 9

#define Up 0
#define Down 1
#define Left 2
#define Right 3

class Pos
{
public:
	int x;
	int y;
	Pos() {};
	Pos(int x, int y)
	{
		this->x = x;
		this->y = y;
	}
};
using namespace std;
vector<Pos> snake;//保存每段蛇体的位置
Pos applePos;//苹果生成的位置
int direction = Right;//当前蛇的方向
int mode = 300;//蛇移动的速度
int total_score = 0;//总积分！
char map[Row][Col] =//用char类型读写更快速！
{
	{9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9},
	{9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9},
	{9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9},
	{9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9},
	{9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9},
	{9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9},
	{9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9},
	{9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9},
	{9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9},
	{9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9},
	{9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9},
	{9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9},
	{9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9},
	{9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9},
	{9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9},
	{9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9},
	{9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9},
	{9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9},
	{9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9},
	{9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9}
};
char frame[Row][Col];//用于打印的缓存
void getScore();
void menu()
{
	getScore();
	cout << "---------------"<<Green<<"贪吃蛇"<<White<<"--------------- " << endl;
	cout << "欢迎来到贪吃蛇游戏  本游戏由"<<Red<<"柒"<<White<<"开发" << endl<<endl;
	cout << "游戏规则:" << endl;
	cout << "	 W A S D 控制移动" << endl;
	cout << "	 吃果子变长" << endl;
	cout << "	 碰到墙游戏结束" << endl;
	cout << "	 吃到苹果即可得分" << endl<<endl;
	cout << "当前总积分:" << total_score << endl;
	cout << "-----------享受你的游戏吧------------" << endl;
	cout << "选择难度" << endl;
	cout << "按下数字并回车以选择" << endl;
	cout << "1.简单" << endl;
	cout << "2.中等" << endl;
	cout << "3.困难" << endl;
	int select = 0;
	while (1)
	{
		cin >> select;
		switch (select)
		{
		case 1:
			mode = 200;
			cout << "您已选择难度：简单" << endl;
			goto tag;
		case 2:
			mode = 100;
			cout << "您已选择难度：中等" << endl;
			goto tag;
		case 3:
			mode = 50;
			cout << "您已选择难度：困难" << endl;
			goto tag;
		default:
			cout << "输入有误，请重新输入" << endl;
		}
	}
tag:;
	system("pause");
}
void test01()
{
	menu();
}
void putScore()
{
	ofstream ofs("score.txt");
	ofs << total_score;
	ofs.close();
}
void getScore()
{
	ifstream ifs("score.txt");
	if (ifs)
	{
		ifs >> total_score;
	}
	ifs.close();

}
void printMap()
{
	//地图复制到缓存中
	memcpy(frame, map, Row * Col);
	//蛇的数据放到缓存中
	for (int i = 0; i < snake.size(); i++)
	{
		Pos p = snake[i];
		frame[p.y][p.x] = Body;//记得要反着放
	}
	frame[applePos.y][applePos.x] = Apple;
	system("cls");
	for (int i = 0; i < Row; i++)
	{
		for (int j = 0; j < Col; j++)
		{
			switch (frame[i][j])
			{
			case Space:cout << "  "; break;
			case Body: cout <<Green<< "蛇"<<White; break;
			case Apple:cout << Red<<"果"<<White; break;
			case Wall: cout << "墙"; break;
			}
		}
		cout << endl;
	}
}
void randApple()
{
	while (1)
	{
		int x = rand() % Col;
		int y = rand() % Row;
		if (map[y][x] == Space)
		{
			for (int i = 0; i < snake.size(); i++)
			{
				Pos p = snake[i];
				if (p.x == x && p.y == y)
				{
					goto tag;
				}
			}
			applePos.x = x;
			applePos.y = y;
			break;
			tag:;
		}
	}
}
bool moveSnake()
{
	Pos next = snake[0];
	if (direction == Up) next.y--;
	if (direction == Down)next.y++;
	if (direction == Left)next.x--;
	if (direction == Right)next.x++;
	switch (frame[next.y][next.x])
	{
	case Space:
		snake.insert(snake.begin(), next);
		snake.pop_back();
		break;
	case Apple:
		snake.insert(snake.begin(), next);
		map[next.y][next.x] = Space;
		randApple();
		break;
	case Wall:
	case Body:
		return false;
	}
	return true;
}
void listenKey()
{
	while (1)
	{
		int key = _getch();
		Pos p1 = snake[0];
		Pos p2 = snake[1];
		switch (key)
		{
		case 'w':
			p1.y--;
			if (p1.x == p2.x && p1.y == p2.y)break;
			direction = Up;
			break;
		case 's':
			p1.y++;
			if (p1.x == p2.x && p1.y == p2.y)break;
			direction = Down;
			break;
		case 'a':
			p1.x--;
			if (p1.x == p2.x && p1.y == p2.y)break;
			direction = Left;
			break;
		case 'd':
			p1.x++;
			if (p1.x == p2.x && p1.y == p2.y)break;
			direction = Right;
			break;
		}
	}
}
int main()
{
	int score = 0;
	menu();
	srand(time(NULL));
	//初始化蛇体
	snake.push_back(Pos(5, 5));
	snake.push_back(Pos(4, 5));
	snake.push_back(Pos(3, 5));
	//启动线程
	thread t(listenKey);
	t.detach();
	randApple();
	while (1)
	{
		printMap();
		Sleep(mode);
		if (!moveSnake())
		{
			system("cls");
			//cout.flush();
			cout << endl << "游戏结束" << endl;
			score = snake.size() - 3;
			total_score += score;
			putScore();
			getScore();
			cout << "本次得分：" << score << endl;
			cout << "总得分：" << total_score << endl;
			break;
		}
	}
	//printMap();
	return 0;
}