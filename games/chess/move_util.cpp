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
  Pos_Util end;

  if (abs(b[x][y].type) != 1) {
    return moves;
  }

  // Move one space
  if (b[x][y + dir].type == 0) {
    Move_Util m;
    m.start = start;
    m.end = idxToPos(x, y + dir);
    moves.push_back(m);
  }

  if (team) {
    // Move two spaces from the start
    if (y == 8) {
      if ((b[x][y + (dir * 2)].type == 0) && (b[x][y + dir].type == 0)) {
        Move_Util m;
        m.start = start;
        m.end = idxToPos(x, y + (dir * 2));
        moves.push_back(m);
      }
    }

    // Capture Piece
    if (b[x + 1][y + dir].type < 0) {
      Move_Util m;
      m.start = start;
      m.end = idxToPos(x + 1, y + dir);
      moves.push_back(m);
    }
    if (b[x - 1][y + dir].type < 0) {
      Move_Util m;
      m.start = start;
      m.end = idxToPos(x - 1, y + dir);
      moves.push_back(m);
    }
  } else {
    // Move two spaces from the start
    if (y == 3) {
      if ((b[x][y + (dir * 2)].type == 0) && (b[x][y + dir].type == 0)) {
        Move_Util m;
        m.start = start;
        m.end = idxToPos(x, y + (dir * 2));
        moves.push_back(m);
      }
    }

    // Capture Piece
    if (b[x + 1][y + dir].type > 0 && b[x + 1][y + dir].type != 7) {
      Move_Util m;
      m.start = start;
      m.end = idxToPos(x + 1, y + dir);
      moves.push_back(m);
    }
    if (b[x - 1][y + dir].type > 0 && b[x - 1][y + dir].type != 7) {
      Move_Util m;
      m.start = start;
      m.end = idxToPos(x - 1, y + dir);
      moves.push_back(m);
    }
  }

  // En Passent
  if ((b[x + 1][y].type == dir) && (b[x + 1][y].hasMoved)) {
    Move_Util m;
    m.start = start;
    m.end = idxToPos(x + 1, y + dir);
    moves.push_back(m);
  }
  if ((b[x - 1][y].type == dir) && (b[x - 1][y].hasMoved)) {
    Move_Util m;
    m.start = start;
    m.end = idxToPos(x - 1, y + dir);
    moves.push_back(m);
  }

  return moves;
}

vector<Move_Util> getKnightMoves(Piece_Util** b, int x, int y, bool team) {
  vector<Move_Util> moves;
  Pos_Util start = idxToPos(x, y);
  Pos_Util end;

  if (abs(b[x][y].type) != 2) {
    return moves;
  }

  if (b[x - 2][y + 1].type != 7) {
    if (b[x - 2][y + 1].type == 0 || (team != isPos(b[x - 2][y + 1].type))) {
      Move_Util m;
      m.start = start;
      m.end = idxToPos(x - 2, y + 1);
      moves.push_back(m);
    }
  }

  if (b[x - 1][y + 2].type != 7) {
    if (b[x - 1][y + 2].type == 0 || (team != isPos(b[x - 1][y + 2].type))) {
      Move_Util m;
      m.start = start;
      m.end = idxToPos(x - 1, y + 2);
      moves.push_back(m);
    }
  }

  if (b[x + 2][y + 1].type != 7) {
    if (b[x + 2][y + 1].type == 0 || (team != isPos(b[x + 2][y + 1].type))) {
      Move_Util m;
      m.start = start;
      m.end = idxToPos(x + 2, y + 1);
      moves.push_back(m);
    }
  }

  if (b[x + 1][y + 2].type != 7) {
    if (b[x + 1][y + 2].type == 0 || (team != isPos(b[x + 1][y + 2].type))) {
      Move_Util m;
      m.start = start;
      m.end = idxToPos(x + 1, y + 2);
      moves.push_back(m);
    }
  }

  if (b[x + 2][y - 1].type != 7) {
    if (b[x + 2][y - 1].type == 0 || (team != isPos(b[x + 2][y - 1].type))) {
      Move_Util m;
      m.start = start;
      m.end = idxToPos(x + 2, y - 1);
      moves.push_back(m);
    }
  }

  if (b[x + 1][y - 2].type != 7) {
    if (b[x + 1][y - 2].type == 0 || (team != isPos(b[x + 1][y - 2].type))) {
      Move_Util m;
      m.start = start;
      m.end = idxToPos(x + 1, y - 2);
      moves.push_back(m);
    }
  }

  if (b[x - 2][y - 1].type != 7) {
    if (b[x - 2][y - 1].type == 0 || (team != isPos(b[x - 2][y - 1].type))) {
      Move_Util m;
      m.start = start;
      m.end = idxToPos(x - 2, y - 1);
      moves.push_back(m);
    }
  }

  if (b[x - 1][y - 2].type != 7) {
    if (b[x - 1][y - 2].type == 0 || (team != isPos(b[x - 1][y - 2].type))) {
      Move_Util m;
      m.start = start;
      m.end = idxToPos(x - 1, y - 2);
      moves.push_back(m);
    }
  }

  return moves;
}

