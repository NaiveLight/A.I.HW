//��ǻ�Ͱ��� 20113281 �ڼ���
//A.I. HOMEWORK 1-1
//8-puzzle by A*
//h(n) = MissMatched Tiles Count;

#include <cstdio>
#include <queue>
#include <stack>
#include <algorithm>
#include <vector>

using namespace std;

typedef struct Puzzle {
public:
	//������ ���� enum
	enum direction{LEFT = 0, UP, RIGHT, DOWN, DIRMAX};
	
	Puzzle* parent; //�θ� ����
	int board[9]; //������ ����
	int depth; //���� ����
	int stateNUM; //STATE Number ����
	int cost; //depth+heuristic ����
	bool cantMove[DIRMAX]; //Ÿ�� �̵� ���ɿ��� ����
	
	//constructor
	Puzzle() :parent(NULL), stateNUM(0), depth(0), cost(0) {
			for (int i = 0; i < 9; ++i)
				board[i] = 0;
	
			for (int i = 0; i < 4; ++i)
				cantMove[i] = true;
		}
	Puzzle(int a[]) : parent(NULL), stateNUM(0), depth(0), cost(0) {
		for (int i = 0; i < 9; i++) {
			board[i] = a[i];
		}

		for (int i = 0; i < 4; i++) {
			canMove(i);
		}
	}
	//MissMatched function
	int heuristic(Puzzle* u) {
		int cnt = 0;
		for (int i = 0; i < 9; i++) {
			if (this->board[i] != u->board[i])
				cnt++;
		}
		return cnt;
	}

	//Ÿ�� �� swap�� ���� function
	void swap(int i, int j) {
		int tmp = board[i];
		board[i] = board[j];
		board[j] = tmp;
	}
	//target�� �� state�� ���ϱ� ���� function
	bool equalPuzzle(Puzzle* u) {
		for (int i = 0; i < 9; i++) {
			if (this->board[i] != u->board[i])
				return false;
		}
		return true;
	}
	//�� Ÿ���� ã�� ���� function
	int findEmpty(){
		for (int i = 0; i < 9; i++) {
			if (board[i] == -1)
				return i;
		}
		return -1;
	}
	//�̵��� ������ ���� ã�� ���� function
	bool canMove(int i) {
		switch (i) {
		case LEFT:
		{
			int emptyPos = findEmpty();
			if (0 == emptyPos || 3 == emptyPos || 6 == emptyPos)
				return false;
		}
			break;
		case UP:
		{
			int emptyPos = findEmpty();
			if (0 == emptyPos || 1 == emptyPos || 2 == emptyPos)
				return false;
			break;
		}
		case RIGHT:
		{
			int emptyPos = findEmpty();
			if (2 == emptyPos || 5 == emptyPos || 8 == emptyPos)
				return false;
			break;
		}
		case DOWN:
		{
			int emptyPos = findEmpty();
			if (6 == emptyPos || 7 == emptyPos || 8 == emptyPos)
				return false;
			break;
		}
		}
		return cantMove[i];
	}
	//���� state node�� �����ϱ� ���� function
	Puzzle* NodeExpention(int val) {
		Puzzle* pRt = new Puzzle;

		pRt->depth = this->depth + 1;
		pRt->parent = this;

		for (int i = 0; i < 9; ++i)
			pRt->board[i] = this->board[i];

		int nReversal;
		switch (val) {
		case LEFT:
			nReversal = RIGHT;
			break;
		case UP:
			nReversal = DOWN;
			break;
		case RIGHT:
			nReversal = LEFT;
			break;
		case DOWN:
			nReversal = UP;
			break;
		}

		pRt->cantMove[nReversal] = false;

		int emptyPos = findEmpty();

		switch (val) {
		case LEFT:
			pRt->swap(emptyPos, emptyPos - 1);
			break;
		case UP:
			pRt->swap(emptyPos, emptyPos - 3);
			break;
		case RIGHT:
			pRt->swap(emptyPos, emptyPos + 1);
			break;
		case DOWN:
			pRt->swap(emptyPos, emptyPos + 3);
			break;
		}

		return pRt;
	}
}puzzle;

//���� ���� ����� ���� function
void print(const puzzle &u) {
	for (int i = 0; i < 9; i++) {
		if (u.board[i] == -1)
			printf("  ");
		else
			printf("%d ", u.board[i]);

		if (i % 3 == 2)
			printf("\n");
	}
}

//�켱���� ť ������ ���� Comparator struct
struct Comparator {
	bool operator()(const puzzle* u, const puzzle* v) {
		if (u->cost == v->cost) {
			return u->stateNUM > v->stateNUM;
		}
		else {
			return u->cost < v->cost;
		}
	}
};

int init[9] = { 2,8,3,1,6,4,7,-1,5 };
int goal[9] = { 1,2,3,8,-1,4,7,6,5 };

int main() {
	//start, target Node ����
	puzzle* start = new puzzle(init);
	puzzle* target = new puzzle(goal);

	//A*�� ���� OPEN�� CLOSE ť, OPEN ť ����� ���� OPENFORPRINT ť
	priority_queue<puzzle*, vector<puzzle*>, Comparator> OPEN, OPFP;
	queue<puzzle*> CLOSE;

	//initial state ���;
	print(*start);

	//state�� �����ϱ� ���� ����
	int stNum = 0;
	start->stateNUM = stNum++;
	//OPEN ť�� start Node�� �ְ� BFS ����
	OPEN.push(start);
	while (!OPEN.empty()) {

		//���� OPEN ť ���
		OPFP = OPEN;
		printf("OPEN STATE\n");
		while (!OPFP.empty()) {
			auto printNode = OPFP.top();
			printf("%d ", printNode->stateNUM);
			OPFP.pop();
		}
		printf("\n");

		//OPEN ť���� NODE �޾ƿ�
		auto tmpNode = OPEN.top();
		OPEN.pop();
		//CLOSEť�� ���� �������� STATE NODE push
		CLOSE.push(tmpNode);

		//4 �������� �˻�
		for (int i = 0; i < 4; i++) {
			if (tmpNode->canMove(i)) {
				puzzle* newPuzzle = tmpNode->NodeExpention(i);
				newPuzzle->stateNUM = stNum++;
				newPuzzle->cost = -(newPuzzle->depth + newPuzzle->heuristic(target));
				printf("%2d STATE�� COST : %d\n", newPuzzle->stateNUM, -newPuzzle->cost);
				//print(*newPuzzle);
				//���ο� STATE�� Target�� ���ٸ� Target State ��� �� PATH ���
				if (target->equalPuzzle(newPuzzle)) {
					printf("\nFIND TARGET STATE\n");
					printf("%d STATE\n", newPuzzle->stateNUM);
					print(*newPuzzle);
					stack<puzzle*> PATH;
					PATH.push(newPuzzle);
					while (newPuzzle->parent != NULL) {
						PATH.push(newPuzzle->parent);
						newPuzzle = newPuzzle->parent;
					}

					printf("SOLUTION PATH\n");
					while (!PATH.empty()) {
						auto tmpPath = PATH.top();
						if(PATH.size() == 1)
							printf("%d\n", tmpPath->stateNUM);
						else
							printf("%d -> ", tmpPath->stateNUM);
						PATH.pop();
					}
					return 0;
				}
				//Target�� ���� �ʴٸ� OPEN ť�� ���ο� STATE push
				else
					OPEN.push(newPuzzle);
			}
		}
	}

	return 0;
}