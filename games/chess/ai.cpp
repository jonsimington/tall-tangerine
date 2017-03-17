
// AI
// This is where you build your AI
#include "ai.hpp"

// You can add #includes here for your AI.

namespace cpp_client {
namespace chess {

/// <summary>
/// This returns your AI's name to the game server.
/// Replace the string name.
/// </summary>
/// <returns>The name of your AI.</returns>
std::string AI::get_name() const {
  // REPLACE WITH YOUR TEAM NAME!
  return "Matthew_Whitesides";
}

/// <summary>
/// This is automatically called when the game first starts, once the game
/// objects are created
/// </summary>
void AI::start() {
  // This is a good place to initialize any variables
  srand(time(NULL));
}

/// <summary>
/// This is automatically called the game (or anything in it) updates
/// </summary>
void AI::game_updated() {
  // If a function you call triggers an update this will be called before it
  // returns.
}

/// <summary>
/// This is automatically called when the game ends.
/// </summary>
/// <param name="won">true if you won, false otherwise</param>
/// <param name="reason">An explanation for why you either won or lost</param>
void AI::ended(bool won, const std::string& reason) {
  // You can do any cleanup of your AI here.  The program ends when this
  // function returns.
}

/// <summary>
/// This is called every time it is this AI.player's turn.
/// </summary>
/// <returns>Represents if you want to end your turn. True means end your turn,
/// False means to keep your turn going and re-call this function.</returns>
bool AI::run_turn() {
  // Here is where you'll want to code your AI.
  // 1) print the board to the console
  print_current_board();

  // 2) print the opponent's last move to the console
  if (game->moves.size() > 0) {
    std::cout << "Opponent's Last Move: '"
              << game->moves[game->moves.size() - 1]->san << "'" << std::endl;
  }

  // 3) print how much time remaining this AI has to calculate moves
  std::cout << "Time Remaining: " << player->time_remaining << " ns"
            << std::endl;

  // 4) move piece
  Piece_Util** b = initBoard();
  bool team = (player->color.compare("White") == 0);
  vector<Move_Util> moves;
  Move_Util finalMove;
  Piece* p = NULL;
  loadBoard(b);
  // printBoard(b);
  moves = getPlayerMoves(b, team);

  if (moves.size() == 0) {
    return true;
  }

  for (Move_Util m : moves) {
    m.h = depthLimitedMiniMax(b, 100, true, team);
  }

  sort(moves.begin(), moves.end());

  int maxHur = 0;
  int maxHurCount = 0;
  int i = 0;
  if (moves.size() > 0) {
    maxHurCount = moves[0].h;
  }

  while ((maxHurCount < moves.size()) && (moves[maxHurCount].h == maxHur)) {
    ++maxHurCount;        
  }

  cout << "Max Count: " << maxHurCount << endl;

  int randNum = rand() % maxHurCount;
  iter_swap(moves.begin(), moves.begin() + randNum);

  for (Move_Util m : moves) {
    if (!inCheck(b, m, team)) {
      finalMove = m;
      break;
    }
  }

  // for (int i = 0; i < 3; ++i) {
  //   cout << "Huristics: " << moves[i].h << endl;
  // }

  for (Piece p : player->pieces) {
    if (p->file.compare(finalMove.start.file) == 0 &&
        p->rank == finalMove.start.rank) {
      if (p->type.compare("Pawn") == 0 &&
          (finalMove.end.rank == 8 || finalMove.end.rank == 1)) {
        p->move(finalMove.end.file, finalMove.end.rank, "Queen");
      } else {
        p->move(finalMove.end.file, finalMove.end.rank);
      }
      // printMovesForPiece(b, finalMove.start.x, finalMove.start.y,
      //                    team);

      cout << "Chosen Move:\n";
      cout << finalMove << endl << endl;
    }
  }

  clean(b);
  return true;  // to signify we are done with our turn.
}


Piece* AI::getPieceAtLoc(std::string file, int rank) {
  for (Piece p : player->pieces) {
    if (p->file.compare(file) == 0 && p->rank == rank) {
      return &p;
    }
  }

  return NULL;
}

void AI::loadBoard(Piece_Util** b) {
  bool team = true;
  bool hasMoved = false;
  for (Piece p : game->pieces) {
    team = (p->owner->color.compare("White") == 0);
    hasMoved = p->has_moved;
    if (p->type.compare("Pawn") == 0) {
      hasMoved = checkForEnPassent(p);
    }
    loadPiece(b, p->rank, p->file, p->type, team, hasMoved);
  }
}

bool AI::checkForEnPassent(Piece p) {
  if (game->moves.size() > 0) {
    if ((game->moves.back()->piece->rank == p->rank) &&
        (game->moves.back()->piece->file == p->file)) {
      if (game->moves.back()->from_rank == 2 &&
          game->moves.back()->to_rank == 4) {
        return true;
      } else if (game->moves.back()->from_rank == 7 &&
                 game->moves.back()->to_rank == 5) {
        return true;
      }
    }
  }

  return false;
}

void AI::printMovesForPiece(Piece_Util** b, int x, int y, bool team) {
  Piece_Util** temp = initBoard();
  vector<Move_Util> tempMoves;
  temp[x][y] = b[x][y];
  tempMoves = getPlayerMoves(temp, team);

  cout << endl;
  switch (b[x][y].type) {
    case 1:
      cout << "Available moves for Pawn:\n";
      break;
    case 2:
      cout << "Available moves for Knight:\n";
      break;
    case 3:
      cout << "Available moves for Bishop:\n";
      break;
    case 4:
      cout << "Available moves for Rook:\n";
      break;
    case 5:
      cout << "Available moves for Queen:\n";
      break;
    case 6:
      cout << "Available moves for King:\n";
      break;
  }

  for (Move_Util m : tempMoves) {
    cout << m << endl;
  }

  cout << endl;
  clean(temp);
}

/// <summary>
///  Prints the current board using pretty ASCII art
/// </summary>
/// <remarks>
/// Note: you can delete this function if you wish
/// </remarks>
void AI::print_current_board() {
  for (int rank = 9; rank >= -1; rank--) {
    std::string str = "";
    if (rank == 9 || rank == 0)  // then the top or bottom of the board
    {
      str = "   +------------------------+";
    } else if (rank == -1)  // then show the ranks
    {
      str = "     a  b  c  d  e  f  g  h";
    } else  // board
    {
      str += " ";
      str += std::to_string(rank);
      str += " |";
      // fill in all the files with pieces at the current rank
      for (int file_offset = 0; file_offset < 8; file_offset++) {
        std::string file(1, 'a' + file_offset);  // start at a, with with file
                                                 // offset increasing the char;
        chess::Piece current_piece = nullptr;
        for (const auto& piece : game->pieces) {
          if (piece->file == file &&
              piece->rank == rank)  // then we found the piece at (file, rank)
          {
            current_piece = piece;
            break;
          }
        }

        char code = '.';  // default "no piece";
        if (current_piece != nullptr) {
          code = current_piece->type[0];

          if (current_piece->type ==
              "Knight")  // 'K' is for "King", we use 'N' for "Knights"
          {
            code = 'N';
          }

          if (current_piece->owner->id == "1")  // the second player (black) is
                                                // lower case. Otherwise it's
                                                // upppercase already
          {
            code = tolower(code);
          }
        }

        str += " ";
        str += code;
        str += " ";
      }

      str += "|";
    }

    std::cout << str << std::endl;
  }
}

// You can add additional methods here for your AI to call

}  // chess

}  // cpp_client