vector<Move_Util> getRookMoves(Piece_Util** b, int x, int y, bool team) {
  vector<Move_Util> moves;
  Pos_Util start = idxToPos(x, y);
  Pos_Util end;
  int i = 0;

  if (abs(b[x][y].type) != 4 && abs(b[x][y].type) != 5) {
    return moves;
  }

  // Up
  while (true) {
    ++i;

    if (b[x][y - i].type != 0) {
      if (b[x][y - i].type == 7 || (team == isPos(b[x][y - i].type))) {
        break;
      }
    }

    if (b[x][y - i].type == 0) {
      Move_Util m;
      m.start = start;
      m.end = idxToPos(x, y - i);
      moves.push_back(m);
      continue;
    }

    if ((team != isPos(b[x][y - i].type))) {
      Move_Util m;
      m.start = start;
      m.end = idxToPos(x, y - i);
      moves.push_back(m);
      break;
    }
  }

  // Down
  i = 0;
  while (true) {
    ++i;

    if (b[x][y + i].type != 0) {
      if (b[x][y + i].type == 7 || (team == isPos(b[x][y + i].type))) {
        break;
      }
    }

    if (b[x][y + i].type == 0) {
      Move_Util m;
      m.start = start;
      m.end = idxToPos(x, y + i);
      moves.push_back(m);
      continue;
    }

    if ((team != isPos(b[x][y + i].type))) {
      Move_Util m;
      m.start = start;
      m.end = idxToPos(x, y + i);
      moves.push_back(m);
      break;
    }
  }

  // Left
  i = 0;
  while (true) {
    ++i;

    if (b[x - i][y].type != 0) {
      if (b[x - i][y].type == 7 || (team == isPos(b[x - i][y].type))) {
        break;
      }
    }

    if (b[x - i][y].type == 0) {
      Move_Util m;
      m.start = start;
      m.end = idxToPos(x - i, y);
      moves.push_back(m);
      continue;
    }

    if ((team != isPos(b[x - i][y].type))) {
      Move_Util m;
      m.start = start;
      m.end = idxToPos(x - i, y);
      moves.push_back(m);
      break;
    }
  }

  // Right
  i = 0;
  while (true) {
    ++i;

    if (b[x + i][y].type != 0) {
      if (b[x + i][y].type == 7 || (team == isPos(b[x + i][y].type))) {
        break;
      }
    }

    if (b[x + i][y].type == 0) {
      Move_Util m;
      m.start = start;
      m.end = idxToPos(x + i, y);
      moves.push_back(m);
      continue;
    }

    if ((team != isPos(b[x + i][y].type))) {
      Move_Util m;
      m.start = start;
      m.end = idxToPos(x + i, y);
      moves.push_back(m);
      break;
    }
  }

  return moves;
}

vector<Move_Util> getBishopMoves(Piece_Util** b, int x, int y, bool team) {
  vector<Move_Util> moves;
  Pos_Util start = idxToPos(x, y);
  Pos_Util end;
  int i = 0;

  if (abs(b[x][y].type) != 3 && abs(b[x][y].type) != 5) {
    return moves;
  }

  // Up Left
  i = 0;
  while (true) {
    ++i;

    if (b[x - i][y - i].type != 0) {
      if (b[x - i][y - i].type == 7 || (team == isPos(b[x - i][y - i].type))) {
        break;
      }
    }

    if (b[x - i][y - i].type == 0) {
      Move_Util m;
      m.start = start;
      m.end = idxToPos(x - i, y - i);
      moves.push_back(m);
      continue;
    }

    if ((team != isPos(b[x - i][y - i].type))) {
      Move_Util m;
      m.start = start;
      m.end = idxToPos(x - i, y - i);
      moves.push_back(m);
      break;
    }
  }

  // Up Right
  i = 0;
  while (true) {
    ++i;

    if (b[x + i][y - i].type != 0) {
      if (b[x + i][y - i].type == 7 || (team == isPos(b[x + i][y - i].type))) {
        break;
      }
    }

    if (b[x + i][y - i].type == 0) {
      Move_Util m;
      m.start = start;
      m.end = idxToPos(x + i, y - i);
      moves.push_back(m);
      continue;
    }

    if ((team != isPos(b[x + i][y - i].type))) {
      Move_Util m;
      m.start = start;
      m.end = idxToPos(x + i, y - i);
      moves.push_back(m);
      break;
    }
  }

  // Down Left
  i = 0;
  while (true) {
    ++i;

    if (b[x - i][y + i].type != 0) {
      if (b[x - i][y + i].type == 7 || (team == isPos(b[x - i][y + i].type))) {
        break;
      }
    }

    if (b[x - i][y + i].type == 0) {
      Move_Util m;
      m.start = start;
      m.end = idxToPos(x - i, y + i);
      moves.push_back(m);
      continue;
    }

    if ((team != isPos(b[x - i][y + i].type))) {
      Move_Util m;
      m.start = start;
      m.end = idxToPos(x - i, y + i);
      moves.push_back(m);
      break;
    }
  }

  // Down Right
  i = 0;
  while (true) {
    ++i;

    if (b[x + i][y + i].type != 0) {
      if (b[x + i][y + i].type == 7 || (team == isPos(b[x + i][y + i].type))) {
        break;
      }
    }

    if (b[x + i][y + i].type == 0) {
      Move_Util m;
      m.start = start;
      m.end = idxToPos(x + i, y + i);
      moves.push_back(m);
      continue;
    }

    if ((team != isPos(b[x + i][y + i].type))) {
      Move_Util m;
      m.start = start;
      m.end = idxToPos(x + i, y + i);
      moves.push_back(m);
      break;
    }
  }

  return moves;
}

