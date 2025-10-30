#ifndef _ROOKPIECE_H__
#define _ROOKPIECE_H__

#include "ChessPiece.hh"

/**
 * Student implementation of a Rook chess piece.
 * The class is defined within the Student namespace.
 */
namespace Student
{
    class RookPiece : public ChessPiece
    {
    public:
        RookPiece(ChessBoard &board, Color color, int row, int column)
            : ChessPiece(board, color, row, column) {
            _type = Rook;
        }

        bool canMoveToLocation(int toRow, int toColumn) override
        {
            return (toRow == _row || toColumn == _column) && 
                   !(toRow == _row && toColumn == _column);
        }

        const char *toString() override
        {
            return _color == White ? "\u2656" : "\u265C";
        }
    };
}

#endif
