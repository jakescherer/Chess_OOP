#ifndef __PAWNPIECE_H__
#define __PAWNPIECE_H__

#include "ChessPiece.hh"
#include <cmath>

/**
 * Student implementation of a Pawn chess piece.
 * The class is defined within the Student namespace.
 */
namespace Student
{
    class PawnPiece : public ChessPiece
    {
    public:
        PawnPiece(ChessBoard &board, Color color, int row, int column)
            : ChessPiece(board, color, row, column) {
            _type = Pawn;
        }

        bool canMoveToLocation(int toRow, int toColumn) override
        {
            int direction = (_color == White) ? -1 : 1;
            int rowDiff = toRow - _row;
            int colDiff = abs(toColumn - _column);

            if (colDiff == 0) {
                if (rowDiff == direction) {
                    return _board->getPiece(toRow, toColumn) == nullptr;
                }
                
                int boardSize = _board->getNumRows();
                int whiteStartRow = boardSize - 2;
                int blackStartRow = 1;
                bool atStartingPosition = (_color == White && _row == whiteStartRow) ||
                                         (_color == Black && _row == blackStartRow);
                
                if (atStartingPosition && rowDiff == 2 * direction) {
                    int middleRow = _row + direction;
                    return _board->getPiece(middleRow, _column) == nullptr &&
                           _board->getPiece(toRow, toColumn) == nullptr;
                }
                return false;
            }

            if (colDiff == 1 && rowDiff == direction) {
                ChessPiece *targetPiece = _board->getPiece(toRow, toColumn);
                return targetPiece != nullptr && targetPiece->getColor() != _color;
            }

            return false;
        }

        const char *toString() override
        {
            return _color == White ? "\u2659" : "\u265F";
        }
    };
}

#endif
