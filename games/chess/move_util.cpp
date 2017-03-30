#include "util.h"

ostream& operator<<(ostream& os, const Move_Util& m) {
  os << m.start << " -> " << m.end << " | H: " << m.h;
  return os;
}

vector<Move_Util> getPlayerMoves(Piece_Util** b, bool team) {
  vector<Move_Util> moves;

  for (int y = 2; y < 10; ++y) {
    for (int x = 2; x < 10; ++x) {
      // Empty or Invalid Square Check
      if (b[x][y].type == 0 || b[x][y].type == 7) {
        continue;
      }

      // Other Players Character Check
      if (team) {
        if (b[x][y].type < 0) {
          continue;
        }
      } else {
        if (b[x][y].type > 0) {
          continue;
        }
      }

      vector<Move_Util> pieceMoves;
      switch (abs(b[x][y].type)) {
        case 1:
          pieceMoves = getPawnMoves(b, x, y, team);
          break;
        case 2:
          pieceMoves = getKnightMoves(b, x, y, team);
          break;
        case 3:
          pieceMoves = getBishopMoves(b, x, y, team);
          break;
        case 4:
          pieceMoves = getRookMoves(b, x, y, team);
          break;
        case 5:
          pieceMoves = getRookMoves(b, x, y, team);
          moves.insert(moves.end(), pieceMoves.begin(), pieceMoves.end());
          pieceMoves = getBishopMoves(b, x, y, team);
          break;
        case 6:
          pieceMoves = getKingMoves(b, x, y, team);
          break;
      }
      moves.insert(moves.end(), pieceMoves.begin(), pieceMoves.end());
    }
  }

  return moves;
}

vector<Move_Util> getPawnMoves(Piece_Util** b, int x, int y, bool team) {
  int dir = (team) ? -1 : 1;
  vector<Move_Util> moves;
  Pos_Util start = idxToPos(x, y);

  // Move one space
  if (b[x][y + dir].type == 0) {
    checkAndPushMove(b, &moves, x, (y + dir), team, start);
  }

  // Move two spaces from the start
  if (team) {
    if (y == 8) {
      if (b[x][y + dir].type == 0 && b[x][y + (dir * 2)].type == 0) {
        checkAndPushMove(b, &moves, x, (y + (dir * 2)), team, start);
      }
    }
  } else {
    if (y == 3) {
      if (b[x][y + dir].type == 0 && b[x][y + (dir * 2)].type == 0) {
        checkAndPushMove(b, &moves, x, (y + (dir * 2)), team, start);
      }
    }
  }

  // Capture Piece
  if (b[x + 1][y + dir].type != 0) {
    checkAndPushMove(b, &moves, (x + 1), (y + dir), team, start);
  }
  if (b[x - 1][y + dir].type != 0) {
    checkAndPushMove(b, &moves, (x - 1), (y + dir), team, start);
  }

  // En Passent
  if ((b[x + 1][y].type == dir) && (b[x + 1][y].hasMoved)) {
    moves.push_back(Move_Util(start, idxToPos((x + 1), (y + dir))));
  }
  if ((b[x - 1][y].type == dir) && (b[x - 1][y].hasMoved)) {
    moves.push_back(Move_Util(start, idxToPos((x - 1), (y + dir))));
  }

  return moves;
}

vector<Move_Util> getKnightMoves(Piece_Util** b, int x, int y, bool team) {
  vector<Move_Util> moves;
  Pos_Util start = idxToPos(x, y);

  if (abs(b[x][y].type) != 2) {
    return moves;
  }

  checkAndPushMove(b, &moves, (x - 2), (y + 1), team, start);
  checkAndPushMove(b, &moves, (x - 1), (y + 2), team, start);
  checkAndPushMove(b, &moves, (x + 2), (y + 1), team, start);
  checkAndPushMove(b, &moves, (x + 1), (y + 2), team, start);
  checkAndPushMove(b, &moves, (x + 2), (y - 1), team, start);
  checkAndPushMove(b, &moves, (x + 1), (y - 2), team, start);
  checkAndPushMove(b, &moves, (x - 2), (y - 1), team, start);
  checkAndPushMove(b, &moves, (x - 1), (y - 2), team, start);

  return moves;
}

