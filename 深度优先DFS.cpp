#include<stdlib.h>
#include<stdio.h>
#include<iostream>
#define M 8
#define N 8
#define MaxSize M * N

#define OK 1
#define ERROR 0
#define INFEASIBLE -1
#define OVER_FLOW -2
using namespace std;


typedef int Status;

//x与y的增量
typedef struct {
	int incX,incY;    
}Direction;

//方向
Direction direct[4];

//位置
typedef struct{
	int x,y;         //当前坐标
	int di;     //当前方向
}Box;

//栈
typedef struct 
{
	Box data[MaxSize];
	int top; 
}SqStack;

//初始化栈
void InitStack(SqStack *&S)
{
	S=(SqStack *)malloc(sizeof(SqStack));
	S->top=0;
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
Status StackEmpty(SqStack *&S)
{
	if(S->top==0)
	{
		return OK;//栈空返回真 
	}
	else
	{
		return ERROR;
	}
}

//入栈
void Push(SqStack *&S,Box e)
{
	if(S->top==MaxSize)
	{
		printf("入栈失败！\n");	
	}
	S->data[S->top]=e;
	S->top++;
}

//出栈
Box Pop(SqStack *&S)
{
	Box e;
	S->top--;
	e = S->data[S->top];
	return e;
}

//销毁
void Destroy(SqStack *&S)
{
	free(S);
	printf("\n已销毁！");
}

//寻找路径
bool findPath(int maze[][N + 2],Direction direct[], SqStack *&S)
{
	Box temp;
	int x, y, di;
	int line, col;
	maze[1][1] = -1;
	temp.x = 1; temp.y = 1; temp.di = -1;
	Push(S, temp);//将temp压入到堆栈
	while (!StackEmpty(S))//当栈不空的时候
	{
		temp = Pop(S);
		x = temp.x; y = temp.y; di = temp.di + 1;
		while (di < 4)
		{
			line = x + direct[di].incX;
			col = y + direct[di].incY;
			if (maze[line][col] == 0)
			{
				temp.x = x; temp.y = y; temp.di = di;
				Push(S, temp);    
				x = line;
				y = col;
				maze[line][col] = -1; 
				if (x == M && y == N) return true;
				else di = 0;
			}
			else
				di++;
		}
	}
	return false;
}

void DisplayPath(Box path[], int count)
{
	for(int i = count-1; i >= 0; i--)
	{
		printf("%d %d %d\n", path[i].x, path[i].y, path[i].di);
	}
}

int main()
{ 
	int maze[M + 2][N + 2] = {
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 0, 0, 1, 0, 0, 0, 1, 0, 1},
	{1, 0, 0, 1, 0, 0, 0, 1, 0, 1},
	{1, 0, 0, 0, 0, 1, 1, 0, 0, 1},
	{1, 0, 1, 1, 1, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 1, 0, 0, 0, 0, 1},
	{1, 0, 1, 0, 0, 0, 1, 0, 0, 1},
	{1, 0, 1, 1, 1, 0, 1, 1, 0, 1},
	{1, 1, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	};
	Direction direct[4];//初始化方向结构体 
	Init(direct);//初始化方向结构体 
	SqStack *S;
	InitStack(S);
	StackEmpty(S);
	Box path[MaxSize];
	findPath(maze, direct, S);
	int count = 0;
	for(int i = 0; !StackEmpty(S); i++)
	{
		path[i] = Pop(S);
		count++;
	}
	DisplayPath(path, count);
	Destroy(S);
	return 0;
}





