////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file util.h
/// @author Matt Whitesides CS5400
/// @brief Utilities Header
///   Contains the header definitions for the added ai releated utilities
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef UTIL_H
#define UTIL_H

#include <algorithm>
#include <climits>
#include <iostream>
#include <string>
#include <unordered_map>
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
  int hist;
  bool isQuiet;
  Move_Util() : inCheck(false), h(-1), hist(0), isQuiet(true) {}
  Move_Util(Pos_Util _start, Pos_Util _end, bool _isQuiet = true) {
    start = _start;
    end = _end;
    inCheck = false;
    h = -1;
    hist = 0;
    isQuiet = _isQuiet;
  }
  friend ostream& operator<<(ostream& os, const Move_Util& m);
  string toString() {
    return ((start.file + to_string(start.rank)) +
            (end.file + to_string(end.rank)));
  }
  bool operator<(const Move_Util& rhs) const { return h > rhs.h; }
  void applyMove(Piece_Util** b);
};

struct moveSortByHist {
  inline bool operator()(const Move_Util& a, const Move_Util& b) {
    return (a.hist > b.hist);
  }
};

enum Algorithm {
  DLMM,        // Depth Limited MiniMax
  ABDLMM,      // Depth Limited MiniMax w/ Alpha Beta Pruning
  ABDLMMFS,    // Fail Soft Depth Limited MiniMax w/ Alpha Beta Pruning
  QSHTABDLMM,  // Depth Limited MiniMax w/ Alpha Beta Pruning, History Table &
               // Quiescence
};
ostream& operator<<(ostream& os, const Algorithm a);

Move_Util getBestMove(Algorithm a, int d, Piece_Util** b, bool team,
                      unordered_map<string, int>* histTable = NULL);
void updateHistTable(unordered_map<string, int>* histTable, string moveStr);
int heuristic(Piece_Util** b, bool team);
int depthLimitedMiniMax(Piece_Util** b, int d, bool isMax, bool team);
int alphaBetaDLMM(Piece_Util** board, int d, bool team);
int alphaBetaMax(Piece_Util** board, int d, int a, int b, bool team);
int alphaBetaMin(Piece_Util** board, int d, int a, int b, bool team);
int alphaBetaFailSoftDLMM(Piece_Util** board, int d, int a, int b, bool isMax,
                          bool team);
int alphaBetaQSHTDLMM(Piece_Util** board, int d, bool team,
                      unordered_map<string, int>* histTable);
int alphaBetaQSHTMax(Piece_Util** board, int d, int a, int b, bool team,
                     unordered_map<string, int>* histTable, string moveStr = "",
                     bool isQuiet = true);
int alphaBetaQSHTMin(Piece_Util** board, int d, int a, int b, bool team,
                     unordered_map<string, int>* histTable, string moveStr = "",
                     bool isQuiet = true);

vector<Move_Util> getPlayerMoves(Piece_Util** b, bool team);
vector<Move_Util> getHistSortedPlayerMoves(
    Piece_Util** b, bool team, unordered_map<string, int>* histTable);
vector<Move_Util> getPawnMoves(Piece_Util** b, int x, int y, bool team);
vector<Move_Util> getKnightMoves(Piece_Util** b, int x, int y, bool team);
vector<Move_Util> getRookMoves(Piece_Util** b, int x, int y, bool team);
vector<Move_Util> getBishopMoves(Piece_Util** b, int x, int y, bool team);
vector<Move_Util> getKingMoves(Piece_Util** b, int x, int y, bool team);
bool checkAndPushMove(Piece_Util** b, vector<Move_Util>* moves, int x, int y,
                      bool team, Pos_Util start);
bool isPos(int x);

bool inCheck(Piece_Util** b, bool team, bool useBoard = false);
bool inCheck(Piece_Util** b, Move_Util m, bool team);
bool inCheckMate(Piece_Util** b, bool team);
void printBoard(Piece_Util** b);

Piece_Util** initBoard(Piece_Util** copy = NULL);
Piece_Util** initBoard(Piece_Util** copy, Move_Util m);
void clean(Piece_Util** b);

Pos_Util idxToPos(int x, int y);
Pos_Util getKing(Piece_Util** b, bool team);

#endif