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
//λ������
typedef struct {
	int x, y;//��ʾ�Թ��е�λ����Ϣx��y��
}PostType;
//�Թ�����
typedef struct {
	int map[row + 2][col + 2];//�û��������0��1����ʾ�Թ��ĳ�ʼ����
	char arr[RANGE + 2][RANGE + 2];//���������������ַ���@# ����ʾ̽��״̬
}MazeType;
//ջ����
typedef struct {
	int step;//��ǰλ����·���ϵġ���š�
	PostType seat;//��ǰλ������
	DirectiveType di;//����һ����λ�õķ���
}ElemType;//ջԪ������
typedef struct {
	ElemType* base;
	ElemType* top;
	int stacksize;
}Stack;//����˳��ջʵ��

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
				cout << "��" << i << "��" << "��" << j << "�У�";
				cin >> maze.map[i][j];
				if (maze.map[i][j] == 1) maze.arr[i][j] = '#';
				else maze.arr[i][j] = ' ';
			}
		}
	}
}

Status Pass(MazeType maze, PostType curpos) {//�жϸ����Ƿ��߹�������
	if (maze.arr[curpos.x][curpos.y] == ' ') return OK;
	return ERROR;
}

Status FootPrint(MazeType& maze, PostType curpos) {//��¼�߹��ĸ���
	if (maze.arr[curpos.x][curpos.y] == ' ') {
		maze.arr[curpos.x][curpos.y] = '*';
		return OK;
	}
	return ERROR;
}

Status Same(PostType curpos, PostType end) {//�ж���������λ���Ƿ�һ��
	if (curpos.x == end.x && curpos.y == end.y)
		return OK;
	return ERROR;
}

PostType NextPos(PostType curpos, int di) {
	switch (di)
	{
	case 1: {curpos.y = curpos.y + 1; return curpos; break; }//�����ƶ�һ��
	case 2: {curpos.x = curpos.x + 1; return curpos; break; }//�����ƶ�һ��
	case 3: {curpos.y = curpos.y - 1; return curpos; break; }//�����ƶ�һ��
	case 4: {curpos.x = curpos.x - 1; return curpos; break; }//�����ƶ�һ��
	}
}

Status MarkPrint(MazeType& maze, PostType pos) {
	maze.arr[pos.x][pos.y] = '@';
	return OK;
}

Status MazePath(MazeType& maze, PostType start, PostType end) {
	Stack S; InitStack(S);
	PostType curpos = start;//�趨����ǰλ�á�Ϊ�����λ�á�
	int curstep = 1;//̽����һ��,��¼̽������
	bool found = ERROR;//�ж��Ƿ�ﵽ�յ�
	ElemType e;//StackԪ��
	do {
		if (Pass(maze, curpos)) {
			//��ǰλ�ÿ���ͨ��������δ���ߵ�����ͨ�������㼣
			FootPrint(maze, curpos);
			e.di = 1; e.seat = curpos; e.step = curstep;
			Push(S, e);//����·��
			if (Same(curpos, end))  found = OK; //�����յ㣨���ڣ�
			else {
				curpos = NextPos(curpos, 1);//��һλ���ǵ�ǰλ�õĶ��ڣ������ƶ�һ��
				//NextPos(curpos, 1);
				curstep++;//̽����һ��
			}
		}
		else {//��ǰλ�ò���ͨ��
			if (!StackEmpty(S)) {
				Pop(S, e);//���ղŲ���ǰ����StackԪ�س�ջ���൱���˻�һ��
				while (e.di == 4 && !StackEmpty(S)) {
					MarkPrint(maze, e.seat);//�Ըղų�ջ��Ԫ�ص�λ�ñ��
					Pop(S, e);//��һ��Ԫ�س�ջ������һ����e��һ��
				}
				if (e.di < 4) {
					e.di++; Push(S, e);//����һ������̽��
					curpos = NextPos(e.seat, e.di);//�趨��ǰλ���Ǹ��·�������
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
	cout << "���ɵ��Թ�Ϊ��" << endl;
	PrintMaze(maze);
	PostType start, end;
	cout << "��������㣺";  cin >> start.x >> start.y;
	cout << "�������յ㣺";  cin >> end.x >> end.y;
	if (MazePath(maze, start, end)) {
		cout << "·�����ڣ��Թ����·��Ϊ��" << endl;
		PrintMaze(maze);
	}
	else {
		cout << "·�������ڣ��Թ����·��Ϊ��" << endl;
		PrintMaze(maze);
	}
	return 0;
}