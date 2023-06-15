//显示中文
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>//用于函数SetConsoleOutputCP(65001);更改cmd编码为utf8
#include<stdlib.h>
#include<stdio.h>
#include<iostream>
#define M 4
#define N 4
#define MaxSize M * N

#define OK 1
#define ERROR 0
#define INFEASIBLE -1
#define OVER_FLOW -2
using namespace std;

typedef int Status;

typedef struct {
	int map[M + 2][N + 2];//用户输入矩阵（0，1）表示迷宫的初始生成
}MazeType;

//初始化迷宫
void InitMaze(MazeType&maze) {
	for (int i = 0; i < M + 2; i++) {
		for (int j = 0; j < N + 2; j++) {
			if (i == 0 || j == 0 || i == M + 1 || j == N + 1) {
				maze.map[i][j] = 1;
			}
			else {
				cout << "第" << i << "行" << "第" << j << "列：";
				cin >> maze.map[i][j];
			}
		}
	}
}


//x与y的增量
typedef struct {
	int incX,incY;    
}Direction;

//位置
typedef struct{
	int x,y;     //当前坐标
	int di;     //当前方向
}Box;

//栈
typedef struct 
{
	Box data[MaxSize];
	int top; 
}SqStack;

//初始化栈
void InitStack(SqStack &S)
{
	S=*(SqStack *)malloc(sizeof(SqStack));
	S.top=0;
}

//四个方向增量初始化
void Init(Direction direct[])
{
	//右
	direct[0].incX = 0;
	direct[0].incY = 1;
	//下
	direct[1].incX = 1;
	direct[1].incY = 0;
	//左
	direct[2].incX = 0;
	direct[2].incY = -1;
	//上
	direct[3].incX = -1;
	direct[3].incY = 0;
}

//判栈空
Status StackEmpty(SqStack &S)
{
	if(S.top==0)
	{
		return OK;//栈空返回真 
	}
	else
	{
		return ERROR;
	}
}

//入栈
void Push(SqStack &S,Box e)
{
	if(S.top==MaxSize)
	{
		printf("入栈失败！\n");	
	}
	S.data[S.top]=e;
	S.top++;
}

//出栈
Box Pop(SqStack &S)
{
	Box e;
	S.top--;
	e = S.data[S.top];
	return e;
}

//销毁
void Destroy(SqStack &S)
{
	free(&S);
	cout<<"栈已销毁！"<<endl;
}

//寻找路径
Status findPath(int maze[][N + 2],Direction direct[], SqStack &S)
{
	Box temp;
	int x, y, di;
	int L, C;
	maze[1][1] = -1;
	temp.x = 1; 
	temp.y = 1; 
	temp.di = -1;
	Push(S, temp);//将temp压入到堆栈
	while (!StackEmpty(S))//当栈不空的时候
	{
		temp = Pop(S);
		x = temp.x; y = temp.y; di = temp.di + 1;
		while (di < 4)
		{
			L = x + direct[di].incX;
			C = y + direct[di].incY;
			if (maze[L][C] == 0)
			{
				temp.x = x; 
				temp.y = y; 
				temp.di = di;
				Push(S, temp);    
				x = L;
				y = C;
				maze[L][C] = -1; 
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
			{
				di++;
			}
		}
	}
	return ERROR;
}

//打印创建的迷宫
Status PrintMaze(MazeType maze) {
	for (int i = 0; i < M + 2; i++) {
		for (int j = 0; j < N + 2; j++) {
			cout << maze.map[i][j] << "  ";
		}
		cout << "\n";
	}
	return OK;
}



//画路径
Status printPath(MazeType maze,Box path[], int count)
{
	//描绘路径
	for(int i = count-1; i >= 0; i--)
	{
		int x = path[i].x;
		int y = path[i].y;
		maze.map[x][y] = 9;
	}
	maze.map[M][N] = 9;
	PrintMaze(maze);
	return OK;
}



int main()
{ 
    //显示中文
    SetConsoleOutputCP(65001);

	cout << "迷宫只能从左上角开始走到右下角" << endl;
    MazeType MAZE;
	InitMaze(MAZE);
	MazeType* result = new MazeType(MAZE);
	cout << "生成的迷宫为：" << endl;
	PrintMaze(MAZE);

	cout<<endl;
	cout << "找到通路的迷宫为(以9表示找到的路径)：" << endl;
	Direction direct[4];//初始化方向结构体 
	Init(direct);//初始化方向结构体 
	SqStack S;
	InitStack(S);
	StackEmpty(S);
	Box path[MaxSize];
	if(findPath(MAZE.map, direct, S))
	{
		int count = 0;
		for(int i = 0; !StackEmpty(S); i++)
		{
			path[i] = Pop(S);
			count++;
		}
		printPath(*result,path,count);
		Destroy(S);
	}
	else
	{
		cout<<"迷宫没有通路"<<endl;
	}
	system("pause");
	return 0;
}