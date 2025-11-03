#include "ChessBoard.hh"
#include "PawnPiece.hh"
#include "RookPiece.hh"
#include "BishopPiece.hh"
#include "KingPiece.hh"

using Student::ChessBoard;
using Student::ChessPiece;

ChessBoard::ChessBoard(int numRow, int numCol)
{
    numRows = numRow;
    numCols = numCol;
    board.resize(numRows);
    for (int i = 0; i < numRows; i++) {
        board[i].resize(numCols, nullptr);
    }
}

ChessBoard::~ChessBoard()
{
    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numCols; j++) {
            if (board[i][j] != nullptr) {
                delete board[i][j];
            }
        }
    }
}

void ChessBoard::createChessPiece(Color col, Type ty, int startRow, int startColumn)
{
    if (board[startRow][startColumn] != nullptr) {
        delete board[startRow][startColumn];
    }

    ChessPiece *piece = nullptr;
    switch (ty) {
        case Pawn:
            piece = new Student::PawnPiece(*this, col, startRow, startColumn);
            break;
        case Rook:
            piece = new Student::RookPiece(*this, col, startRow, startColumn);
            break;
        case Bishop:
            piece = new Student::BishopPiece(*this, col, startRow, startColumn);
            break;
        case King:
            piece = new Student::KingPiece(*this, col, startRow, startColumn);
            break;
     
    }
    board[startRow][startColumn] = piece;
}

bool ChessBoard::isValidMove(int fromRow, int fromColumn, int toRow, int toColumn)
{
    if (fromRow < 0 || fromRow >= numRows || fromColumn < 0 || fromColumn >= numCols) {
        return false;
    }
    if (toRow < 0 || toRow >= numRows || toColumn < 0 || toColumn >= numCols) {
        return false;
    }

    ChessPiece *piece = board[fromRow][fromColumn];
    if (piece == nullptr) {
        return false;
    }

    ChessPiece *targetPiece = board[toRow][toColumn];
    if (targetPiece != nullptr && targetPiece->getColor() == piece->getColor()) {
        return false;
    }

    if (!piece->canMoveToLocation(toRow, toColumn)) {
        return false;
    }

    Type pieceType = piece->getType();
    if (pieceType == Rook || pieceType == Bishop) {
        int rowStep = 0;
        int colStep = 0;

        if (toRow != fromRow) {
            rowStep = (toRow > fromRow) ? 1 : -1;
        }
        if (toColumn != fromColumn) {
            colStep = (toColumn > fromColumn) ? 1 : -1;
        }

        int currentRow = fromRow + rowStep;
        int currentCol = fromColumn + colStep;

        while (currentRow != toRow || currentCol != toColumn) {
            if (board[currentRow][currentCol] != nullptr) {
                return false; // Path is obstructed
            }
            currentRow += rowStep;
            currentCol += colStep;
        }
    }

    return true;
}

bool ChessBoard::movePiece(int fromRow, int fromColumn, int toRow, int toColumn)
{
    // Check if the piece being moved belongs to the current team
    ChessPiece *piece = getPiece(fromRow, fromColumn);
    if (piece == nullptr || piece->getColor() != turn) {
        return false; // no piece or not the team's turn
    }

    if (isValidMove(fromRow, fromColumn, toRow, toColumn)) {
        ChessPiece *targetPiece = getPiece(toRow, toColumn);

        // delete other team target if found
        if (targetPiece != nullptr && targetPiece->getColor() != piece->getColor()) {
            delete targetPiece;
        }

        // Move piece
        board[toRow][toColumn] = piece;
        board[fromRow][fromColumn] = nullptr;
        piece->setPosition(toRow, toColumn);

        // Toggle turn
        turn = (turn == White) ? Black : White;

        return true;
    }

    return false;
}

bool ChessBoard::isPieceUnderThreat(int row, int col) {
    ChessPiece *target = getPiece(row, col);
    if (target == nullptr || target->getColor() == turn) {
        return false;
    }

    Color targetColor = target->getColor();

    for (int r = 0; r < numRows; r++){
        for (int c = 0; c < numCols; c++) {
            ChessPiece *attacker = getPiece(r, c);

            if (attacker == nullptr) { //empty space
                continue;
            }
            if (attacker->getColor() == targetColor) {//same team
                continue;
            }
            // Use isValidMove to check if attacker can reach target for path obstruction
            if (isValidMove(r, c, row, col)) {
                return true;
            }
        }
    }

    return false;
}

std::ostringstream ChessBoard::displayBoard()
{
    std::ostringstream outputString;
    outputString << "  ";
    for (int i = 0; i < numCols; i++){
        outputString << i << " ";
    }
    outputString << std::endl << "  ";
    for (int i = 0; i < numCols; i++){
        outputString << "– ";
    }
    outputString << std::endl;

    for (int row = 0; row < numRows; row++){
        outputString << row << "|";
        for (int column = 0; column < numCols; column++){
            ChessPiece *piece = board.at(row).at(column);
            outputString << (piece == nullptr ? " " : piece->toString()) << " ";
        }
        outputString << "|" << std::endl;
    }

    outputString << "  ";
    for (int i = 0; i < numCols; i++){
        outputString << "– ";
    }
    outputString << std::endl << std::endl;

    return outputString;
}
