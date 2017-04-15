////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file ai_util.cpp
/// @author Matt Whitesides CS5400
/// @brief AI Utilities
///   Functions and Structures to assist in AI logic.
///   0 -- Empty
///   1 -- Pawn
///   2 -- Knight
///   3 -- Bishop
///   4 -- Rook
///   5 -- Queen
///   6 -- King
///   7 -- Invaliid
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "util.h"

ostream& operator<<(ostream& os, const Algorithm a) {
  switch (a) {
    case DLMM:
      os << "Depth Limited Mini Max";
      break;
    case ABDLMM:
      os << "Depth Limited Mini Max w/ Alpha Beta Purning";
      break;
    case ABDLMMFS:
      os << "Fail Soft Depth Limited Mini Max w/ Alpha Beta Purning";
      break;
    case QSHTABDLMM:
      os << "Depth Limited MiniMax w/ Alpha Beta Pruning, History Table & "
            "Quiescence ";
      break;
    default:
      os << "None";
      break;
  }
  return os;
}

int heuristic(Piece_Util** b, bool team) {
  // 39 Being all the other teams pieces are gone.
  // When the opponent has all their pieces the h will equal 0;
  int h = 39;

  // Reward putting the other team in check
  if (inCheck(b, !team)) {
    h += 5;
    if (inCheckMate(b, !team)) {
      return INT_MAX;
    }
  }

  for (int y = 2; y < 10; ++y) {
    for (int x = 2; x < 10; ++x) {
      if (!team) {
        switch (b[x][y].type) {
          case 1:
            --h;
            break;
          case 2:
          case 3:
            h -= 3;
            break;
          case 4:
            h -= 5;
            break;
          case 5:
            h -= 9;
            break;
        }
      } else {
        switch (b[x][y].type) {
          case -1:
            --h;
            break;
          case -2:
          case -3:
            h -= 3;
            break;
          case -4:
            h -= 5;
            break;
          case -5:
            h -= 9;
            break;
        }
      }
    }
  }

  return h;
}

int depthLimitedMiniMax(Piece_Util** b, int d, bool isMax, bool team) {
  if (d <= 0) {
    return heuristic(b, team);
  }

  int v = 0;

  if (isMax) {
    v = INT_MIN;

    for (Move_Util m : getPlayerMoves(b, team)) {
      Piece_Util** tempBoard = initBoard(b, m);
      v = max(v, depthLimitedMiniMax(tempBoard, --d, false, team));
      clean(tempBoard);
    }

    return v;
  } else {
    v = INT_MAX;

    for (Move_Util m : getPlayerMoves(b, !team)) {
      Piece_Util** tempBoard = initBoard(b, m);
      v = min(v, depthLimitedMiniMax(tempBoard, --d, true, team));
      clean(tempBoard);
    }

    return v;
  }

  return -1;
}

int alphaBetaFailSoftDLMM(Piece_Util** board, int d, int a, int b, bool isMax,
                          bool team) {
  if (d <= 0) {
    return heuristic(board, team);
  }

  int v = 0;

  if (isMax) {
    v = INT_MIN;

    for (Move_Util m : getPlayerMoves(board, team)) {
      Piece_Util** tempBoard = initBoard(board, m);

      v = max(v, alphaBetaFailSoftDLMM(tempBoard, --d, a, b, false, team));
      a = max(a, v);

      clean(tempBoard);

      if (b <= a) {
        break;
      }
    }

    return v;
  } else {
    v = INT_MAX;

    for (Move_Util m : getPlayerMoves(board, !team)) {
      Piece_Util** tempBoard = initBoard(board, m);

      v = min(v, alphaBetaFailSoftDLMM(tempBoard, --d, a, b, true, team));
      b = min(b, v);

      clean(tempBoard);

      if (b <= a) {
        break;
      }
    }

    return v;
  }

  return -1;
}

int alphaBetaDLMM(Piece_Util** board, int d, bool team) {
  return alphaBetaMax(board, d, INT_MIN, INT_MAX, team);
}

int alphaBetaMax(Piece_Util** board, int d, int a, int b, bool team) {
  if (d <= 0) {
    return heuristic(board, team);
  }

  int v = INT_MIN;

  for (Move_Util m : getPlayerMoves(board, team)) {
    Piece_Util** tempBoard = initBoard(board, m);

    v = max(v, alphaBetaMin(tempBoard, --d, a, b, team));

    clean(tempBoard);
    if (v >= b) {
      return v;
    }

    a = max(a, v);
  }

  return v;
}

