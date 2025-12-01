#ifndef __KNIGHTPIECE_H__
#define __KNIGHTPIECE_H__

#include "ChessPiece.hh"
#include <cmath>

namespace Student
{
    class KnightPiece : public ChessPiece
    {
    public:
        KnightPiece(ChessBoard &board, Color color, int row, int column)
            : ChessPiece(board, color, row, column)
        {
            _type = Knight;
        }

        bool canMoveToLocation(int toRow, int toColumn) override
        {
            int rowDiff = abs(toRow - _row);
            int colDiff = abs(toColumn - _column);

            //L shape movement
            return (rowDiff == 2 && colDiff == 1) ||
                   (rowDiff == 1 && colDiff == 2);
        }

        const char *toString() override
        {
            return _color == White ? "\u2658" : "\u265E";
        }
    };
}

#endif
