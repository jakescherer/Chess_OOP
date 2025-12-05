#include "ChessPiece.hh"
#include "ChessBoard.hh"

using Student::ChessPiece;
using Student::ChessBoard;

ChessPiece::ChessPiece(ChessBoard &board, Color color, int row, int column)
{
    _board = &board;
    _color = color;
    _row = row;
    _column = column;
    _type = Pawn;
    _hasMoved = false;
}

Color ChessPiece::getColor()
{
    return _color;
}

Type ChessPiece::getType()
{
    return _type;
}

int ChessPiece::getRow()
{
    return _row;
}

int ChessPiece::getColumn()
{
    return _column;
}

bool ChessPiece::getHasMoved()
{
    return _hasMoved;
}

void ChessPiece::setPosition(int row, int column)
{
    _row = row;
    _column = column;
    _hasMoved = true;
}

void ChessPiece::setPositionWithoutMoving(int row, int column)
{
    _row = row;
    _column = column;
}