int alphaBetaMin(Piece_Util** board, int d, int a, int b, bool team) {
  if (d <= 0) {
    return heuristic(board, team);
  }

  int v = INT_MAX;

  for (Move_Util m : getPlayerMoves(board, !team)) {
    Piece_Util** tempBoard = initBoard(board, m);

    v = min(v, alphaBetaMax(tempBoard, --d, a, b, team));

    clean(tempBoard);
    if (v <= a) {
      return v;
    }

    b = min(b, v);
  }

  return v;
}

int alphaBetaQSHTDLMM(Piece_Util** board, int d, bool team,
                      unordered_map<string, int>* histTable) {
  return alphaBetaQSHTMax(board, d, INT_MIN, INT_MAX, team, histTable);
}

int alphaBetaQSHTMax(Piece_Util** board, int d, int a, int b, bool team,
                     unordered_map<string, int>* histTable, string moveStr,
                     bool isQuiet) {
  if (d <= 0 && isQuiet) {
    updateHistTable(histTable, moveStr);
    return heuristic(board, team);
  }

  int v = INT_MIN;

  vector<Move_Util> moves = getHistSortedPlayerMoves(board, team, histTable);
  for (Move_Util m : moves) {
    Piece_Util** tempBoard = initBoard(board, m);

    v = max(v, alphaBetaQSHTMin(tempBoard, --d, a, b, team, histTable,
                                m.toString(), m.isQuiet));

    clean(tempBoard);
    if (v >= b) {
      updateHistTable(histTable, m.toString());
      return v;
    }

    a = max(a, v);
  }

  return v;
}

int alphaBetaQSHTMin(Piece_Util** board, int d, int a, int b, bool team,
                     unordered_map<string, int>* histTable, string moveStr,
                     bool isQuiet) {
  if (d <= 0 && isQuiet) {
    updateHistTable(histTable, moveStr);
    return heuristic(board, team);
  }

  int v = INT_MAX;

  vector<Move_Util> moves = getHistSortedPlayerMoves(board, team, histTable);
  for (Move_Util m : moves) {
    Piece_Util** tempBoard = initBoard(board, m);

    v = min(v, alphaBetaQSHTMax(tempBoard, --d, a, b, team, histTable,
                                m.toString(), m.isQuiet));

    clean(tempBoard);
    if (v <= a) {
      updateHistTable(histTable, m.toString());
      return v;
    }

    b = min(b, v);
  }

  return v;
}

void updateHistTable(unordered_map<string, int>* histTable, string moveStr) {
  if (histTable->find(moveStr) != histTable->end()) {
    (*histTable)[moveStr] = ((*histTable)[moveStr] + 1);
  } else {
    histTable->insert({moveStr, 0});
  }
}

Move_Util getBestMove(Algorithm a, int d, Piece_Util** b, bool team,
                      unordered_map<string, int>* histTable) {
  vector<Move_Util> moves = getPlayerMoves(b, team);

  if (moves.size() == 0) {
    return Move_Util();
  }

  for (Move_Util& m : moves) {
    Piece_Util** tempBoard = initBoard(b, m);

    switch (a) {
      case DLMM:
        m.h = depthLimitedMiniMax(tempBoard, d, true, team);
        break;
      case ABDLMM:
        m.h = alphaBetaDLMM(tempBoard, d, team);
        break;
      case ABDLMMFS:
        m.h = alphaBetaFailSoftDLMM(tempBoard, d, INT_MIN, INT_MAX, true, team);
        break;
      case QSHTABDLMM:
        m.h = alphaBetaQSHTDLMM(tempBoard, d, team, histTable);
        break;
      default:
        m.h = -1;
    }

    clean(tempBoard);
  }

  // Sort moves by huristic value
  sort(moves.begin(), moves.end());

  // Get the all moves that match the best huristic
  int moveCount = (unsigned int)moves.size();
  int maxHur = moves[0].h;
  int i = 0;
  for (i = 0; (moves[i].h == maxHur) && (i < moveCount); ++i)
    ;

  // Randomize moves that have the same top huristic
  random_shuffle(moves.begin(), moves.begin() + i);

  // Only pick a move not leaving you in check
  for (i = 0; (i < moveCount) && (inCheck(b, moves[i], team)); ++i)
    ;

  // for (auto m : moves) {
  //   cout << m << endl;
  // }
  // cout << endl;

  // cout << "Hist Table: \n";
  // for (auto it : *histTable) {
  //   cout << " " << it.first << " : " << it.second << endl;
  //   ;
  // }

  return moves[i];
}