#include <iostream>
#include <algorithm>
#include <list>
#include <cstdlib>
#include <ctime>
#include <windows.h>

using namespace std;

const int maxn = 100;
struct node{ // 每个节点的属性 
	int x, y, f, g, h;
	bool isBatter;
}map[maxn][maxn]; // road用来记录路径 
struct dir{
	int x, y;
	char way;
}road[maxn][maxn];
int n;
char mainMap[maxn][maxn];
node start, fin;
list<node> openSet; // 存可以继续判断的节点 
list<node> closeSet; // 存已经判断过的节点 
int dirx[4] = {-1,  0, 0, 1}; // 4个方向 
int diry[4] = { 0, -1, 1, 0};
string ans;
void color(int x) // 给文字上色 
{
	if(x >= 0 && x <= 15)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),x);
	else
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),7);	
}

void gotoxy(int x,int y) // 将光标移动到指定坐标 
{
	COORD pos = {x,y};
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hOut,pos);
}

bool cmp(node a, node b){ // 对 list中的f继续排序 
	return a.f < b.f;
}

bool isInclude(list<node> arr, node qur){ // 判断一个节点是否在list中 
	list<node>::iterator it;
	for(it = arr.begin(); it != arr.end(); it++){
		if(it->x == qur.x && it->y == qur.y){
			return true;
		}
	}
	return false;
}

void createMap(){
	ans = "";
	int m;
	int wall[maxn][maxn];
	cin >> n >> m;
	memset(wall, 0, sizeof(wall));
	memset(mainMap, 0 ,sizeof(mainMap));
	srand(time(NULL));
	for(int i = 1; i <= n; i++)
		for(int j = 1; j <= n; j++)
			wall[i][j] = rand() % m; // 在地图内随机生成障碍物(m越小，障碍物越多) 
	for(int i = 0; i <= n+1; i++){
		mainMap[i][0] = '#';
		mainMap[0][i] = '#';
		mainMap[n+1][i] = '#';
		mainMap[i][n+1] = '#';
	}
	start.x = 1;
	start.y = 1;
	fin.x = n;
	fin.y = n;
	for(int i = 1; i <= n; i++){
		for(int j = 1; j <= n; j++){
			if(wall[i][j] == 1)
				mainMap[i][j] = '#';
			else
				mainMap[i][j] = '.';
			map[i][j].x = i;
			map[i][j].y = j;
			road[i][j].x = 0;
			road[i][j].y = 0;
		}
	}
	mainMap[1][1] = '.';
	mainMap[n][n] = '.';
	system("cls");
}

int getHeuristicDist(node a, node b){ // 计算当前点到终点的距离 h 
	return (10*(abs(a.x-b.x)) + 10*(abs(a.y-b.y))); 	
}

void dfs(int x, int y){ // 回溯路径 
	if(x == start.x && y == start.y)
		return;
	else
		dfs(road[x][y].x, road[x][y].y);
	mainMap[x][y] = '@';
	ans += road[x][y].way;
}

void drawMap(){ // 重绘地图 
	gotoxy(0, 0);
	for(int i = 0; i <= n+1; i++){
		for(int j = 0; j <= n+1; j++){
			if(mainMap[i][j] == '@'){
				color(4);
				cout << '.';
			}else if(mainMap[i][j] == '#'){
				color(7);
				cout << mainMap[i][j];
			}
			else{
				cout << ' ';
			}
		}
		cout << endl;
	}
	cout << ans << endl;
}

void debug(){
	list<node>::iterator it;
	for(it = openSet.begin(); it != openSet.end(); it++){
		gotoxy(it->y, it->x);
		color(4);
		cout << '*';
	}
	for(it = closeSet.begin(); it != closeSet.end(); it++){
		gotoxy(it->y, it->x);
		color(5);
		cout << '.';
	}	
} 

void aStart(){
	// 清空list 
	openSet.clear();
	closeSet.clear();
	start.g = 0;
	start.h = 0;
	start.f = 0;
	openSet.push_front(start); // 把起点加入到openSet中 
	while(!openSet.empty()){ // 当openSet不为空时 
		node now = openSet.front(); // 取openSet中f最小的一个节点 
		if(now.x == fin.x && now.y == fin.y){ // 当终点在openSet中时结束循环 
			dfs(fin.x, fin.y);
			drawMap();
			cout << "DONE" << endl;
			break;
		}
		openSet.pop_front(); // 将第一个节点删除 
		closeSet.push_back(now); // 将删除的节点加入到closeSet中 
		for(int i = 0; i < 4; i++){ // 判断8个邻居 
			node neihbor = map[now.x+dirx[i]][now.y+diry[i]];
			// 边界 
			if(mainMap[now.x+dirx[i]][now.y+diry[i]] == '#' || now.x+dirx[i] >= n+1 || now.x+dirx[i] <= 0 || now.y+diry[i] >= n+1 || now.y+diry[i] <= 0)
		 		continue;
 			// 判断改节点是否在closeSet中 
		 	if(isInclude(closeSet, neihbor))
		 		continue;
	 		node tmp; // 临时节点,用来判断是否需要改变当前邻居的属性 
	 		tmp.g = now.g + 10; // 计算新的g值 
	 		//cout << now.g << endl;
	 		if(!isInclude(openSet, neihbor)) // 如果邻居不在openSet中则加入 
		 		tmp.isBatter = true; // 暂时判断为更好的节点 
		 	else if(tmp.g < neihbor.g) // 改变后的g更小 
	 			tmp.isBatter = true; // 判断为更好的节点 
	 		else
	 			tmp.isBatter = false; // 如果更差 
 			if(tmp.isBatter){
 				// 记录父节点
			 	if(i == 3) // 判断当前可行解方向 
					road[neihbor.x][neihbor.y].way = 'S';  
            	else if(i == 2)   
					road[neihbor.x][neihbor.y].way = 'D';  
            	else if(i == 1)   
					road[neihbor.x][neihbor.y].way = 'A';  
            	else if(i == 0)   
					road[neihbor.x][neihbor.y].way = 'W';  
 				road[neihbor.x][neihbor.y].x = now.x; 
 				road[neihbor.x][neihbor.y].y = now.y;
 				// 更新改邻居节点的属性 
			 	neihbor.g = tmp.g;
			 	neihbor.h = getHeuristicDist(neihbor, fin); 
			 	neihbor.f = neihbor.g + neihbor.h; // f = g + h;
			 	openSet.push_front(neihbor); // 加入到openSet中 
		 	}
		 	//cout << neihbor.f << '-' << neihbor.x << '-' << neihbor.y << endl;
		}
		debug();
		Sleep(100);
		openSet.sort(cmp); 
		/*node text = openSet.front();
		cout << text.f << "--" << text.x << "--" << text.y << endl;
		cout << openSet.size() << endl;
		cout << "--------" << endl;
		*/
	}
	if(openSet.empty()){ // 当所有可以搜索的节点都搜索完成后没有搜索到终点则无解 
		gotoxy(0, n+2);
		cout << "Failure" << endl;
	}
}

int main(){
	while(1){
		createMap();
		drawMap();
		aStart();
		
	}
}
