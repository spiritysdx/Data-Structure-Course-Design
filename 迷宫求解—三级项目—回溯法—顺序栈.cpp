#include<stdlib.h>
#include<stdio.h>
#include<iostream>
using namespace std;

#define RANGE 4
#define row 4
#define col 4

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
//迷宫类型
typedef struct {
	int map[row + 2][col + 2];//用户输入矩阵（0，1）表示迷宫的初始生成
	char arr[RANGE + 2][RANGE + 2];//程序的输入矩阵，以字符“@# ”表示探索状态
}MazeType;
//栈类型
typedef struct {
	int step;//当前位置在路径上的“序号”
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

void InitMaze(MazeType& maze) {
	for (int i = 0; i < row + 2; i++) {
		for (int j = 0; j < col + 2; j++) {
			if (i == 0 || j == 0 || i == row + 1 || j == col + 1) {
				maze.map[i][j] = 0;
				maze.arr[i][j] = '#';
			}
			else {
				cout << "第" << i << "行" << "第" << j << "列：";
				cin >> maze.map[i][j];
				if (maze.map[i][j] == 1) maze.arr[i][j] = '#';
				else maze.arr[i][j] = ' ';
			}
		}
	}
}

Status Pass(MazeType maze, PostType curpos) {//判断格子是否走过且能走
	if (maze.arr[curpos.x][curpos.y] == ' ') return OK;
	return ERROR;
}

Status FootPrint(MazeType& maze, PostType curpos) {//记录走过的格子
	if (maze.arr[curpos.x][curpos.y] == ' ') {
		maze.arr[curpos.x][curpos.y] = '*';
		return OK;
	}
	return ERROR;
}

Status Same(PostType curpos, PostType end) {//判断两个格子位置是否一样
	if (curpos.x == end.x && curpos.y == end.y)
		return OK;
	return ERROR;
}

PostType NextPos(PostType curpos, int di) {
	switch (di)
	{
	case 1: {curpos.y = curpos.y + 1; return curpos; break; }//向右移动一格
	case 2: {curpos.x = curpos.x + 1; return curpos; break; }//向下移动一格
	case 3: {curpos.y = curpos.y - 1; return curpos; break; }//向左移动一格
	case 4: {curpos.x = curpos.x - 1; return curpos; break; }//向上移动一格
	}
}

Status MarkPrint(MazeType& maze, PostType pos) {
	maze.arr[pos.x][pos.y] = '@';
	return OK;
}

Status MazePath(MazeType& maze, PostType start, PostType end) {
	Stack S; InitStack(S);
	PostType curpos = start;//设定“当前位置”为“入口位置”
	int curstep = 1;//探索第一步,记录探索步数
	bool found = ERROR;//判断是否达到终点
	ElemType e;//Stack元素
	do {
		if (Pass(maze, curpos)) {
			//当前位置可以通过，即是未曾走到过的通道留下足迹
			FootPrint(maze, curpos);
			e.di = 1; e.seat = curpos; e.step = curstep;
			Push(S, e);//加入路径
			if (Same(curpos, end))  found = OK; //到达终点（出口）
			else {
				curpos = NextPos(curpos, 1);//下一位置是当前位置的东邻（向右移动一格）
				//NextPos(curpos, 1);
				curstep++;//探索下一步
			}
		}
		else {//当前位置不能通过
			if (!StackEmpty(S)) {
				Pop(S, e);//将刚才不能前进的Stack元素出栈，相当于退回一步
				while (e.di == 4 && !StackEmpty(S)) {
					MarkPrint(maze, e.seat);//对刚才出栈的元素的位置标记
					Pop(S, e);//下一个元素出栈，与上一步的e不一样
				}
				if (e.di < 4) {
					e.di++; Push(S, e);//换下一个方向探索
					curpos = NextPos(e.seat, e.di);//设定当前位置是该新方向向上
				}
			}
		}
	} while (!StackEmpty(S) && !found);
	return found;
}

Status PrintMaze(MazeType maze) {
	for (int i = 0; i < RANGE + 2; i++) {
		for (int j = 0; j < RANGE + 2; j++) {
			cout << maze.arr[i][j] << "  ";
		}
		cout << "\n";
	}
	return OK;
}

int main() {
	MazeType maze;
	InitMaze(maze);
	cout << "生成的迷宫为：" << endl;
	PrintMaze(maze);
	PostType start, end;
	cout << "请输入起点：";  cin >> start.x >> start.y;
	cout << "请输入终点：";  cin >> end.x >> end.y;
	if (MazePath(maze, start, end)) {
		cout << "路径存在，迷宫求解路径为：" << endl;
		PrintMaze(maze);
	}
	else {
		cout << "路径不存在，迷宫求解路径为：" << endl;
		PrintMaze(maze);
	}
	return 0;
}