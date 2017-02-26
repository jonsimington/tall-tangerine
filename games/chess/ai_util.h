#include <iostream>
#include <vector>
#include <string>
using namespace std;

struct Pos {
  int rank;
  string file;
  int x;
  int y;
Pos() : rank(1), file("a"), x(0), y(0) {}
  friend ostream& operator<<(ostream& os, const Pos& p);
};

struct Move_Util {
  Pos start;
  Pos end;
  bool inCheck;
Move_Util() : inCheck(false) {}
Move_Util(Pos _start, Pos _end, bool _inCheck) {
  start = _start;
  end = _end;
  inCheck = _inCheck;
}
  friend ostream& operator<<(ostream& os, const Move_Util& m);
};

int** initBoard(int** copy = NULL);
Pos idxToPos(int x, int y);
bool isPos(int x);
bool inCheck(int** b, Move_Util m, bool team);
void clean(int** b);
void applyMove(int** b, Move_Util m);
void loadPiece(int** b, int rank, string file, string type, bool team);
void printBoard(int** b);
Pos getKing(int** b, bool team);
vector<Move_Util> getPlayerMoves(int** b, bool team);
vector<Move_Util> getPawnMoves(int** b, int x, int y, bool team);
vector<Move_Util> getKnightMoves(int** b, int x, int y, bool team);
vector<Move_Util> getRookMoves(int** b, int x, int y, bool team);
vector<Move_Util> getBishopMoves(int** b, int x, int y, bool team);
vector<Move_Util> getKingMoves(int** b, int x, int y, bool team);
