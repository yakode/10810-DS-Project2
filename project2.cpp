#include <iostream>
#include <fstream>
using namespace std;
int R, C, B;
char floor_map[1000+5][1000+5];
//0-->floor need to clean
//1-->obstacle
//2-->have cleaned:)
fstream outFile;

using Point = struct point{
	int x, y;
};
bool operator == (Point A, Point B){
	return A.x == B.x && A.y == B.y;
}
bool operator != (Point A, Point B){
	return A.x != B.x || A.y != B.y;
}
Point RP;

bool all_clean();

int step_map[1000+5][1000+5];
void cal_step_map(Point start);

Point find_farthest();

int step2B(Point A, Point B);

int A2B(Point A, Point B, bool out);

int main(){
	//read map
	ifstream inFile("floor.data");
	if(!inFile) cout << "fail to open floor.data \n";
	char input_map[1005][1005];
	inFile >> R >> C >> B;
	for(int i = 0; i < R; ++i)
		inFile >> input_map[i];
	inFile.close();

	//copy map and find R
	for(int i = 0; i < R; ++i){
		for(int j = 0; j < C; ++j){
			floor_map[i][j] = input_map[i][j];
			if(input_map[i][j] == 'R'){
				RP.x = i;
				RP.y = j;
			}
		}
	}

	//cal total steps
	Point pos = RP;
	int total_step = 0;
	while(!all_clean()){
		cal_step_map(pos);

		Point target = find_farthest();

		
		total_step += A2B(pos, target, false);
		total_step += A2B(target, RP, false);
		pos = RP;
	}

	//print every step
	outFile.open("final.path", ios::out);
	outFile << total_step << '\n';

	for(int i = 0; i < R; ++i)
		for(int j = 0; j < C; ++j)
			floor_map[i][j] = input_map[i][j];

	outFile << RP.x << ' ' << RP.y << "\n";
	while(!all_clean()){
		cal_step_map(pos);
		Point target = find_farthest();
		A2B(pos, target, true);
		A2B(target, RP, true);
		pos = RP;
	}

	outFile.close();
	return 0;
}
bool all_clean(){
	for(int i = 0; i < R; ++i)
		for(int j = 0; j < C; ++j)
			if(floor_map[i][j] == '0') return false;
	return true;
}

int dx[4] = {0, 0, -1, 1},
	dy[4] = {-1, 1, 0, 0};

void cal_step_map(Point start){
	for(int i = 0; i < R; ++i)
		for(int j = 0; j < C; ++j)
			step_map[i][j] = 0;
	
	int N = 100000;
	Point queue[N];
	int front = -1, rear = -1;
	queue[++rear] = start;
	step_map[start.x][start.y] = 0;
	while(front != rear){
		front = (front + 1) % N;
		Point ptr = queue[front];
		for(int i = 0; i < 4; ++i){
			Point tmp;
			tmp.x = ptr.x + dx[i];
			tmp.y = ptr.y + dy[i];
			if(tmp == RP ||tmp.x >= R || tmp.x < 0 || tmp.y >= C || tmp.y < 0 || 
					floor_map[tmp.x][tmp.y] == '1' || step_map[tmp.x][tmp.y] != 0) continue;
			step_map[tmp.x][tmp.y] = step_map[ptr.x][ptr.y] + 1;
			rear = (rear + 1) % N;
			queue[rear] = tmp;
		}
	}
}

Point find_farthest(){
	int max_dis = -1;
	Point ans;
	ans.x = 0;
	ans.y = 0;
	for(int i = 0; i < R; ++i){
		for(int j = 0; j < C; ++j){
			if(floor_map[i][j] == '0' && step_map[i][j] > max_dis){
				max_dis = step_map[i][j];
				ans.x = i;
				ans.y = j;
			}
		}
	}
	return ans;
}

int step2B(Point A, Point B){
	for(int i = 0; i < R; ++i)
		for(int j = 0; j < C; ++j)
			step_map[i][j] = 0;

	int N = 100000;
	Point queue[N];
	int front = -1, rear = -1;
	queue[++rear] = A;
	step_map[A.x][A.y] = 0;
	while(front != rear){
		front = (front + 1) % N;
		Point ptr = queue[front];
		for(int i = 0; i < 4; ++i){
			Point tmp;
			tmp.x = ptr.x + dx[i];
			tmp.y = ptr.y + dy[i];
			if(tmp == A || tmp.x >= R || tmp.x < 0 || tmp.y >= C || tmp.y < 0 || 
					floor_map[tmp.x][tmp.y] == '1' || step_map[tmp.x][tmp.y] != 0) continue;
			step_map[tmp.x][tmp.y] = step_map[ptr.x][ptr.y] + 1;
			if(tmp == B) return step_map[tmp.x][tmp.y];
			rear = (rear + 1) % N;
			queue[rear] = tmp;
		}
	}
	return -1;
}

int A2B(Point A, Point B, bool out){
	int step = 0;
	while(A != B){
		int disAB = step2B(A, B);
		
		for(int i = 0; i < 4; ++i){

			Point tmp;
			tmp.x = A.x + dx[i];
			tmp.y = A.y + dy[i];
			if(tmp.x >= R || tmp.x < 0 || tmp.y >= C || tmp.y < 0 || floor_map[tmp.x][tmp.y] == '1') continue;
			if(tmp == B){
				A = B;
				floor_map[A.x][A.y] = '2';
				step++;
				if(out) outFile << B.x << ' ' << B.y << "\n";
				break;
			}
			int disTB = step2B(tmp, B);

			if(disAB == disTB+1){
				A = tmp;
				if(B!=RP) floor_map[A.x][A.y] = '2';
				step++;
				if(out) outFile << A.x << ' ' << A.y << "\n";
				break;
			}
		}
	}
	return step;
}
