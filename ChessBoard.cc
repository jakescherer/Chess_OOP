#include "ChessBoard.hh"
#include "PawnPiece.hh"
#include "RookPiece.hh"
#include "BishopPiece.hh"
#include "KingPiece.hh"
#include "KnightPiece.hh"
#include "QueenPiece.hh"

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
        case Knight:
            piece = new Student::KnightPiece(*this, col, startRow, startColumn);
            break;
        case Queen:
            piece = new Student::QueenPiece(*this, col, startRow, startColumn);
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

    // Check ALL check-related constraints: 
    // 1. If king is in check, move must resolve it
    // 2. Move cannot leave own king in check
    Color movingColor = piece->getColor();
    int kingRow, kingCol;
    if (findKing(movingColor, kingRow, kingCol)) {
        // Simulate the move to check if king would be in check afterwards
        ChessPiece *capturedPiece = board[toRow][toColumn];
        int originalRow = piece->getRow();
        int originalCol = piece->getColumn();
        
        // Make the move temporarily
        board[toRow][toColumn] = piece;
        board[fromRow][fromColumn] = nullptr;
        piece->setPosition(toRow, toColumn);
        
        // Check if king would be in check after this move
        bool kingWouldBeInCheck = false;
        if (findKing(movingColor, kingRow, kingCol)) {
            kingWouldBeInCheck = isPieceUnderThreat(kingRow, kingCol);
        }
        
        // Rollback the move
        board[fromRow][fromColumn] = piece;
        board[toRow][toColumn] = capturedPiece;
        piece->setPosition(originalRow, originalCol);
        
        if (kingWouldBeInCheck) {
            return false; // Move would leave king in check
        }
    }

    Type pieceType = piece->getType();
    if (pieceType == Rook || pieceType == Bishop || pieceType == Queen) {
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

    // Kings cannot move into check
    if (pieceType == King) {
        ChessPiece *capturedPiece = board[toRow][toColumn];
        int originalRow = piece->getRow();
        int originalCol = piece->getColumn();
        
        board[toRow][toColumn] = piece;
        board[fromRow][fromColumn] = nullptr;
        piece->setPosition(toRow, toColumn);
        
        bool wouldBeUnderThreat = isPieceUnderThreat(toRow, toColumn);
        
        // rollback move
        board[fromRow][fromColumn] = piece;
        board[toRow][toColumn] = capturedPiece;
        piece->setPosition(originalRow, originalCol);
        
        if (wouldBeUnderThreat) {
            return false; // King cannot move into check
        }
    }

    return true;
}

bool ChessBoard::movePiece(int fromRow, int fromColumn, int toRow, int toColumn)
{
    bool oldEP = enPassantAvailable;
    int oldRow = enPassantRow;
    int oldCol = enPassantCol;
    // Check if the piece being moved belongs to the current team
    ChessPiece *piece = getPiece(fromRow, fromColumn);
    if (piece == nullptr || piece->getColor() != turn) {
        return false; // no piece or not the team's turn
    }

    // basic move check (bounds, piece movement rules, path obstruction, check prevention)
    if (!isValidMove(fromRow, fromColumn, toRow, toColumn)) {
        return false;
    }

    ChessPiece *targetPiece = getPiece(toRow, toColumn);
    
    if (piece->getType() == Pawn &&
        targetPiece == nullptr &&
        toColumn != fromColumn)
    {
        if (oldEP && toRow == oldRow && toColumn == oldCol)
        {
            int capturedPawnRow = (piece->getColor() == White) ? toRow + 1 : toRow - 1;
            ChessPiece* capturedPawn = board[capturedPawnRow][toColumn];
            if (capturedPawn != nullptr && capturedPawn->getType() == Pawn && capturedPawn->getColor() != piece->getColor())
            {
                delete capturedPawn;
                board[capturedPawnRow][toColumn] = nullptr;
            }
        }
    }

     enPassantAvailable = false;

    if (piece->getType() == Pawn) {
        int direction = (piece->getColor() == White) ? -1 : 1;
        if (toRow == fromRow + 2 * direction) {
            enPassantAvailable = true;
            enPassantRow = fromRow + direction;
            enPassantCol = fromColumn;
            enPassantColor = piece->getColor();
        }
    }

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

bool ChessBoard::isPieceUnderThreat(int row, int col) {
    ChessPiece *target = getPiece(row, col);
    if (target == nullptr) {
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
            
            // Simple line of attack check - can this piece reach the target?
            if (!attacker->canMoveToLocation(row, col)) {
                continue;
            }

            // Check path obstruction for Rook and Bishop
            Type pieceType = attacker->getType();
            if (pieceType == Rook || pieceType == Bishop) {
                int rowStep = 0;
                int colStep = 0;

                if (row != r) {
                    rowStep = (row > r) ? 1 : -1;
                }
                if (col != c) {
                    colStep = (col > c) ? 1 : -1;
                }

                int currentRow = r + rowStep;
                int currentCol = c + colStep;
                bool pathClear = true;

                while (currentRow != row || currentCol != col) {
                    if (board[currentRow][currentCol] != nullptr) {
                        pathClear = false;
                        break;
                    }
                    currentRow += rowStep;
                    currentCol += colStep;
                }
                
                if (!pathClear) {
                    continue;
                }
            }

            return true; // Found an attacker with clear line of attack
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

bool ChessBoard::findKing(Color color, int &kingRow, int &kingCol)
{
    for (int r = 0; r < numRows; r++) {
        for (int c = 0; c < numCols; c++) {
            ChessPiece *piece = board[r][c];
            if (piece != nullptr && piece->getType() == King && piece->getColor() == color) {
                kingRow = r;
                kingCol = c;
                return true;
            }
        }
    }
    return false; // king not found (shouldn't happen in a valid game)
}

bool ChessBoard::wouldBeInCheck(int fromRow, int fromColumn, int toRow, int toColumn)
{
    ChessPiece *movingPiece = board[fromRow][fromColumn];
    if (movingPiece == nullptr) {
        return false; // No piece to move
    }

    Color movingColor = movingPiece->getColor();
    
    // Save current state for rollback
    ChessPiece *capturedPiece = board[toRow][toColumn];
    int originalRow = movingPiece->getRow();
    int originalCol = movingPiece->getColumn();
    
    board[toRow][toColumn] = movingPiece;
    board[fromRow][fromColumn] = nullptr;
    movingPiece->setPosition(toRow, toColumn);
    
    int kingRow, kingCol;
    bool kingFound = findKing(movingColor, kingRow, kingCol);
    
    bool inCheck = false;
    if (kingFound) {
        inCheck = isPieceUnderThreat(kingRow, kingCol);
    }
    
    // Rollback the move
    board[fromRow][fromColumn] = movingPiece;
    board[toRow][toColumn] = capturedPiece;
    movingPiece->setPosition(originalRow, originalCol);
    
    return inCheck;
}