vector<Move_Util> getKingMoves(Piece_Util** b, int x, int y, bool team) {
  vector<Move_Util> moves;
  Pos_Util start = idxToPos(x, y);
  Pos_Util end;

  if (abs(b[x][y].type) != 6) {
    return moves;
  }

  // Up
  if (b[x][y - 1].type != 7) {
    if ((team != isPos(b[x][y - 1].type)) || (b[x][y - 1].type == 0)) {
      Move_Util m;
      m.start = start;
      m.end = idxToPos(x, y - 1);
      moves.push_back(m);
    }
  }

  // Down
  if (b[x][y + 1].type != 7) {
    if ((team != isPos(b[x][y + 1].type)) || (b[x][y + 1].type == 0)) {
      Move_Util m;
      m.start = start;
      m.end = idxToPos(x, y + 1);
      moves.push_back(m);
    }
  }

  // Left
  if (b[x - 1][y].type != 7) {
    if ((team != isPos(b[x - 1][y].type)) || (b[x - 1][y].type == 0)) {
      Move_Util m;
      m.start = start;
      m.end = idxToPos(x - 1, y);
      moves.push_back(m);
    }
  }

  // Right
  if (b[x + 1][y].type != 7) {
    if ((team != isPos(b[x + 1][y].type)) || (b[x + 1][y].type == 0)) {
      Move_Util m;
      m.start = start;
      m.end = idxToPos(x + 1, y);
      moves.push_back(m);
    }
  }

  // Up Left
  if (b[x - 1][y - 1].type != 7) {
    if ((team != isPos(b[x - 1][y - 1].type)) || (b[x - 1][y - 1].type == 0)) {
      Move_Util m;
      m.start = start;
      m.end = idxToPos(x - 1, y - 1);
      moves.push_back(m);
    }
  }

  // Up Right
  if (b[x + 1][y - 1].type != 7) {
    if ((team != isPos(b[x + 1][y - 1].type)) || (b[x + 1][y - 1].type == 0)) {
      Move_Util m;
      m.start = start;
      m.end = idxToPos(x + 1, y - 1);
      moves.push_back(m);
    }
  }

  // Down Left
  if (b[x - 1][y + 1].type != 7) {
    if ((team != isPos(b[x - 1][y + 1].type)) || (b[x - 1][y + 1].type == 0)) {
      Move_Util m;
      m.start = start;
      m.end = idxToPos(x - 1, y + 1);
      moves.push_back(m);
    }
  }

  // Down Right
  if (b[x + 1][y + 1].type != 7) {
    if ((team != isPos(b[x + 1][y + 1].type)) || (b[x + 1][y + 1].type == 0)) {
      Move_Util m;
      m.start = start;
      m.end = idxToPos(x + 1, y + 1);
      moves.push_back(m);
    }
  }

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
        Move_Util m;
        m.start = start;
        m.end = idxToPos(x - 1, y);
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
        Move_Util m;
        m.start = start;
        m.end = idxToPos(x + 1, y);
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

void Move_Util::applyMove(Piece_Util** b) {
  int type = b[start.x][start.y].type;
  b[start.x][start.y].type = 0;
  b[end.x][end.y].type = type;
}

bool isPos(int x) { return (x >= 0); }