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

            // Forward movement along same column
            if (colDiff == 0) {
                // One square forward
                if (rowDiff == direction) {
                    return _board->getPiece(toRow, toColumn) == nullptr;
                }
                
                // Two squares forward - special starting positions
                int boardSize = _board->getNumRows();
                bool canMoveTwoSquares = false;
                
                if (_color == Black && _row == 1) {
                    canMoveTwoSquares = true;
                } else if (_color == White && _row == boardSize - 2) {
                    canMoveTwoSquares = true;
                }
                
                if (canMoveTwoSquares && rowDiff == 2 * direction) {
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
