////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file ai_util.cpp
/// @author Matt Whitesides CS5400
/// @brief AI Utilities
///   Functions and Structures to assist in move generation and AI logic.
///   0 -- Empty
///   1 -- Pawn
///   2 -- Knight
///   3 -- Bishop
///   4 -- Rook
///   5 -- Queen
///   6 -- King
///   7 -- Invaliid
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "ai_util.h"

Piece_Util** initBoard(Piece_Util** copy) {
  Piece_Util** b = new Piece_Util*[12];
  for (int i = 0; i < 12; ++i) {
    b[i] = new Piece_Util[12];
  }

  // Init a blank board or from a template
  if (copy == NULL) {
    int temp[12][12] = {
        {7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7},
        {7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7},
        {7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 7, 7},
        {7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 7, 7},
        {7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 7, 7},
        {7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 7, 7},
        {7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 7, 7},
        {7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 7, 7},
        {7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 7, 7},
        {7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 7, 7},
        {7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7},
        {7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7},
    };

    for (int y = 0; y < 12; ++y) {
      for (int x = 0; x < 12; ++x) {
        b[x][y].type = temp[x][y];
      }
    }
  } else {
    for (int y = 0; y < 12; ++y) {
      for (int x = 0; x < 12; ++x) {
        b[x][y].type = copy[x][y].type;
        b[x][y].hasMoved = copy[x][y].hasMoved;
      }
    }
  }

  return b;
}

Piece_Util** initBoard(Piece_Util** copy, Move_Util m) {
  Piece_Util** b = initBoard(copy);
  applyMove(b, m);
  return b;
}

void clean(Piece_Util** b) {
  for (int i = 0; i < 12; ++i) {
    delete[] b[i];
  }
  delete[] b;
}

Pos_Util idxToPos(int x, int y) {
  Pos_Util pos;
  string file;
  int rank = (y - 1);

  pos.x = x;
  pos.y = y;

  switch (rank) {
    case 1:
      rank = 8;
      break;
    case 2:
      rank = 7;
      break;
    case 3:
      rank = 6;
      break;
    case 4:
      rank = 5;
      break;
    case 5:
      rank = 4;
      break;
    case 6:
      rank = 3;
      break;
    case 7:
      rank = 2;
      break;
    case 8:
      rank = 1;
      break;
    default:
      rank = 1;
      break;
  }

  --x;
  switch (x) {
    case 1:
      file = "a";
      break;
    case 2:
      file = "b";
      break;
    case 3:
      file = "c";
      break;
    case 4:
      file = "d";
      break;
    case 5:
      file = "e";
      break;
    case 6:
      file = "f";
      break;
    case 7:
      file = "g";
      break;
    case 8:
      file = "h";
      break;
    default:
      file = "a";
      break;
  }

  pos.rank = rank;
  pos.file = file;

  return pos;
}

bool isPos(int x) { return (x >= 0); }

bool inCheck(Piece_Util** b, Move_Util m, bool team) {
  bool ret = false;
  Piece_Util** temp = initBoard(b);
  vector<Move_Util> moves;
  Pos_Util king;

  applyMove(temp, m);
  moves = getPlayerMoves(temp, !team);
  king = getKing(temp, team);

  for (Move_Util m : moves) {
    if (m.end.rank == king.rank) {
      if (m.end.file.compare(king.file) == 0) {
        ret = true;
        break;
      }
    }
  }

  clean(temp);
  return ret;
}

bool inCheck(Piece_Util** b, bool team) {
  bool ret = false;
  Piece_Util** temp = initBoard(b);
  vector<Move_Util> moves;
  Pos_Util king;

  moves = getPlayerMoves(temp, !team);
  king = getKing(temp, team);

  for (Move_Util m : moves) {
    if (m.end.rank == king.rank) {
      if (m.end.file.compare(king.file) == 0) {
        ret = true;
        break;
      }
    }
  }

  clean(temp);
  return ret;
}

void applyMove(Piece_Util** b, Move_Util m) {
  int type = b[m.start.x][m.start.y].type;
  b[m.start.x][m.start.y].type = 0;
  b[m.end.x][m.end.y].type = type;
}

