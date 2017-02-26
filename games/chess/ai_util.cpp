#include "ai_util.h"
//0 -- Empty
//1 -- Pawn
//2 -- Knight
//3 -- Bishop
//4 -- Rook
//5 -- Queen
//6 -- King
//7 -- Invaliid

int** initBoard(int** copy) {
  int** b = new int*[12];
  for(int i = 0; i < 12; ++i) {
    b[i] = new int[12];
  }

  //Init a blank board or from a template
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
        b[x][y] = temp[x][y];
      }
    }
  }
  else {
    for (int y = 0; y < 12; ++y) {
      for (int x = 0; x < 12; ++x) {
        b[x][y] = copy[x][y];
      }
    }
  }

  return b;
}

void clean(int** b) {
  for(int i = 0; i < 12; ++i) {
    delete [] b[i];
  }
  delete [] b;
}

Pos idxToPos(int x, int y) {
  Pos pos;
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

bool isPos(int x) {
  return (x >= 0);
}

bool inCheck(int** b, Move_Util m, bool team) {
  bool ret = false;
  int** temp = initBoard(b);
  vector<Move_Util> moves;
  Pos king;

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

void applyMove(int** b, Move_Util m) {
  int type = b[m.start.x][m.start.y];
  b[m.start.x][m.start.y] = 0;
  b[m.end.x][m.end.y] = type;
}

Pos getKing(int** b, bool team) {
  Pos ret;
  int kingNum = (team) ? 6 : -6;

  for (int y = 2; y < 10; ++y) {
    for (int x = 2; x < 10; ++x) {
      if (b[x][y] == kingNum) {
        return idxToPos(x, y);
      }
    }
  }

  return ret;
}

void printBoard(int** b) {
  for (int y = 0; y < 12; ++y){
    for (int x = 0; x < 12; ++x) {
      if (b[x][y] >= 0) cout << ' ';
      cout << b[x][y] << ' ';
    }
    cout << endl;
  }
}

void loadPiece(int** b, int rank, string file, string type, bool team) {
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
  }
  else if (file.compare("b") == 0) {
    f = 3;
  }
  else if (file.compare("c") == 0) {
    f = 4;
  }
  else if (file.compare("d") == 0) {
    f = 5;
  }
  else if (file.compare("e") == 0) {
    f = 6;
  }
  else if (file.compare("f") == 0) {
    f = 7;
  }
  else if (file.compare("g") == 0) {
    f = 8;
  }
  else if (file.compare("h") == 0) {
    f = 9;
  }

  if (type.compare("Pawn") == 0) {
    b[f][r] = (1 * t);
  }
  else if (type.compare("Knight") == 0) {
    b[f][r] = (2 * t);
  }
  else if (type.compare("Bishop") == 0) {
    b[f][r] = (3 * t);
  }
  else if (type.compare("Rook") == 0) {
    b[f][r] = (4 * t);
  }
  else if (type.compare("Queen") == 0) {
    b[f][r] = (5 * t);
  }
  else if (type.compare("King") == 0) {
    b[f][r] = (6 * t);
  }
}

ostream& operator<<(ostream& os, const Pos& p) {
  os << p.file << p.rank;
  return os;
}

ostream& operator<<(ostream& os, const Move_Util& m) {
  os << m.start << " -> " << m.end;
  return os;
}

vector<Move_Util> getPlayerMoves(int** b, bool team) {

  vector<Move_Util> moves;

  for (int y = 2; y < 10; ++y) {
    for (int x = 2; x < 10; ++x) {
      //Empty or Invalid Square Check
      if (b[x][y] == 0 || b[x][y] == 7) {
        continue;
      }

      //Other Players Character Check
      if (team) {
        if (b[x][y] < 0) {
          continue;
        }
      }
      else {
        if (b[x][y] > 0) {
          continue;
        }
      }

      vector<Move_Util> pieceMoves;
      switch (abs(b[x][y])) {
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

vector<Move_Util> getPawnMoves(int** b, int x, int y, bool team) {
  int dir = (team) ? -1 : 1;
  vector<Move_Util> moves;
  Pos start = idxToPos(x, y);
  Pos end;

  if (abs(b[x][y]) != 1) {
    return moves;
  }

  //Move one space
  if (b[x][y + dir] == 0) {
    Move_Util m;
    m.start = start;
    m.end = idxToPos(x, y + dir);
    moves.push_back(m);
  }

  if (team) {
    //Move two spaces from the start
    if (y == 8) {
      if ((b[x][y + (dir * 2)] == 0) && (b[x][y + dir] == 0)) {
        Move_Util m;
        m.start = start;
        m.end = idxToPos(x, y + (dir * 2));
        moves.push_back(m);
      }
    }

    //Capture Piece
    if (b[x + 1][y + dir] < 0) {
      Move_Util m;
      m.start = start;
      m.end = idxToPos(x + 1, y + dir);
      moves.push_back(m);
    }
    if (b[x - 1][y + dir] < 0) {
      Move_Util m;
      m.start = start;
      m.end = idxToPos(x - 1, y + dir);
      moves.push_back(m);
    }
  }
  else {
    //Move two spaces from the start
    if (y == 3) {
      if ((b[x][y + (dir * 2)] == 0) && (b[x][y + dir] == 0)) {
        Move_Util m;
        m.start = start;
        m.end = idxToPos(x, y + (dir * 2));
        moves.push_back(m);
      }
    }

    //Capture Piece
    if (b[x + 1][y + dir] > 0 && b[x + 1][y + dir] != 7) {
      Move_Util m;
      m.start = start;
      m.end = idxToPos(x + 1, y + dir);
      moves.push_back(m);
    }
    if (b[x - 1][y + dir] > 0 && b[x - 1][y + dir] != 7) {
      Move_Util m;
      m.start = start;
      m.end = idxToPos(x - 1, y + dir);
      moves.push_back(m);
    }
  }

  return moves;
}

vector<Move_Util> getKnightMoves(int** b, int x, int y, bool team) {
  vector<Move_Util> moves;
  Pos start = idxToPos(x, y);
  Pos end;

  if (abs(b[x][y]) != 2) {
    return moves;
  }

  if (b[x - 2][y + 1] != 7) {
    if (b[x - 2][y + 1] == 0 || (team != isPos(b[x - 2][y + 1]))) {
      Move_Util m;
      m.start = start;
      m.end = idxToPos(x - 2, y + 1);
      moves.push_back(m);
    }
  }

  if (b[x - 1][y + 2] != 7) {
    if (b[x - 1][y + 2] == 0 || (team != isPos(b[x - 1][y + 2]))) {
      Move_Util m;
      m.start = start;
      m.end = idxToPos(x - 1, y + 2);
      moves.push_back(m);
    }
  }

  if (b[x + 2][y + 1] != 7) {
    if (b[x + 2][y + 1] == 0 || (team != isPos(b[x + 2][y + 1]))) {
      Move_Util m;
      m.start = start;
      m.end = idxToPos(x + 2, y + 1);
      moves.push_back(m);
    }
  }

  if (b[x + 1][y + 2] != 7) {
    if (b[x + 1][y + 2] == 0 || (team != isPos(b[x + 1][y + 2]))) {
      Move_Util m;
      m.start = start;
      m.end = idxToPos(x + 1, y + 2);
      moves.push_back(m);
    }
  }

  if (b[x + 2][y - 1] != 7) {
    if (b[x + 2][y - 1] == 0 || (team != isPos(b[x + 2][y - 1]))) {
      Move_Util m;
      m.start = start;
      m.end = idxToPos(x + 2, y - 1);
      moves.push_back(m);
    }
  }

  if (b[x + 1][y - 2] != 7) {
    if (b[x + 1][y - 2] == 0 || (team != isPos(b[x + 1][y - 2]))) {
      Move_Util m;
      m.start = start;
      m.end = idxToPos(x + 1, y - 2);
      moves.push_back(m);
    }
  }

  if (b[x - 2][y - 1] != 7) {
    if (b[x - 2][y - 1] == 0 || (team != isPos(b[x - 2][y - 1]))) {
      Move_Util m;
      m.start = start;
      m.end = idxToPos(x - 2, y - 1);
      moves.push_back(m);
    }
  }

  if (b[x - 1][y - 2] != 7) {
    if (b[x - 1][y - 2] == 0 || (team != isPos(b[x - 1][y - 2]))) {
      Move_Util m;
      m.start = start;
      m.end = idxToPos(x - 1, y - 2);
      moves.push_back(m);
    }
  }

  return moves;
}

vector<Move_Util> getRookMoves(int** b, int x, int y, bool team) {
  vector<Move_Util> moves;
  Pos start = idxToPos(x, y);
  Pos end;
  int i = 0;

  if (abs(b[x][y]) != 4 && abs(b[x][y]) != 5) {
    return moves;
  }

  //Up
  while(true) {
    ++i;

    if (b[x][y - i] != 0) {
      if (b[x][y - i] == 7 || (team == isPos(b[x][y - i]))) {
        break;
      }
    }

    if (b[x][y - i] == 0) {
      Move_Util m;
      m.start = start;
      m.end = idxToPos(x, y - i);
      moves.push_back(m);
      continue;
    }

    if ((team != isPos(b[x][y - i]))) {
      Move_Util m;
      m.start = start;
      m.end = idxToPos(x, y - i);
      moves.push_back(m);
      break;
    }
  }

  //Down
  i = 0;
  while(true) {
    ++i;

    if (b[x][y + i] != 0) {
      if (b[x][y + i] == 7 || (team == isPos(b[x][y + i]))) {
        break;
      }
    }

    if (b[x][y + i] == 0) {
      Move_Util m;
      m.start = start;
      m.end = idxToPos(x, y + i);
      moves.push_back(m);
      continue;
    }

    if ((team != isPos(b[x][y + i]))) {
      Move_Util m;
      m.start = start;
      m.end = idxToPos(x, y + i);
      moves.push_back(m);
      break;
    }
  }

  //Left
  i = 0;
  while(true) {
    ++i;

    if (b[x - i][y] != 0) {
      if (b[x - i][y] == 7 || (team == isPos(b[x - i][y]))) {
        break;
      }
    }

    if (b[x - i][y] == 0) {
      Move_Util m;
      m.start = start;
      m.end = idxToPos(x - i, y);
      moves.push_back(m);
      continue;
    }

    if ((team != isPos(b[x - i][y]))) {
      Move_Util m;
      m.start = start;
      m.end = idxToPos(x - i, y);
      moves.push_back(m);
      break;
    }
  }

  //Right
  i = 0;
  while(true) {
    ++i;

    if (b[x + i][y] != 0) {
      if (b[x + i][y] == 7 || (team == isPos(b[x + i][y]))) {
        break;
      }
    }

    if (b[x + i][y] == 0) {
      Move_Util m;
      m.start = start;
      m.end = idxToPos(x + i, y);
      moves.push_back(m);
      continue;      
    }

    if ((team != isPos(b[x + i][y]))) {
      Move_Util m;
      m.start = start;
      m.end = idxToPos(x + i, y);
      moves.push_back(m);
      break;
    }
  }

  return moves;
}

vector<Move_Util> getBishopMoves(int** b, int x, int y, bool team) {
  vector<Move_Util> moves;
  Pos start = idxToPos(x, y);
  Pos end;
  int i = 0;

  if (team) {
    cout << "White!" << endl;
  }
  else {
    cout << "Black!" << endl;
  }

  if (abs(b[x][y]) != 3 && abs(b[x][y]) != 5) {
    return moves;
  }

  //Up Left
  i = 0;
  while(true) {
    ++i;

    if (b[x - i][y - i] != 0) {
      if (b[x - i][y - i] == 7 || (team == isPos(b[x - i][y - i]))) {
        break;
      }
    }

    if (b[x - i][y - i] == 0) {
      Move_Util m;
      m.start = start;
      m.end = idxToPos(x - i, y - i);
      moves.push_back(m);
      cout << "BishopUpLeftA: " << m << endl;
      continue;
    }

    if ((team != isPos(b[x - i][y - i]))) {
      Move_Util m;
      m.start = start;
      m.end = idxToPos(x - i, y - i);
      moves.push_back(m);
      cout << "BishopUpLeftB: " << m << endl;
      break;
    }
  }

  //Up Right
  i = 0;
  while(true) {
    ++i;

    if (b[x + i][y - i] != 0) {
      if (b[x + i][y - i] == 7 || (team == isPos(b[x + i][y - i]))) {
        break;
      }
    }

    if (b[x + i][y - i] == 0) {
      Move_Util m;
      m.start = start;
      m.end = idxToPos(x + i, y - i);
      moves.push_back(m);
      cout << "BishopUpRightA: " << m << endl;
      continue;
    }

    if ((team != isPos(b[x + i][y - i]))) {
      Move_Util m;
      m.start = start;
      m.end = idxToPos(x + i, y - i);
      moves.push_back(m);
      cout << "BishopUpRightB: " << m << endl;
      break;
    }
  }

  //Down Left
  i = 0;
  while(true) {
    ++i;

    if (b[x - i][y + i] != 0) {
      if (b[x - i][y + i] == 7 || (team == isPos(b[x - i][y + i]))) {
        break;
      }
    }

    if (b[x - i][y + i] == 0) {
      Move_Util m;
      m.start = start;
      m.end = idxToPos(x - i, y + i);
      moves.push_back(m);
      cout << "BishopDownLeftA: " << m << endl;
      continue;
    }

    if ((team != isPos(b[x - i][y + i]))) {
      Move_Util m;
      m.start = start;
      m.end = idxToPos(x - i, y + i);
      moves.push_back(m);
      cout << "BishopDownLeftB: " << m << endl;
      break;
    }
  }

  //Down Right
  i = 0;
  while(true) {
    ++i;

    if (b[x + i][y + i] != 0) {
      if (b[x + i][y + i] == 7 || (team == isPos(b[x + i][y + i]))) {
        break;
      }
    }

    if (b[x + i][y + i] == 0) {
      Move_Util m;
      m.start = start;
      m.end = idxToPos(x + i, y + i);
      moves.push_back(m);
      cout << "BishopDownRightA: " << m << endl;
      continue;
    }

    if ((team != isPos(b[x + i][y + i]))) {
      Move_Util m;
      m.start = start;
      m.end = idxToPos(x + i, y + i);
      moves.push_back(m);
      cout << "BishopDownRightB: " << m << endl;
      break;
    }
  }

  return moves;
}

vector<Move_Util> getKingMoves(int** b, int x, int y, bool team) {
  vector<Move_Util> moves;
  Pos start = idxToPos(x, y);
  Pos end;

  if (abs(b[x][y]) != 6) {
    return moves;
  }

  //Up
  if (b[x][y - 1] != 7) {
    if ((team != isPos(b[x][y - 1])) || (b[x][y - 1] == 0)) {
      Move_Util m;
      m.start = start;
      m.end = idxToPos(x, y - 1);
      moves.push_back(m);
    }
  }

  //Down
  if (b[x][y + 1] != 7) {
    if ((team != isPos(b[x][y + 1])) || (b[x][y + 1] == 0)) {
      Move_Util m;
      m.start = start;
      m.end = idxToPos(x, y + 1);
      moves.push_back(m);
    }
  }

  //Left
  if (b[x - 1][y] != 7) {
    if ((team != isPos(b[x - 1][y])) || (b[x - 1][y] == 0)) {
      Move_Util m;
      m.start = start;
      m.end = idxToPos(x - 1, y);
      moves.push_back(m);
    }
  }

  //Right
  if (b[x + 1][y] != 7) {
    if ((team != isPos(b[x + 1][y])) || (b[x + 1][y] == 0)) {
      Move_Util m;
      m.start = start;
      m.end = idxToPos(x + 1, y);
      moves.push_back(m);
    }
  }

  //Up Left
  if (b[x - 1][y - 1] != 7) {
    if ((team != isPos(b[x - 1][y - 1])) || (b[x - 1][y - 1] == 0)) {
      Move_Util m;
      m.start = start;
      m.end = idxToPos(x - 1, y - 1);
      moves.push_back(m);
    }
  }

  //Up Right
  if (b[x + 1][y - 1] != 7) {
    if ((team != isPos(b[x + 1][y - 1])) || (b[x + 1][y - 1] == 0)) {
      Move_Util m;
      m.start = start;
      m.end = idxToPos(x + 1, y - 1);
      moves.push_back(m);
    }
  }

  //Down Left
  if (b[x - 1][y + 1] != 7) {
    if ((team != isPos(b[x - 1][y + 1])) || (b[x - 1][y + 1] == 0)) {
      Move_Util m;
      m.start = start;
      m.end = idxToPos(x - 1, y + 1);
      moves.push_back(m);
    }
  }

  //Down Right
  if (b[x + 1][y + 1] != 7) {
    if ((team != isPos(b[x + 1][y + 1])) || (b[x + 1][y + 1] == 0)) {
      Move_Util m;
      m.start = start;
      m.end = idxToPos(x + 1, y + 1);
      moves.push_back(m);
    }
  }

  return moves;
}
