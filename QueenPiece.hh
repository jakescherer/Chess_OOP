#ifndef __QUEENPIECE_H__
#define __QUEENPIECE_H__

#include "ChessPiece.hh"
#include <cmath>

namespace Student
{
    class QueenPiece : public ChessPiece
    {
    public:
        QueenPiece(ChessBoard &board, Color color, int row, int column)
            : ChessPiece(board, color, row, column)
        {
            _type = Queen;
        }

        bool canMoveToLocation(int toRow, int toColumn) override
        {
            int rowDiff = abs(toRow - _row);
            int colDiff = abs(toColumn - _column);

            if (toRow == _row && toColumn == _column)
                return false;

            if (toRow == _row || toColumn == _column)
                return true;

            if (rowDiff == colDiff)
                return true;

            return false;
        }

        const char *toString() override
        {
            return _color == White ? "\u2655" : "\u265B";
        }
    };
}

#endif