vector<Move_Util> getRookMoves(Piece_Util** b, int x, int y, bool team) {
  vector<Move_Util> moves;
  Pos_Util start = idxToPos(x, y);
  int i = 0;

  // Up
  while (true) {
    ++i;
    if (!checkAndPushMove(b, &moves, x, (y - i), team, start)) {
      break;
    }
  }

  // Down
  i = 0;
  while (true) {
    ++i;
    if (!checkAndPushMove(b, &moves, x, (y + i), team, start)) {
      break;
    }
  }

  // Left
  i = 0;
  while (true) {
    ++i;
    if (!checkAndPushMove(b, &moves, (x - i), y, team, start)) {
      break;
    }
  }

  // Right
  i = 0;
  while (true) {
    ++i;
    if (!checkAndPushMove(b, &moves, (x + i), y, team, start)) {
      break;
    }
  }

  return moves;
}

vector<Move_Util> getBishopMoves(Piece_Util** b, int x, int y, bool team) {
  vector<Move_Util> moves;
  Pos_Util start = idxToPos(x, y);
  int i = 0;

  // Up Left
  i = 0;
  while (true) {
    ++i;
    if (!checkAndPushMove(b, &moves, (x - i), (y - i), team, start)) {
      break;
    }
  }

  // Up Right
  i = 0;
  while (true) {
    ++i;
    if (!checkAndPushMove(b, &moves, (x + i), (y - i), team, start)) {
      break;
    }
  }

  // Down Left
  i = 0;
  while (true) {
    ++i;
    if (!checkAndPushMove(b, &moves, (x - i), (y + i), team, start)) {
      break;
    }
  }

  // Down Right
  i = 0;
  while (true) {
    ++i;
    if (!checkAndPushMove(b, &moves, (x + i), (y + i), team, start)) {
      break;
    }
  }

  return moves;
}

vector<Move_Util> getKingMoves(Piece_Util** b, int x, int y, bool team) {
  vector<Move_Util> moves;
  Pos_Util start = idxToPos(x, y);

  if (abs(b[x][y].type) != 6) {
    return moves;
  }

  // Up
  checkAndPushMove(b, &moves, x, (y - 1), team, start);

  // Down
  checkAndPushMove(b, &moves, x, (y + 1), team, start);

  // Left
  checkAndPushMove(b, &moves, (x - 1), y, team, start);

  // Right
  checkAndPushMove(b, &moves, (x + 1), y, team, start);

  // Up Left
  checkAndPushMove(b, &moves, (x - 1), (y - 1), team, start);

  // Up Right
  checkAndPushMove(b, &moves, (x + 1), (y - 1), team, start);

  // Down Left
  checkAndPushMove(b, &moves, (x - 1), (y + 1), team, start);

  // Down Right
  checkAndPushMove(b, &moves, (x + 1), (y + 1), team, start);

  // Castle
  int i = 0;
  if (!b[x][y].hasMoved) {
    // Left
    i = 0;
    while (true) {
      ++i;
      if (b[x - i][y].type == 0) {
        continue;
      }
      if ((abs(b[x - i][y].type) == 4) && (!b[x - i][y].hasMoved)) {
        Move_Util m = Move_Util(start, idxToPos(x - 1, y));
        if (!inCheck(b, m, team)) {
          m.end = idxToPos(x - 2, y);
          moves.push_back(m);
        }
      } else {
        break;
      }
    }

    // Right
    i = 0;
    while (true) {
      ++i;
      if (b[x + i][y].type == 0) {
        continue;
      }
      if ((abs(b[x + i][y].type) == 4) && (!b[x + i][y].hasMoved)) {
        Move_Util m = Move_Util(start, idxToPos(x + 1, y));
        if (!inCheck(b, m, team)) {
          m.end = idxToPos(x + 2, y);
          moves.push_back(m);
        }
      } else {
        break;
      }
    }
  }

  return moves;
}

bool checkAndPushMove(Piece_Util** b, vector<Move_Util>* moves, int x, int y,
                      bool team, Pos_Util start) {
  if (b[x][y].type != 7) {
    if ((team != isPos(b[x][y].type)) || (b[x][y].type == 0)) {
      moves->push_back(Move_Util(start, idxToPos(x, y)));

      if (b[x][y].type != 0) {
        return false;
      }
      return true;
    }
  }

  return false;
}

void Move_Util::applyMove(Piece_Util** b) {
  int type = b[start.x][start.y].type;
  b[start.x][start.y].type = 0;
  b[end.x][end.y].type = type;
}

bool isPos(int x) { return (x >= 0); }