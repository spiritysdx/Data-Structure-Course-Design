//显示中文
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>//用于函数SetConsoleOutputCP(65001);更改cmd编码为utf8
#include<stdlib.h>
#include<stdio.h>
#include<iostream>
using namespace std;

#define RANGE 4
#define M 4
#define N 4
#define MaxSize M * N

#define OK 1
#define ERROR 0
#define INFEASIBLE -1
#define OVER_FLOW -2

typedef int Status;
typedef int DirectiveType;
//位置坐标
typedef struct {
	int x, y;//表示迷宫中的位置信息x行y列
}PostType;
//x与y的增量
typedef struct {
	int incX,incY;    
}Direction;
//迷宫类型
typedef struct {
	int map[M + 2][N + 2];//用户输入矩阵（0，1）表示迷宫的初始生成
}MazeType;
//栈类型
typedef struct {
	//int step;//当前位置在路径上的“序号”
	PostType seat;//当前位置坐标
	DirectiveType di;//往下一坐标位置的方向
}ElemType;//栈元素类型
typedef struct {
	ElemType* base;
	ElemType* top;
	int stacksize;
}Stack;//利用顺序栈实现

Status InitStack(Stack& S) {
	S.base = new ElemType[100];
	if (!S.base) exit(OVER_FLOW);
	S.top = S.base;
	S.stacksize = 100;
	return OK;
}

Status StackEmpty(Stack S) {
	if (S.top == S.base) return OK;
	else return ERROR;
}

Status Push(Stack& S, ElemType e) {
	if (S.top - S.base >= S.stacksize) {
		ElemType* newbase = (ElemType*)realloc(S.base, (S.stacksize + 10) * sizeof(ElemType));
		if (!newbase) exit(OVER_FLOW);
		S.base = newbase;
		S.top = S.base + S.stacksize;
		S.stacksize += 10;
	}
	*S.top++ = e;
	return OK;
}

Status Pop(Stack& S, ElemType& e) {
	if (S.top == S.base) return ERROR;
	e = *--S.top;
	return OK;
}

void InitMaze(MazeType&maze) {
	for (int i = 0; i < M + 2; i++) {
		for (int j = 0; j < N + 2; j++) {
			if (i == 0 || j == 0 || i == M + 1 || j == N + 1) {
				maze.map[i][j] = 0;
			}
			else {
				cout << "第" << i << "行" << "第" << j << "列：";
				cin >> maze.map[i][j];
			}
		}
	}
}


//四个方向增量初始化
void Init(Direction direct[])
{
	//右0
	direct[0].incX = 0;
	direct[0].incY = 1;
	//下1
	direct[1].incX = 1;
	direct[1].incY = 0;
	//左2
	direct[2].incX = 0;
	direct[2].incY = -1;
	//上3
	direct[3].incX = -1;
	direct[3].incY = 0;
}



//寻找路径
Status findPath(int maze[][N + 2],Direction direct[], Stack &S)
{
	ElemType temp;
	int x, y, di;
	int line, col;
	maze[1][1] = -1;
	temp.seat.x = 1; 
    temp.seat.y = 1; 
    temp.di = -1;
	Push(S, temp);//将temp压入到堆栈
	while (!StackEmpty(S))//当栈不空的时候
	{
		Pop(S,temp);
		x = temp.seat.x; 
        y = temp.seat.y; 
        di = temp.di + 1;
		while (di < 4)
		{
			line = x + direct[di].incX;
			col = y + direct[di].incY;
			if (maze[line][col] == 0)
			{
				temp.seat.x = x; 
                temp.seat.y = y; 
                temp.di = di;
				Push(S, temp);    
				x = line;
				y = col;
				maze[line][col] = -1; 
				if (x == M && y == N) 
                {
                    return OK;
                }
				else 
                {
                    di = 0;
                }
			}
			else
				di++;
		}
	}
	return ERROR;
}



//销毁路径
void DisplayPath(ElemType path[], int count)
{
	for(int i = count-1; i >= 0; i--)
	{
		printf("%d %d %d\n", path[i].seat.x, path[i].seat.y, path[i].di);
	}
}

int main() 
{
	//显示中文
	
    SetConsoleOutputCP(65001);

	//MazeType maze;
	//InitMaze(maze);
	//cout << "生成的迷宫为：" << endl;
	//PrintMaze(maze);
	//PostType start, end;
	//cout << "请输入起点：";  
	//cin >> start.x >> start.y;
	//cout << "请输入终点：";  
	//cin >> end.x >> end.y;


	int maze[M + 2][N + 2] = {
	{1, 1, 1, 1, 1, 1},
	{1, 0, 0, 1, 0, 0},
	{1, 0, 0, 1, 0, 1},
	{1, 0, 0, 0, 0, 1},
	{1, 0, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1},
	};

	Direction direct[4];//初始化方向结构体 
	Init(direct);//初始化方向结构体 
	Stack *S;
	InitStack(*S);
	StackEmpty(*S);
	ElemType path[MaxSize];
	int count = 0;
	for(int i = 0; !StackEmpty(*S); i++)
	{
		Pop(*S,path[i]);
		count++;
	}
	if (findPath(maze, direct, *S)) {
		cout << "路径存在，迷宫求解路径为：" << endl;
		DisplayPath(path, count);
	}
	else {
		cout << "路径不存在，迷宫求解路径为：" << endl;
		DisplayPath(path, count);
	}
	//system("pause");
	return 0;
}