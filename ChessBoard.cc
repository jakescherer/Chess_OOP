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

    bool willPromote = false;
    if (piece->getType() == Pawn) {
        if ((piece->getColor() == White && toRow == 0) || (piece->getColor() == Black && toRow == numRows - 1)) {
            willPromote = true;
        }
    }

    ChessPiece *targetPiece = board[toRow][toColumn];
    if (targetPiece != nullptr && targetPiece->getColor() == piece->getColor()) {
        return false;
    }

    if (!piece->canMoveToLocation(toRow, toColumn)) {
        if (piece->getType() == King && fromRow == toRow && abs(toColumn - fromColumn) == 2) {
            if (piece->getHasMoved()) {
                return false;
            }
            
            if (isPieceUnderThreat(fromRow, fromColumn)) {
                return false;
            }
            
            int direction = (toColumn > fromColumn) ? 1 : -1;
            int rookCol = (direction == 1) ? numCols - 1 : 0;
            int rookRow = fromRow;
            
            ChessPiece *rook = board[rookRow][rookCol];
            
            if (rook == nullptr || rook->getType() != Rook || 
                rook->getColor() != piece->getColor() || rook->getHasMoved()) {
                return false;
            }
            
            int start = (direction == 1) ? fromColumn + 1 : rookCol + 1;
            int end = (direction == 1) ? rookCol : fromColumn;
            for (int col = start; col < end; col++) {
                if (board[fromRow][col] != nullptr) {
                    return false;
                }
            }
            
            int skipCol = fromColumn + direction;
            
            ChessPiece *originalAtSkip = board[fromRow][skipCol];
            board[fromRow][skipCol] = piece;
            board[fromRow][fromColumn] = nullptr;
            int origRow = piece->getRow();
            int origCol = piece->getColumn();
            piece->setPositionWithoutMoving(fromRow, skipCol);
            
            bool skipUnderThreat = isPieceUnderThreat(fromRow, skipCol);
            
            board[fromRow][fromColumn] = piece;
            board[fromRow][skipCol] = originalAtSkip;
            piece->setPositionWithoutMoving(origRow, origCol);
            
            if (skipUnderThreat) {
                return false;
            }
        } else {
            return false;
        }
    }

    Color movingColor = piece->getColor();
    int kingRow, kingCol;
    if (findKing(movingColor, kingRow, kingCol)) {
        ChessPiece *capturedPiece = board[toRow][toColumn];
        int originalRow = piece->getRow();
        int originalCol = piece->getColumn();

        ChessPiece *movedPiece = piece;
        ChessPiece *promotedSimulated = nullptr;
        if (willPromote) {
            promotedSimulated = new Student::QueenPiece(*this, piece->getColor(), toRow, toColumn);
            movedPiece = promotedSimulated;
        }

        board[toRow][toColumn] = movedPiece;
        board[fromRow][fromColumn] = nullptr;
        movedPiece->setPositionWithoutMoving(toRow, toColumn);

        bool kingWouldBeInCheck = false;
        if (findKing(movingColor, kingRow, kingCol)) {
            kingWouldBeInCheck = isPieceUnderThreat(kingRow, kingCol);
        }

        board[fromRow][fromColumn] = piece;
        board[toRow][toColumn] = capturedPiece;
        piece->setPositionWithoutMoving(originalRow, originalCol);

        if (promotedSimulated != nullptr) {
            delete promotedSimulated;
        }

        if (kingWouldBeInCheck) {
            return false;
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
                return false;
            }
            currentRow += rowStep;
            currentCol += colStep;
        }
    }
    return true;
}

