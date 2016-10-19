//
//  gameOfLife.h
//  Assignment 2
//
//  Created by Christopher Watson on 9/30/15.
//  Copyright (c) 2015 Christopher Watson. All rights reserved.
//

#ifndef __Assignment_2__gameOfLife__
#define __Assignment_2__gameOfLife__

#include <stdio.h>
#include <iostream>


class gameOfLife{
    
public:
    
    gameOfLife(){
    }
    
    ~gameOfLife(){
    }
    
    void setUpGame(int gameMode, bool randomSelected);
    int gameModeSelection();
    bool importOrRandom();
    int reportStyleSelection();
    int* mapFromRandom (int height, int width, int fillSelection);
    int* mapFromInput (int height, int width, int fillSelection, int* dataInput);
    void runSimulations (int height, int width, int* originalMap, int gameMode, int reportStyle);
    
    
};

#endif /* defined(__Assignment_2__gameOfLife__) */
