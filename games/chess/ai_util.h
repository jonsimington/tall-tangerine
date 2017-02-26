#include <iostream>
#include <vector>
#include <string>
using namespace std;

struct Pos_Util {
  int rank;
  string file;
  int x;
  int y;
Pos_Util() : rank(1), file("a"), x(0), y(0) {}
  friend ostream& operator<<(ostream& os, const Pos_Util& p);
};

struct Move_Util {
  Pos_Util start;
  Pos_Util end;
  bool inCheck;
Move_Util() : inCheck(false) {}
Move_Util(Pos_Util _start, Pos_Util _end, bool _inCheck) {
  start = _start;
  end = _end;
  inCheck = _inCheck;
}
  friend ostream& operator<<(ostream& os, const Move_Util& m);
};

struct Piece_Util {
  int type;
  bool hasMoved;
  Piece_Util() : type(0), hasMoved(false) {}
};

Piece_Util** initBoard(Piece_Util** copy = NULL);
Pos_Util idxToPos(int x, int y);
bool isPos(int x);
bool inCheck(Piece_Util** b, Move_Util m, bool team);
void clean(Piece_Util** b);
void applyMove(Piece_Util** b, Move_Util m);
void loadPiece(Piece_Util** b, int rank, string file, string type, bool team, bool hasMoved);
void printBoard(Piece_Util** b);
Pos_Util getKing(Piece_Util** b, bool team);
vector<Move_Util> getPlayerMoves(Piece_Util** b, bool team);
vector<Move_Util> getPawnMoves(Piece_Util** b, int x, int y, bool team);
vector<Move_Util> getKnightMoves(Piece_Util** b, int x, int y, bool team);
vector<Move_Util> getRookMoves(Piece_Util** b, int x, int y, bool team);
vector<Move_Util> getBishopMoves(Piece_Util** b, int x, int y, bool team);
vector<Move_Util> getKingMoves(Piece_Util** b, int x, int y, bool team);
