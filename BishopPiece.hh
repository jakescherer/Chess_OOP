#ifndef __BISHOPPIECE_H__
#define __BISHOPPIECE_H__

#include "ChessPiece.hh"
#include <cmath>

/**
 * Student implementation of a Bishop chess piece.
 * The class is defined within the Student namespace.
 */
namespace Student
{
    class BishopPiece : public ChessPiece
    {
    public:
        BishopPiece(ChessBoard &board, Color color, int row, int column)
            : ChessPiece(board, color, row, column) {
            _type = Bishop;
        }

        bool canMoveToLocation(int toRow, int toColumn) override
        {
            int rowDiff = abs(toRow - _row);
            int colDiff = abs(toColumn - _column);
            return rowDiff == colDiff && rowDiff > 0;
        }

        const char *toString() override
        {
            return _color == White ? "\u2657" : "\u265D";
        }
    };
}

#endif
