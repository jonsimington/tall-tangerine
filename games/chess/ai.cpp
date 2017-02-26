// AI
// This is where you build your AI
#include "ai.hpp"
#include "ai_util.h"

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
    /// This is automatically called when the game first starts, once the game objects are created
    /// </summary>
    void AI::start() {
      // This is a good place to initialize any variables
      srand(time(NULL));
    }

    /// <summary>
    /// This is automatically called the game (or anything in it) updates
    /// </summary>
    void AI::game_updated() {
      // If a function you call triggers an update this will be called before it returns.
    }

    /// <summary>
    /// This is automatically called when the game ends.
    /// </summary>
    /// <param name="won">true if you won, false otherwise</param>
    /// <param name="reason">An explanation for why you either won or lost</param>
    void AI::ended(bool won, const std::string& reason) {
      // You can do any cleanup of your AI here.  The program ends when this function returns.
    }

    /// <summary>
    /// This is called every time it is this AI.player's turn.
    /// </summary>
    /// <returns>Represents if you want to end your turn. True means end your turn, False means to keep your turn going and re-call this function.</returns>
    bool AI::run_turn() {
      // Here is where you'll want to code your AI.
      // 1) print the board to the console
      print_current_board();

      // 2) print the opponent's last move to the console
      if(game->moves.size() > 0) {
        std::cout << "Opponent's Last Move: '" << game->moves[game->moves.size() - 1]->san << "'" << std::endl;
      }

      // 3) print how much time remaining this AI has to calculate moves
      std::cout << "Time Remaining: " << player->time_remaining << " ns" << std::endl;

      // 4) move piece
      //Check for King/Queen Side Castling and en passent.
      //You have to do a move to get out of check if in check.
      int** b = initBoard();
      bool team = (player->color.compare("White") == 0);

      vector<Move_Util> moves;
      Piece* p = NULL;
      loadBoard(b);
      printBoard(b);
      moves = getPlayerMoves(b, team);
      int randIdx = rand() % moves.size();

      // for (auto m : moves) {
      //   cout << "Moves: " << m << endl;
      // }

      while (true) {
        if (inCheck(b, moves[randIdx], team)) {
          moves.erase(moves.begin() + randIdx);
          if (moves.size() == 0) {
            randIdx = 0;
            break;
          }
          randIdx = rand() % moves.size();
        }
        else {
          break;
        }
      }

      for (Piece p : player->pieces) {
        if (p->file.compare(moves[randIdx].start.file) == 0 && p->rank == moves[randIdx].start.rank) {
          p->move(moves[randIdx].end.file, moves[randIdx].end.rank);
        }
      }

      clean(b);
      return true; // to signify we are done with our turn.
    }

    Piece* AI::getPieceAtLoc(std::string file, int rank) {
      for (Piece p : player->pieces) {
        if (p->file.compare(file) == 0 && p->rank == rank) {
          return &p;
        }
      }

      return NULL;
    }

    void AI::loadBoard(int** b) {
      bool team = true;
      for (Piece p : game->pieces) {
        team = (p->owner->color.compare("White") == 0);
        loadPiece(b, p->rank, p->file, p->type, team);
      }
    }

    /// <summary>
    ///  Prints the current board using pretty ASCII art
    /// </summary>
    /// <remarks>
    /// Note: you can delete this function if you wish
    /// </remarks>
    void AI::print_current_board() {
      for(int rank = 9; rank >= -1; rank--)
      {
        std::string str = "";
        if(rank == 9 || rank == 0) // then the top or bottom of the board
        {
          str = "   +------------------------+";
        }
        else if(rank == -1) // then show the ranks
        {
          str = "     a  b  c  d  e  f  g  h";
        }
        else // board
        {
          str += " ";
          str += std::to_string(rank);
          str += " |";
          // fill in all the files with pieces at the current rank
          for(int file_offset = 0; file_offset < 8; file_offset++)
          {
            std::string file(1, 'a' + file_offset); // start at a, with with file offset increasing the char;
            chess::Piece current_piece = nullptr;
            for(const auto& piece : game->pieces)
            {
              if(piece->file == file && piece->rank == rank) // then we found the piece at (file, rank)
              {
                current_piece = piece;
                break;
              }
            }

            char code = '.'; // default "no piece";
            if(current_piece != nullptr)
            {
              code = current_piece->type[0];

              if(current_piece->type == "Knight") // 'K' is for "King", we use 'N' for "Knights"
              {
                code = 'N';
              }

              if(current_piece->owner->id == "1") // the second player (black) is lower case. Otherwise it's upppercase already
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

  } // chess

} // cpp_client
