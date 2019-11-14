#include <iostream>
#include <fstream>
using namespace std;
int R, C, B;
char floor_map[1000+5][1000+5];
//0-->floor need to clean
//1-->obstacle
//2-->have cleaned:)
fstream tmpFile;

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

int dirty;

bool all_clean();
void sum_dirty();

int step_map[1000+5][1000+5];
int mini_step_map[1000+5][1000+5];
void cal_step_map(Point start);

Point find_farthest();
Point find_near(Point start, int battery);

int step2B(Point A, Point B);

int A2B(Point A, Point B);
void dfs(Point cur, Point A);

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
	tmpFile.open("tmp.path", ios::out);

	Point pos = RP;
	int total_step = 0;
	int this_step;
	int battery = B;
	cal_step_map(pos);
	sum_dirty();
	while(!all_clean()){
		Point target = find_farthest();
		this_step = A2B(pos, target);
		total_step += this_step;
		battery -= this_step;
		pos = target;
		
		total_step += A2B(pos, RP);
		pos = RP;
		battery = B;
	}
	tmpFile.close();

	//copy path from tmp.path to final.path
	fstream outFile;
	outFile.open("final.path", ios::out);
	outFile << total_step << '\n';
	outFile << RP.x << ' ' << RP.y << '\n';
	ifstream tmpinFile("tmp.path");
	if(!tmpinFile) cout << "fail to open tmp.path \n";
	int x, y;
	for(int i = 0; i < total_step; ++i){
		tmpinFile >> x >> y;
		outFile << x << ' ' << y << '\n';
	}
	tmpinFile.close();
	outFile.close();
	return 0;
}
bool all_clean(){
	return dirty == 0;
}

void sum_dirty(){
	dirty = 0;
	for(int i = 0; i < R; ++i)
		for(int j = 0; j < C; ++j)
			if(floor_map[i][j] == '0') dirty++;

}

int dx[4] = {0, 0, -1, 1},
	dy[4] = {-1, 1, 0, 0};

Point queue1[1000000];
void cal_step_map(Point start){
	for(int i = 0; i < R; ++i)
		for(int j = 0; j < C; ++j)
			step_map[i][j] = 0;
	
	int N = 1000000;
	int front = -1, rear = -1;
	queue1[++rear] = start;
	step_map[start.x][start.y] = 0;
	while(front != rear){
		front = (front + 1) % N;
		Point ptr = queue1[front];
		for(int i = 0; i < 4; ++i){
			Point tmp;
			tmp.x = ptr.x + dx[i];
			tmp.y = ptr.y + dy[i];
			if(tmp == RP ||tmp.x >= R || tmp.x < 0 || tmp.y >= C || tmp.y < 0 || 
					floor_map[tmp.x][tmp.y] == '1' || step_map[tmp.x][tmp.y] != 0) continue;
			step_map[tmp.x][tmp.y] = step_map[ptr.x][ptr.y] + 1;
			rear = (rear + 1) % N;
			queue1[rear] = tmp;
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

Point queue3[1000000];
Point pre_step[1005][1005];
int step2B(Point A, Point B){
	if(A == B) return 0;

	for(int i = 0; i < R; ++i)
		for(int j = 0; j < C; ++j)
			mini_step_map[i][j] = 0;

	int N = 1000000;
	int front = -1, rear = -1;
	queue3[++rear] = A;
	mini_step_map[A.x][A.y] = 0;
	while(front != rear){
		front = (front + 1) % N;
		Point ptr = queue3[front];
		for(int i = 0; i < 4; ++i){
			Point tmp;
			tmp.x = ptr.x + dx[i];
			tmp.y = ptr.y + dy[i];
			if(tmp == A || tmp.x >= R || tmp.x < 0 || tmp.y >= C || tmp.y < 0 || 
					floor_map[tmp.x][tmp.y] == '1' || mini_step_map[tmp.x][tmp.y] != 0) continue;
			pre_step[tmp.x][tmp.y] = ptr;
			mini_step_map[tmp.x][tmp.y] = mini_step_map[ptr.x][ptr.y] + 1;
			if(tmp == B) return mini_step_map[tmp.x][tmp.y];
			rear = (rear + 1) % N;
			queue3[rear] = tmp;
		}
	}
	return -1;
}

int A2B(Point A, Point B){
	int step = 0;
	int disAB;
	disAB = step2B(A, B);
	dfs(B, A);
	return disAB;
}

void dfs(Point cur, Point A){
	if(cur == A){
		return;
	}else{
		dfs(pre_step[cur.x][cur.y], A);
		if(floor_map[cur.x][cur.y] == '0'){
			floor_map[cur.x][cur.y] = '2';
			dirty--;
		}
		tmpFile << cur.x << ' ' << cur.y << "\n";
	}
}
