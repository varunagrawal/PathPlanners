#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<string>
#include<algorithm>
#include<cmath>
#include<fstream>
#include<vector>

using namespace std;

vector<string> getMap(){
    ifstream fin;
    fin.open("map2.txt");
    
    if(fin.fail()){
		printf("Please make sure the map file is in the same directory!\n");
		exit(1);
    }

    string s;
    vector<string> map;
    map.clear();

    while(true){
        getline(fin, s);

        if(fin.eof())
            break;

		if(s == "") break;
		
        map.push_back(s);
    }

    return map;
}

vector< vector<int> > setGrid(vector<string> map){
    vector< vector<int> > grid;
    grid.clear();

    int rows = map.size();
    int cols = map[0].length();

    grid.resize(rows);
    for(int i=0;i<rows;i++){
        grid[i].resize(cols);

        for(int j=0;j<cols;j++){

            if(map[i][j] == '#')
                grid[i][j] = -1;
            else if(map[i][j] == '0')
                grid[i][j] = 0;
            else if(map[i][j] == 'R')
                grid[i][j] = 2;
            else if(map[i][j] == 'S')
                grid[i][j] = 3;

        }
    }

    return grid;
}

vector<int> getGoal(vector< vector<int> > grid){
    vector<int> goal;
    goal.resize(2);

    for(int i=0;i<grid.size();i++){
        for(int j=0;j<grid[i].size();j++){
            if(grid[i][j] == 3){
                goal[0] = i;
                goal[1] = j;

                return goal;
            }
        }
    }

}

vector<int> getStart(vector< vector<int> > grid){
    vector<int> start;
    start.resize(2);

    for(int i=0;i<grid.size();i++){
        for(int j=0;j<grid[i].size();j++){
            if(grid[i][j] == 2){
                start[0] = i;
                start[1] = j;

                return start;
            }
        }
    }

}

vector< vector<int> > setHeuristic(vector< vector<int> > h, vector<int> goal){
    for(int i=0;i<h.size();i++){
        for(int j=0;j<h[i].size();j++){
            h[i][j] = abs(goal[0]-i) + abs(goal[1]-j);
        }
    }

    return h;
}

vector< vector<int> > a_star(vector< vector<int> > grid, vector< vector<int> > h, vector<int> s, vector<int> goal){
	int moves[3] = {-1, 0, 1};
	int rows = grid.size(), cols = grid[0].size();
	
	vector< vector<int> > open, close;
	open.clear();
	
	close = grid;
	
    	vector<int> start, next;
    	start.resize(5);
    	next.resize(5);
	
	start[0] = start[2] = h[s[0]][s[1]];
	start[1] = 0;
	start[3] = s[0];
	start[4] = s[1];
	open.push_back(start);
	
    	while(true){
		
		if(open.empty()){
			printf("No path from Start to Goal!\n");
			break;
		}
	
		sort(open.begin(), open.end());
		reverse(open.begin(), open.end());
		start = open[open.size()-1];
		open.pop_back();
		
		if(start[3] == goal[0] && start[4] == goal[1])
			break;

		close[start[3]][start[4]] = -1;
		
		for(int i=0;i<3;i++){
			for(int j=0;j<3;j++){
				
				int x = start[3] + moves[i];
				int y = start[4] + moves[j];
		
                		if(x < 0 || y < 0)
                    			continue;
	                	else if(x >= rows || y >= cols)
        	            		continue;
                		else if(grid[x][y] == -1 || close[x][y] == -1)
					continue;
				else{
					next[1] = start[1] + 1;
					next[0] = next[1] + h[x][y];
					next[2] = h[x][y];
					next[3] = x;
					next[4] = y;
		
					open.push_back(next);
	
					close[next[3]][next[4]] = -1;	
                		}
            		}
		}

		grid[start[3]][start[4]] = 1;

	}

	return grid;
}


vector< string > setPath(vector< vector<int> > grid, vector<int> start, vector<int> goal, vector<string> map){
	int x, y;
	x = goal[0];
	y = goal[1];
	int moves[3] = {-1, 0, 1};
	
	while(true){
		if(x == start[0] && y == start[1])
			break;
		
		int X, Y, tx, ty;
		int min = 1000;
		
		for(int i=0;i<3;i++){
			for(int j=0;j<3;j++){
		
				X = x + moves[i];
				Y = y + moves[j];
				
				if(X >= grid.size() || X < 0 || Y >= grid[x].size() || Y < 0)
					continue;
				else if(grid[X][Y] == -1)
					continue;
				
				if(grid[X][Y] == 1){
					int d = abs(X - start[0]) + abs(Y - start[1]);
					
					if(d < min){
						min = d;
						tx = X;
						ty = Y;	
					}
					
				}
			}
		}
		
		grid[x][y] = 0;
		x = tx;
		y = ty;
		map[x][y] = 'P';
							
	}
	
	map[start[0]][start[1]] = 'R';
	return map;
}


int main(int argc, char** argv)
{
    //printf("Retrieving Map\n\n");
    vector< string > map = getMap();

    vector< vector<int> > grid = setGrid(map);

    int rows = grid.size();
    int cols = grid[0].size();

    vector<int> start, goal;

    start = getStart(grid);
    goal = getGoal(grid);

    vector< vector<int> > h = setHeuristic(grid, goal);

    /*printf("The map is:\n\n");
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++)
            printf("%3c", map[i][j]);
        printf("\n");
    }
    printf("\n");
	
	for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++)
            printf("%3d", h[i][j]);
        printf("\n");
    }
    printf("\n");*/
    
    //printf("Solving using A* Search\n\n");
    vector< vector<int> > path = a_star(grid, h, start, goal);
    //printf("Done\n\n");

    //Numerical representation of map. Uncomment to print.
    /*for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++)
            printf("%3d", path[i][j]);
        printf("\n");
    }*/
    
	vector<string> ans = setPath(path, start, goal, map);

	/*for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            if(path[i][j] == 1)
                ans[i][j] = 'P';
        }
    }
    ans[start[0]][start[1]] = 'R';
	*/
	
	for(int i=0;i<rows;i++){
		for(int j=0;j<cols;j++)
			printf("%3c", ans[i][j]);
		        printf("\n");
	}

    //system("pause");
    return 0;
}  

