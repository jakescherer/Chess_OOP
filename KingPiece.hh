#ifndef __KINGPIECE_H__
#define __KINGPIECE_H__

#include "ChessPiece.hh"
#include <cmath>

/**
 * Student implementation of a King chess piece.
 * The class is defined within the Student namespace.
 */
namespace Student
{
    class KingPiece : public ChessPiece
    {
    public:
        KingPiece(ChessBoard &board, Color color, int row, int column)
            : ChessPiece(board, color, row, column) {
            _type = King;
        }

        bool canMoveToLocation(int toRow, int toColumn) override
        {
            int rowDiff = abs(toRow - _row);
            int colDiff = abs(toColumn - _column);
            return (rowDiff <= 1 && colDiff <= 1) && 
                   !(rowDiff == 0 && colDiff == 0);
        }

        const char *toString() override
        {
            return _color == White ? "\u2654" : "\u265A";
        }
    };
}

#endif
