//
//  main.cpp
//  Assignment 2
//
//  Created by Christopher Watson on 9/30/15.
//  Copyright (c) 2015 Christopher Watson. All rights reserved.
//
#include "gameOfLife.h"
#include <iostream>

int main(int argc, const char * argv[]) {
    
    std::cout << "Starting Program" << std::endl;
    
    gameOfLife gol;
    
    int gameMode;
    bool randomSelected;
    
    gameMode = gol.gameModeSelection();
    
    randomSelected = gol.importOrRandom();
    
    gol.setUpGame(gameMode, randomSelected);
    
    
    std::cout << "\n\n\aProgram has finished running." << std::endl;
    
    return 0;
}
