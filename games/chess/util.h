#ifndef UTIL_H
#define UTIL_H

#include <algorithm>
#include <climits>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

struct Pos_Util {
  int rank;
  string file;
  int x;
  int y;
  Pos_Util() : rank(1), file("a"), x(0), y(0) {}
  friend ostream& operator<<(ostream& os, const Pos_Util& p);
};

struct Piece_Util {
  int type;
  bool hasMoved;
  Piece_Util() : type(0), hasMoved(false) {}
};

struct Move_Util {
  Pos_Util start;
  Pos_Util end;
  bool inCheck;
  int h;
  Move_Util() : inCheck(false), h(-1) {}
  friend ostream& operator<<(ostream& os, const Move_Util& m);
  bool operator<(const Move_Util& rhs) const { return h > rhs.h; }
  void applyMove(Piece_Util** b);
};

enum Algorithm {
  DLMM,   // Depth Limited MiniMax
  ABDLMM  // Depth Limited MiniMax w/ Alpha Beta Pruning
};
ostream& operator<<(ostream& os, const Algorithm a);

Move_Util getBestMove(Algorithm a, int d, Piece_Util** b, bool team);
int heuristic(Piece_Util** b, bool team);
int depthLimitedMiniMax(Piece_Util** b, int d, bool isMax, bool team);
int alphaBetaDLMM(Piece_Util** board, int d, int a, int b, bool isMax,
                  bool team);
                  
vector<Move_Util> getPlayerMoves(Piece_Util** b, bool team);
vector<Move_Util> getPawnMoves(Piece_Util** b, int x, int y, bool team);
vector<Move_Util> getKnightMoves(Piece_Util** b, int x, int y, bool team);
vector<Move_Util> getRookMoves(Piece_Util** b, int x, int y, bool team);
vector<Move_Util> getBishopMoves(Piece_Util** b, int x, int y, bool team);
vector<Move_Util> getKingMoves(Piece_Util** b, int x, int y, bool team);
bool isPos(int x);

bool inCheck(Piece_Util** b, bool team, bool useBoard = false);
bool inCheck(Piece_Util** b, Move_Util m, bool team);
void printBoard(Piece_Util** b);

Piece_Util** initBoard(Piece_Util** copy = NULL);
Piece_Util** initBoard(Piece_Util** copy, Move_Util m);
void clean(Piece_Util** b);

Pos_Util idxToPos(int x, int y);
Pos_Util getKing(Piece_Util** b, bool team);

#endif