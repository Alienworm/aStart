#include <iostream>
#include <algorithm>
#include <list>
#include <string>
#include <cstring>

const int maxn = 100;
struct aStartNode{
	int x, y;
	int f, g, h;
	bool isBatter;
	aStartNode(){
		f = 0;
		g = 0;
		h = 0;
		isBatter = false;
	} 
};
	
struct roadNode{
	int x, y;
	char dir;
};
int dirx[4] = {-1,  0, 0, 1};
int diry[4] = { 0, -1, 1, 0};

class aStart{
public:
	aStart();
	~aStart(); 
	int getH(aStartNode a, aStartNode b);
	void getPath(int x, int y);
	void createMap(int n, int m); 
	bool isInclude(std::list<aStartNode> arr, aStartNode qur);
	std::string mainAstart();	
private:
	aStartNode node[maxn][maxn], start, fin;
	roadNode road[maxn][maxn];
	std::list<aStartNode> openSet;
	std::list<aStartNode> closeSet;
	std::string ans;
	char map[maxn][maxn];
	int n, m;
};

bool cmp(aStartNode a, aStartNode b){
	return a.f < b.f;
}

aStart::aStart(){
	openSet.clear();
	closeSet.clear();
	ans = "";
	memset(map, 0, sizeof(map));
}
aStart::~aStart(){
}

int aStart::getH(aStartNode a, aStartNode b){
	return (10*(abs(a.x-b.x)) + 10*(abs(a.y-b.y)));
}

void aStart::createMap(int c, int r){
	n = c;
	m = r;
	for(int i = 0; i < n; i++){
		for(int j = 0; j < m; j++){
			std::cin >> map[i][j];
			if(map[i][j] == 's'){
				start.x = i;
				start.y = j;
			}
			if(map[i][j] == 'e'){
				fin.x = i;
				fin.y = j;
			}
			node[i][j].x = i;
			node[i][j].y = j;
		}
	}
}

bool aStart::isInclude(std::list<aStartNode> arr, aStartNode qur){
	std::list<aStartNode>::iterator it;
	for(it = arr.begin(); it != arr.end(); it++)
		if(it->x == qur.x && it->y == qur.y)
			return true;
	return false;
}

void aStart::getPath(int x, int y){
	if(x == start.x && y == start.y)
		return;
	else
		getPath(road[x][y].x, road[x][y].y);
	ans += road[x][y].dir;
}

std::string aStart::mainAstart(){
	openSet.push_front(start);
	while(!openSet.empty()){
		openSet.sort(cmp);
		aStartNode now = openSet.front();
		if(now.x == fin.x && now.y == fin.y){
			getPath(fin.x, fin.y);
			return ans;
		}
		openSet.pop_front();
		closeSet.push_back(now);
		for(int i = 0; i < 4; i++){
			aStartNode nei = node[now.x+dirx[i]][now.y+diry[i]];
			if(map[now.x+dirx[i]][now.y+diry[i]] == '#')
				continue;
			if(now.x+dirx[i] > n || now.x+dirx[i] < 0 || now.y+diry[i] > m || now.y+diry[i] < 0)
				continue;
			if(isInclude(closeSet, nei))
				continue;
			aStartNode tmp;
			tmp.g = now.g + 10; 
			if(!isInclude(openSet, nei))
				tmp.isBatter = true;
			else if(tmp.g < nei.g)
				tmp.isBatter = true;
			else
				tmp.isBatter = false;
			if(tmp.isBatter){
				road[nei.x][nei.y].x = now.x;
				road[nei.x][nei.y].y = now.y;
				if(i == 3)  
					road[nei.x][nei.y].dir = 'S';  
            	else if(i == 2)   
					road[nei.x][nei.y].dir = 'D';  
            	else if(i == 1)   
					road[nei.x][nei.y].dir = 'A';  
            	else if(i == 0)   
					road[nei.x][nei.y].dir = 'W'; 
				nei.g = tmp.g;
				nei.h = getH(nei, fin);
				nei.f = nei.g + nei.h;
				openSet.push_back(nei);
			}
		}
	}
	if(openSet.empty())
		return "";
}
