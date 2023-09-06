////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file pos_util.cpp
/// @author Matt Whitesides CS5400
/// @brief Position Utilities
///   Functions and Structures to assist in position translation.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "util.h"

ostream& operator<<(ostream& os, const Pos_Util& p) {
  os << p.file << p.rank;
  return os;
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