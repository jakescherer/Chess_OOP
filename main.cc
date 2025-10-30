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

int main()
{
    test_part1_4x4_1();
    return EXIT_SUCCESS;
}
 