#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>//for sort
#include <vector>
using namespace std;

void CellSort(vector<vector<int>>*);
bool Compare(const vector<int>,const vector<int> );

enum class State{kObstacle,kEmpty,kPath,kClosed,kStart,kFinish};
const int delta[4][2]{{-1, 0}, {0, -1}, {1, 0}, {0, 1}};

string CellString(enum State);

vector<State> ParseLine(string line) {
    istringstream sline(line);
    int n;
    char c;
    vector<State> row;
    while (sline >> n >> c && c == ',') {
      if(n==0)
      row.push_back(State::kEmpty);
      else
        row.push_back(State::kObstacle);
    }
    return row;
}

string CellString(State cell) {
  switch(cell) {
    case State::kObstacle: return "|   ";
    case State::kPath: return "C   ";
     case State::kStart: return"S   ";
    case State::kFinish: return"F   ";
        default: return "0   ";
  }
}

void CellSort(vector<vector<int>> *v)
{sort(v->begin(), v->end(), Compare);
}

void PrintBoard(const vector<vector<State>> board) {
  for (int i = 0; i < board.size(); i++) {
    for (int j = 0; j < board[i].size(); j++)
     {
      cout<<CellString(board[i][j]);
     }
     cout<<std::endl;
  }
}



vector<vector<State>> ReadBoardFile(string path) {
  ifstream myfile (path);
  vector<vector<State>> board{};
    if(myfile) {
    string line;
    while (getline(myfile, line)) {
      vector<State> row = ParseLine(line);
      board.push_back(row);
    }
  }
  return board;
}

bool Compare(const vector<int>a,const vector<int> b)
{
    int f1 = a[2] + a[3]; // f1 = g1 + h1
    int f2 = b[2] + b[3]; // f2 = g2 + h2
    return f1 > f2;
}

int Heuristic(int x1,int y1,int x2,int y2)
{
    return abs(x2-x1)+abs(y2-y1);
}

void AddToOpen(int x,int y,int g,int h,vector<vector<int>>& open,vector<vector<State>>& grid)
{
    vector<int> row{x,y,g,h};
    open.push_back(row);
    grid[x][y]=State::kClosed;
}

bool CheckValidCell(int x, int y, vector<vector<State>> &grid) {
  bool on_grid_x = (x >= 0 && x < grid.size());
  bool on_grid_y = (y >= 0 && y < grid[0].size());
  if (on_grid_x && on_grid_y)
    return grid[x][y] == State::kEmpty;
  return false;
}

void ExpandNeighbors(vector<int>& current,int goal[2],vector<vector<int>>& open,vector<vector<State>>& grid)
{
  int x=current[0];
  int y=current[1];
  int g=current[2];

  for(int i=0;i<4;i++)
  {
    int x2=x+delta[i][0];
    int y2=y+delta[i][1];

    if(CheckValidCell(x2,y2,grid))
    {
      int g2=g+1;
      int h2=Heuristic(x2,y2,goal[0],goal[1]);
      AddToOpen(x2,y2,g2,h2,open,grid);
    }
  }
}

vector<vector<State>> Search(vector<vector<State>> grid,int init[2],int goal[2])
{
     vector<vector<int>> open{};

     int x=init[0];
     int y=init[1];
     int g=0;
     int h=Heuristic(x,y,goal[0],goal[1]);
     AddToOpen(x,y,g,h,open,grid);
     while(open.size()>0)
     {
         CellSort(&open);
         auto current=open.back();
         open.pop_back();
         x=current[0];
         y=current[1];
         grid[x][y]=State::kPath;

    if (x == goal[0] && y == goal[1]) {
      grid[0][0]=State::kStart;
      grid[4][5]=State::kFinish;

      return grid;
    }

    ExpandNeighbors(current,goal,open,grid);
     }

     cout<<"No! path found";
     return vector<vector<State>> {};
}

int main()
{
    int init[2] {0,0};
    int goal[2] {4,5};
auto board=ReadBoardFile("1.board");
auto solution=Search(board,init,goal);
    PrintBoard(solution);
}
