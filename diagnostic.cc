#include <iostream>
#include "Chess.h"
#include "ChessBoard.hh"

int main() {
    std::cout << "=== Diagnostic Tests ===" << std::endl;
    
    // Test 1: Pawn moving from different starting positions
    std::cout << "\nTest 1: White Pawn starting positions" << std::endl;
    Student::ChessBoard board1(8, 8);
    board1.createChessPiece(White, Pawn, 6, 0);  // Standard chess starting position
    std::cout << "White Pawn at (6,0) can move to (5,0): " << board1.isValidMove(6, 0, 5, 0) << std::endl;
    std::cout << "White Pawn at (6,0) can move to (4,0): " << board1.isValidMove(6, 0, 4, 0) << std::endl;
    
    // Test 2: Black Pawn starting positions
    std::cout << "\nTest 2: Black Pawn starting positions" << std::endl;
    Student::ChessBoard board2(8, 8);
    board2.createChessPiece(Black, Pawn, 1, 0);  // Standard chess starting position
    std::cout << "Black Pawn at (1,0) can move to (2,0): " << board2.isValidMove(1, 0, 2, 0) << std::endl;
    std::cout << "Black Pawn at (1,0) can move to (3,0): " << board2.isValidMove(1, 0, 3, 0) << std::endl;
    
    // Test 3: Pawn diagonal capture
    std::cout << "\nTest 3: Pawn diagonal capture" << std::endl;
    Student::ChessBoard board3(4, 4);
    board3.createChessPiece(White, Pawn, 2, 1);
    board3.createChessPiece(Black, Pawn, 1, 2);
    std::cout << "White Pawn at (2,1) can capture at (1,2): " << board3.isValidMove(2, 1, 1, 2) << std::endl;
    std::cout << "White Pawn at (2,1) can move to (1,1): " << board3.isValidMove(2, 1, 1, 1) << std::endl;
    
    // Test 4: Piece moving to same square
    std::cout << "\nTest 4: Piece moving to same square" << std::endl;
    Student::ChessBoard board4(4, 4);
    board4.createChessPiece(White, Rook, 2, 2);
    std::cout << "Rook at (2,2) can move to (2,2): " << board4.isValidMove(2, 2, 2, 2) << std::endl;
    
    return 0;
}
