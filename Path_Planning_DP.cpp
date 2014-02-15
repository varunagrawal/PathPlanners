//This program plans a path using Dynamic Programming

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



vector< vector<int> > setValues(vector< vector<int> > grid, vector<int> start, vector<int> goal){
	for(int i=0;i<grid.size();i++){
		for(int j=0;j<grid[i].size();j++){
			if(grid[i][j] == -1){
				continue;
			}else if(i == goal[0] && j == goal[1]){
				grid[i][j] = 0;	
			}else{
				grid[i][j] = 100;
			}
		}
	}
	
	return grid;
}


vector< vector<int> > dp(vector< vector<int> > grid, vector<int> start, vector<int> goal){
	
	grid = setValues(grid, start, goal);		
	int moves[3] = {-1, 0, 1};
	
	 
	bool change = true;
	while(change == true){
		change = false;
    	
		for(int x=0;x<grid.size();x++){
			for(int y=0;y<grid[x].size();y++){
			
				if(grid[x][y] == -1)
					continue;
				
				if(x == goal[0] && y == goal[1]){
					if(grid[x][y] > 0){
						grid[x][y] = 0;
						change = true;
					}		
				}else{
		
					for(int i=0;i<3;i++){
						for(int j=0;j<3;j++){
						
							int X = x + moves[i];
							int Y = y + moves[j];
						
						
							if(X >= 0 && X < grid.size() && Y >=0 && Y < grid[x].size() && grid[X][Y] != -1){
						
								if(grid[X][Y]+1 < grid[x][y]){
									grid[x][y] = grid[X][Y]+1;
									change = true;
								}
								
							}
									
						}

					}
				}
			}	
		}
	}
	
	return grid;
	
}

vector<string> setPath(vector<string> map, vector< vector<int> > grid, vector<int> start, vector<int> goal){
	
	int x, y;
	x = start[0];
	y = start[1];
	int moves[3] = {-1, 0, 1};
	int min = grid[start[0]][start[1]];
	
	bool moved = true;
	
	while(true){
		if(x == goal[0] && y == goal[1])
			break;
			
		if(!moved){
			printf("No path from Start to Goal!");
			break;
		}
		moved = false;

		int tx, ty;
		
		for(int i=0;i<3;i++){
			for(int j=0;j<3;j++){
						
				int X = x + moves[i];
				int Y = y + moves[j];
				
				if(X >= 0 && X < grid.size() && Y >= 0 && Y < grid[x].size() && grid[X][Y] != -1){
					if(grid[X][Y] < min){
						min = grid[X][Y];
						tx = X;
						ty = Y;
						moved = true;
					}
				}
				
			}
		}	
		map[x][y] = 'P';
		x = tx;
		y = ty;
    	
	}	
	
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

    //vector< vector<int> > h = setHeuristic(grid, goal);

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
    //vector< vector<int> > path = a_star(grid, h, start, goal);
    //printf("Done\n\n");

    //Numerical representation of map. Uncomment to print.
    /*for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++)
            printf("%3d", path[i][j]);
        printf("\n");
    }
    printf("\n");*/
    

    //vector< vector<int> > path = a_star(grid, h, start, goal);
    
    vector< vector<int> > path = dp(grid, start, goal);
    
	vector<string> ans = setPath(map, path, start, goal);
	
    /*vector<string> ans = map;
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            if(path[i][j] == 1)
                ans[i][j] = 'P';
        }
    }*/
    
    
    ans[start[0]][start[1]] = 'R';

    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++)
            printf("%3c", ans[i][j]);
        printf("\n");
    }

    //system("pause");
    return 0;
}