bool ChessBoard::movePiece(int fromRow, int fromColumn, int toRow, int toColumn)
{
    bool oldEP = enPassantAvailable;
    int oldRow = enPassantRow;
    int oldCol = enPassantCol;
    ChessPiece *piece = getPiece(fromRow, fromColumn);
    if (piece == nullptr || piece->getColor() != turn) {
        return false;
    }

    if (!isValidMove(fromRow, fromColumn, toRow, toColumn)) {
        return false;
    }

    bool isCastling = false;
    ChessPiece *rookToMove = nullptr;
    int rookFromCol = -1;
    int rookToCol = -1;
    
    if (piece->getType() == King && fromRow == toRow && abs(toColumn - fromColumn) == 2) {
        isCastling = true;
        int direction = (toColumn > fromColumn) ? 1 : -1;
        rookFromCol = (direction == 1) ? numCols - 1 : 0;
        rookToCol = fromColumn + direction;
        rookToMove = board[fromRow][rookFromCol];
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

    if (targetPiece != nullptr && targetPiece->getColor() != piece->getColor()) {
        delete targetPiece;
    }

    board[toRow][toColumn] = piece;
    board[fromRow][fromColumn] = nullptr;
    piece->setPosition(toRow, toColumn);
    
    if (isCastling && rookToMove != nullptr) {
        board[fromRow][rookToCol] = rookToMove;
        board[fromRow][rookFromCol] = nullptr;
        rookToMove->setPosition(fromRow, rookToCol);
    }
    
    if (piece->getType() == Pawn) {
        int lastRowWhite = 0;
        int lastRowBlack = numRows - 1;
        if ((piece->getColor() == White && toRow == lastRowWhite) || (piece->getColor() == Black && toRow == lastRowBlack)) {
            Color promoteColor = piece->getColor();
            delete piece;
            ChessPiece* newQueen = new Student::QueenPiece(*this, promoteColor, toRow, toColumn);
            board[toRow][toColumn] = newQueen;
            piece = newQueen;
        }
    }
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

            if (attacker == nullptr) {
                continue;
            }
            if (attacker->getColor() == targetColor) {
                continue;
            }
            
            if (!attacker->canMoveToLocation(row, col)) {
                continue;
            }

            Type pieceType = attacker->getType();
            if (pieceType == Rook || pieceType == Bishop || pieceType == Queen) {
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

            return true;
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
    return false;
}

bool ChessBoard::wouldBeInCheck(int fromRow, int fromColumn, int toRow, int toColumn)
{
    ChessPiece *movingPiece = board[fromRow][fromColumn];
    if (movingPiece == nullptr) {
        return false;
    }

    Color movingColor = movingPiece->getColor();
    
    ChessPiece *capturedPiece = board[toRow][toColumn];
    int originalRow = movingPiece->getRow();
    int originalCol = movingPiece->getColumn();
    
    board[toRow][toColumn] = movingPiece;
    board[fromRow][fromColumn] = nullptr;
    movingPiece->setPositionWithoutMoving(toRow, toColumn);
    
    int kingRow, kingCol;
    bool kingFound = findKing(movingColor, kingRow, kingCol);
    
    bool inCheck = false;
    if (kingFound) {
        inCheck = isPieceUnderThreat(kingRow, kingCol);
    }
    
    board[fromRow][fromColumn] = movingPiece;
    board[toRow][toColumn] = capturedPiece;
    movingPiece->setPositionWithoutMoving(originalRow, originalCol);
    
    return inCheck;
}

float ChessBoard::scoreBoard()
{
    int myScore = 0;
    int opponentScore = 0;
    
    Color myColor = turn;
    Color opponentColor = (turn == White) ? Black : White;
    
    for (int r = 0; r < numRows; r++) {
        for (int c = 0; c < numCols; c++) {
            ChessPiece *piece = board[r][c];
            if (piece == nullptr) {
                continue;
            }
            
            int pieceValue = 0;
            switch (piece->getType()) {
                case King:
                    pieceValue = 2000;
                    break;
                case Queen:
                    pieceValue = 90;
                    break;
                case Rook:
                    pieceValue = 50;
                    break;
                case Knight:
                case Bishop:
                    pieceValue = 30;
                    break;
                case Pawn:
                    pieceValue = 10;
                    break;
            }
            
            if (piece->getColor() == myColor) {
                myScore += pieceValue;
            } else {
                opponentScore += pieceValue;
            }
        }
    }
    
    for (int r = 0; r < numRows; r++) {
        for (int c = 0; c < numCols; c++) {
            ChessPiece *piece = board[r][c];
            if (piece == nullptr || piece->getColor() != myColor) {
                continue;
            }
            
            for (int toR = 0; toR < numRows; toR++) {
                for (int toC = 0; toC < numCols; toC++) {
                    if (isValidMove(r, c, toR, toC)) {
                        myScore++;
                    }
                }
            }
        }
    }
    
    Color originalTurn = turn;
    turn = opponentColor;
    
    for (int r = 0; r < numRows; r++) {
        for (int c = 0; c < numCols; c++) {
            ChessPiece *piece = board[r][c];
            if (piece == nullptr) {
                continue;
            }
            
            if (piece->getColor() == opponentColor) {
                for (int toR = 0; toR < numRows; toR++) {
                    for (int toC = 0; toC < numCols; toC++) {
                        if (isValidMove(r, c, toR, toC)) {
                            opponentScore++;
                        }
                    }
                }
            }
        }
    }
    
    turn = originalTurn;
    
    return (myScore - opponentScore) / 10.0f;
}

float ChessBoard::getHighestNextScore()
{
    float highestScore = -999999.0f;
    
    for (int fromR = 0; fromR < numRows; fromR++) {
        for (int fromC = 0; fromC < numCols; fromC++) {
            ChessPiece *piece = board[fromR][fromC];
            if (piece == nullptr || piece->getColor() != turn) {
                continue;
            }
            
            for (int toR = 0; toR < numRows; toR++) {
                for (int toC = 0; toC < numCols; toC++) {
                    if (isValidMove(fromR, fromC, toR, toC)) {
                        ChessPiece *targetPiece = board[toR][toC];
                        int origRow = piece->getRow();
                        int origCol = piece->getColumn();
                        
                        board[toR][toC] = piece;
                        board[fromR][fromC] = nullptr;
                        piece->setPositionWithoutMoving(toR, toC);
                        
                        Color originalTurn = turn;
                        turn = (turn == White) ? Black : White;
                        
                        float score = -scoreBoard();
                        
                        turn = originalTurn;
                        
                        board[fromR][fromC] = piece;
                        board[toR][toC] = targetPiece;
                        piece->setPositionWithoutMoving(origRow, origCol);
                        
                        if (score > highestScore) {
                            highestScore = score;
                        }
                    }
                }
            }
        }
    }
    
    return highestScore;
}