Pos_Util getKing(Piece_Util** b, bool team) {
  Pos_Util ret;
  int kingNum = (team) ? 6 : -6;

  for (int y = 2; y < 10; ++y) {
    for (int x = 2; x < 10; ++x) {
      if (b[x][y].type == kingNum) {
        return idxToPos(x, y);
      }
    }
  }

  return ret;
}

void printBoard(Piece_Util** b) {
  for (int y = 2; y < 10; ++y) {
    for (int x = 2; x < 10; ++x) {
      if (b[x][y].type >= 0) cout << ' ';
      cout << b[x][y].type << ' ';
    }
    cout << endl;
  }
}

void loadPiece(Piece_Util** b, int rank, string file, string type, bool team,
               bool hasMoved) {
  int r = 0;
  int f = 0;
  int t = (team) ? 1 : -1;

  switch (rank) {
    case 1:
      r = 8;
      break;
    case 2:
      r = 7;
      break;
    case 3:
      r = 6;
      break;
    case 4:
      r = 5;
      break;
    case 5:
      r = 4;
      break;
    case 6:
      r = 3;
      break;
    case 7:
      r = 2;
      break;
    case 8:
      r = 1;
      break;
  }

  ++r;

  if (file.compare("a") == 0) {
    f = 2;
  } else if (file.compare("b") == 0) {
    f = 3;
  } else if (file.compare("c") == 0) {
    f = 4;
  } else if (file.compare("d") == 0) {
    f = 5;
  } else if (file.compare("e") == 0) {
    f = 6;
  } else if (file.compare("f") == 0) {
    f = 7;
  } else if (file.compare("g") == 0) {
    f = 8;
  } else if (file.compare("h") == 0) {
    f = 9;
  }

  if (type.compare("Pawn") == 0) {
    b[f][r].type = (1 * t);
  } else if (type.compare("Knight") == 0) {
    b[f][r].type = (2 * t);
  } else if (type.compare("Bishop") == 0) {
    b[f][r].type = (3 * t);
  } else if (type.compare("Rook") == 0) {
    b[f][r].type = (4 * t);
  } else if (type.compare("Queen") == 0) {
    b[f][r].type = (5 * t);
  } else if (type.compare("King") == 0) {
    b[f][r].type = (6 * t);
  }

  b[f][r].hasMoved = hasMoved;
}

ostream& operator<<(ostream& os, const Pos_Util& p) {
  os << p.file << p.rank;
  return os;
}

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
          moves.insert(moves.end(), pieceMoves.begin(), pieceMoves.end());
          break;
        case 2:
          pieceMoves = getKnightMoves(b, x, y, team);
          moves.insert(moves.end(), pieceMoves.begin(), pieceMoves.end());
          break;
        case 3:
          pieceMoves = getBishopMoves(b, x, y, team);
          moves.insert(moves.end(), pieceMoves.begin(), pieceMoves.end());
          break;
        case 4:
          pieceMoves = getRookMoves(b, x, y, team);
          moves.insert(moves.end(), pieceMoves.begin(), pieceMoves.end());
          break;
        case 5:
          pieceMoves = getRookMoves(b, x, y, team);
          moves.insert(moves.end(), pieceMoves.begin(), pieceMoves.end());
          pieceMoves = getBishopMoves(b, x, y, team);
          moves.insert(moves.end(), pieceMoves.begin(), pieceMoves.end());
          break;
        case 6:
          pieceMoves = getKingMoves(b, x, y, team);
          moves.insert(moves.end(), pieceMoves.begin(), pieceMoves.end());
          break;
      }
    }
  }

  // for (Move_Util m : moves) {
  //   cout << m << endl;
  // }

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

int heuristic(Piece_Util** b, bool team) {
  // 39 Being all the other teams pieces are gone.
  // When the opponent has all their pieces the h will equal 0;
  int h = 39;

  // Reward putting the other team in check
  if (inCheck(b, !team)) {
    h += 5;
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

int alphaBetaDLMM(Piece_Util** board, int d, int a, int b, bool isMax, bool team) {
  if (d <= 0) {
    return heuristic(board, team);
  }

  int v = 0;

  if (isMax) {
    v = INT_MIN;

    for (Move_Util m : getPlayerMoves(board, team)) {
      Piece_Util** tempBoard = initBoard(board, m);

      v = max(v, alphaBetaDLMM(tempBoard, --d, a, b, false, team));
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

      v = min(v, alphaBetaDLMM(tempBoard, --d, a, b, true, team));
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
