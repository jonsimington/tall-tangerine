#include "util.h"

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
  m.applyMove(b);
  return b;
}

void clean(Piece_Util** b) {
  for (int i = 0; i < 12; ++i) {
    delete[] b[i];
  }
  delete[] b;
}

bool inCheck(Piece_Util** b, Move_Util m, bool team) {
  Piece_Util** temp = initBoard(b, m);
  return inCheck(temp, team, true);
}

bool inCheck(Piece_Util** b, bool team, bool useBoard) {
  bool ret = false;
  vector<Move_Util> moves;
  Pos_Util king;
  Piece_Util** temp = NULL;

  if (useBoard) {
    temp = b;
  }
  else {
    temp = initBoard(b);
  }

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

void printBoard(Piece_Util** b) {
  for (int y = 2; y < 10; ++y) {
    for (int x = 2; x < 10; ++x) {
      if (b[x][y].type >= 0) cout << ' ';
      cout << b[x][y].type << ' ';
    }
    cout << endl;
  }
}