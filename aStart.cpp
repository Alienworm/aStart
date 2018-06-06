#include "aStart.h"

int main(){
	int n, m;
	aStart t;
	std::cin >> n >> m;
	t.createMap(n, m);
	std::cout << t.mainAstart() << std::endl;
}
/*
test:
10 10
s...#.....
....#.....
.########.
........#.
..........
........#.
........#.
...######.
..........
.........e
*/
