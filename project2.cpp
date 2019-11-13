#include <iostream>
#include <fstream>
using namespace std;
int R, C, B;
char floor_map[1000+5][1000+5];
//0-->floor need to clean
//1-->obstacle
//2-->have cleaned:)
fstream outFile;

typedef Point = struct point{
	int x, y;
};
Point R;

bool all_clean();

int step_map[1000+5][1000+5];
void cal_step_map(Point start);

Point find_farthest();

int step2R(Point start);

int A2B(Point A, Point B, bool out);

int main(){
	//read map
	ifstream inFile("floor.data");
	if(!inFile) cout << "fail to open floor.data \n";
	char input_map[1001][1001];
	inFile >> R >> C >> B;
	for(int i = 0; i < R; ++i)
		inFile >> input_map[i];
	inFile.close();

	//copy map and find R
	for(int i = 0; i < R; ++i){
		for(int j = 0; j < C; ++j){
			floor_map[i][j] = input_map[i][j];
			if(input_map[i][j] == 'R'){
				R.x = i;
				R.y = j;
			}
		}
	}

	//cal total steps
	Point pos = R;
	int total_step = 0;
	while(!all_clean()){
		cal_step_map(pos);
		Point target = find_farthest();
		total_step += A2B(pos, target, false);
		total_step += A2B(target, R, false);
		pos = R;
	}

	//print every step
	outFile.open("final.path", ios::out);

	for(int i = 0; i < R; ++i)
		for(int j = 0; j < C; ++j)
			floor_map[i][j] = input_map[i][j];

	while(!all_clean()){
		cal_step_map(pos);
		Point target = find_farthest();
		total_step += A2B(pos, target, true);
		total_step += A2B(target, R, true);
		pos = R;
	}

	outFile.close();
	return 0;
}
