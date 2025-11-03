#include <assert.h>
#include <iostream>
#include "Chess.h"
#include "ChessBoard.hh"
#include "ChessPiece.hh"

void test_part1_4x4_1()
{
    Student::ChessBoard sBoard(4, 4);
    sBoard.createChessPiece(White, Rook, 3, 2);
    sBoard.createChessPiece(Black, Bishop, 1, 3);
    sBoard.createChessPiece(Black, Rook, 1, 1);
    sBoard.createChessPiece(White, Rook, 2, 3);

    std::cout << sBoard.displayBoard().str();

    std::cout << "Testing Rook at (3,2) can move to (3,0): " 
              << (sBoard.isValidMove(3, 2, 3, 0) ? "PASS" : "FAIL") << std::endl;
    
    std::cout << "Testing Rook at (3,2) cannot move to (2,1): " 
              << (!sBoard.isValidMove(3, 2, 2, 1) ? "PASS" : "FAIL") << std::endl;
    
    std::cout << "Testing Bishop at (1,3) can move to (2,2): " 
              << (sBoard.isValidMove(1, 3, 2, 2) ? "PASS" : "FAIL") << std::endl;
    
    std::cout << "Testing Rook at (3,2) can move to (0,2): " 
              << (sBoard.isValidMove(3, 2, 0, 2) ? "PASS" : "FAIL") << std::endl;

    return;
}
void testMovePieceWithTurn() {
    std::cout << "=== Test Move Piece with Turn ===" << std::endl;

    Student::ChessBoard board(8, 8);
    board.createChessPiece(White, Pawn, 6, 0);
    board.createChessPiece(Black, Pawn, 1, 0);

    // White's turn
    std::cout << "White Pawn at (6,0) moves to (5,0): " 
              << (board.movePiece(6, 0, 5, 0) ? "PASS" : "FAIL") << std::endl;

    // Black's turn
    std::cout << "Black Pawn at (1,0) moves to (2,0): " 
              << (board.movePiece(1, 0, 2, 0) ? "PASS" : "FAIL") << std::endl;

    // White tries to move again (should fail)
    std::cout << "White Pawn at (5,0) moves to (4,0): " 
              << (board.movePiece(5, 0, 4, 0) ? "PASS: Move Failed" : "FAIL") << std::endl;

    // Black tries to move out of turn (should fail)
    std::cout << "Black Pawn at (2,0) moves to (3,0): " 
              << (board.movePiece(2, 0, 3, 0) ? "PASS: Move Failed" : "FAIL") << std::endl;
}

int main()
{
    test_part1_4x4_1();
    testMovePieceWithTurn();
    return EXIT_SUCCESS;
}
 