//
//  gameOfLife.cpp
//  Assignment 2
//
//  Created by Christopher Watson on 9/30/15.
//  Copyright (c) 2015 Christopher Watson. All rights reserved.
//

#include "gameOfLife.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <time.h>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <chrono>
#include <thread>
#include <unistd.h>

using namespace std;


//====================================================================================
#pragma mark
#pragma mark Game Mode Selection
//====================================================================================
//Func to select Game Mode
int gameOfLife::gameModeSelection (){
    int gameMode;
    string gameModeName;
    
    cout << "WELCOME TO THE GAME OF LIFE!\nWhich game mode would you like to play?\n";
    cout << "\t 1) Classic\n\t 2) Doughnut\n\t 3) Mirrored" << endl;
    do {
        cout << "\nSelect (1,2,3)?: ";
        cin >> gameMode;
    }while ((gameMode != 1) && (gameMode != 2) && (gameMode != 3));
    
    if (gameMode == 1)
        gameModeName = "Classic";
    else if (gameMode == 2)
        gameModeName = "Doughnut";
    else if (gameMode == 3)
        gameModeName = "Mirrored";
    
    cout << "\n\nYou have selected " << gameModeName;
    
    
    return gameMode;

}

//====================================================================================
#pragma mark
#pragma mark Import File or Random
//====================================================================================
//Func to select Random or Input
bool gameOfLife::importOrRandom (){
    int selection;
    bool isRandom;
    string randomSelectedName;
    
    cout << "Would you like to create the Game Map by:";
    cout << "\n\t 0) Inputting a .txt file yourself using the name\n\t 1) Generating a Random Game Map" << endl;
    do {
        cout << "\nSelect (0,1)?: ";
        cin >> selection;
    }while ((selection != 0) && (selection != 1));
    
    if (selection == 0){
        randomSelectedName = "input a .txt source file.";
        isRandom = false;
    }
    else if (selection == 1){
        randomSelectedName = "Generate an Random Game Map. ";
        isRandom = true;
    }
    cout << "\n\nYou have selected to " << randomSelectedName << endl;
    
    return isRandom;
}
//====================================================================================
#pragma mark
#pragma mark Report Style
//====================================================================================
//Func to select Report Style
int gameOfLife::reportStyleSelection (){
    int reportSelection;
    string reportSelectedName;
    
    cout << "\n\nOne Last Thing before we play...\nHow would you like to report the generation changes?:";
    cout << "\n\t 1) Progress generations by pressing ENTER\n\t 2) Outputting to a .txt file\n\t 3) Wait for a slight pause before continuing" << endl;
    do {
        cout << "\nSelect (1,2,3)?: ";
        cin >> reportSelection;
    }while ((reportSelection != 1) && (reportSelection != 2) && (reportSelection != 3));
    
    if (reportSelection == 1)
        reportSelectedName = "Press ENTER to proceed generations";
    else if (reportSelection == 2)
        reportSelectedName = "Output to a .txt file";
    else if (reportSelection == 3)
        reportSelectedName = "allow for a small pause before proceeding";
    
    
    cout << "\n\nYou have selected to " << reportSelectedName << "\n\n";
    
    return reportSelection;
}

//====================================================================================
#pragma mark
#pragma mark Initial Map Creation -- INPUT FILE
//====================================================================================
int* gameOfLife::mapFromInput (int height, int width, int fillSelection, int* dataInput){
    int Y(height + 2);
    int X(width + 2);
    int sizeOfFile(height * width);
    int* dataFromFile = new int[sizeOfFile];
    int size(X);
    for (int i(0); i < sizeOfFile; ++i){
        dataFromFile[i] = (dataInput)[i];
//        cout << dataFromFile[i];
    }
    delete [] dataInput;
    
    int sizeOfData(X * Y);
    int *listOfData = new int[sizeOfData];
    
    //Copy the file data into a contained Box (+2's)
    int location(0);
    for (int j(0); j < Y; ++j){
        for (int k(0); k < X; ++k){
            if ((j == 0) || (k == 0) || (j == (Y-1)) || (k == (X-1))){
                listOfData[(j*size)+k] = 0;
            }
            else {
                listOfData[(j*size)+k] = dataFromFile[location];
                ++location;
            }
        }
    }

    
    return listOfData;
    
}

//====================================================================================
#pragma mark
#pragma mark Initial Map Creation -- RANDOM
//====================================================================================
//Func to create map
int* gameOfLife::mapFromRandom (int height, int width, int fillSelection){
    //Create 2 Extra to Put array inside a box to allow for mode calculations
    int X(width + 2);
    int Y(height + 2);
    string inputData = "";
    //Create 1D Array of Data



    srand(time(NULL));
    
    int sizeOfData(X * Y);
    int *listOfData = new int[sizeOfData];
    int fillOrNaw(0);
    for (int i(0); i <= sizeOfData; ++i){
        fillOrNaw = rand() % 2;
        listOfData[i] = fillOrNaw;
    }
    
    return listOfData;

}
//====================================================================================
#pragma mark
#pragma mark Run Simulations on map
//====================================================================================
void gameOfLife::runSimulations (int height, int width, int* originalMap, int gameMode, int reportStyle){
    //Give a border to allow for calculating the gameModes
    int X(width + 2);
    int Y(height + 2);
    
    //Create file to output to
    ofstream outputFile;
    if (reportStyle == 2){
        outputFile.open ("GameStatistics.txt", std::ios::out | std::ios::app);
    }
    int size = X;
    int sizeOfData = X * Y;
    //Create two maps, a 2D map and neighbors
    int* firstMapData = new int[sizeOfData];
    
    for (int i(0); i < sizeOfData; ++i)
        firstMapData[i] = (originalMap)[i];
    
    
    int* secondMapData = new int[sizeOfData];
    
    for (int i(0); i < sizeOfData; ++i)
        secondMapData[i] = firstMapData[i];
    
    int neighbors(0);
    int replicaCount(0);
    int genNum(0);
    
    int infiniteTracker(0);
    int infiniteCounter(0);
    int* infiniteArray = new int[sizeOfData];
    
    //Assess Original Map and Copy it to New Map Array based on gameMode
    //1. Display original config
    //2. Check game type and proceed accordingly in while loop
    //3. End upon duplication of file. NOTE: If no duplication, file will continue to run.
    
    //Original Config
    cout << "-----------------------\nInitial Map Configuration" << "\n-----------------------\n";
    if (reportStyle == 2)
    outputFile << "-----------------------\nInitial Map Configuration" << "\n-----------------------\n";
    for (int j(0); j < Y; ++j){
        for (int k(0); k < X; ++k){
            if ((j != 0) && (k != 0) && (j != (Y-1)) && (k != (X-1))){
                if (firstMapData[(j * size) + k] == 1){
                    cout << "X ";
                    if (reportStyle == 2)
                    outputFile << "X ";
                }
                else{
                    cout << "- ";
                    if (reportStyle == 2)
                    outputFile << "- ";
                }
                
            }
        }
        cout << "\n";
        if (reportStyle == 2)
        outputFile << "\n";
    }
    
    while (true){
        //Keep track of infinites and end if occurrs
        if (infiniteCounter == 2){
            for (int i(0); i < sizeOfData; ++i){
                if (secondMapData[i] == infiniteArray[i])
                ++infiniteTracker;
            }
            
            if (infiniteTracker == sizeOfData){
                cout << "-----------------------\nInfinite Survival Immanent" << "\n-----------------------\n";
                if (reportStyle == 2)
                outputFile << "-----------------------\nInfinite Survival Immanent" << "\n-----------------------\n";
                break;
            }
            infiniteCounter = 0;
        }
        if (infiniteCounter == 0){
            for (int i(0); i < sizeOfData; ++i)
            infiniteArray[i] = secondMapData[i];
        }
        
        //Classic
        cout << "-----------------------\n\tGeneration " << genNum << "\n-----------------------\n";
        if (reportStyle == 2)
            outputFile << "-----------------------\n\tGeneration " << genNum << "\n-----------------------\n";
        if (gameMode == 1){
            for (int j(0); j < Y; ++j){
                for (int k(0); k < X; ++k){
                    if ((j == 0) || (k == 0) || (j == (Y-1)) || (k == (X-1)))
                        secondMapData[(j*size) + k] = 0;
                    
                    neighbors = firstMapData[(j * size) + (k + 1)] + firstMapData[((j+2) * size) + (k+2)] + firstMapData[((j+2) * size) + k] + firstMapData[((j+2) * size) + (k+1)] + firstMapData[(j * size) + (k+2)] + firstMapData[((j+1) * size) + (k+2)] + firstMapData[((j+1) * size) + k] + firstMapData[(j * size) + k];
                    
                    if (neighbors <= 1)
                        secondMapData[((j+1) * size) + (k+1)] = 0;
                    else if (neighbors == 2){
                        if (firstMapData[((j+1) * size) + (k+1)] == 1)
                            secondMapData[((j+1) * size) + (k+1)] = 1;
                        else
                            secondMapData[((j+1) * size) + (k+1)] = 0;
                    }
                    else if (neighbors == 3)
                        secondMapData[((j+1) * size) + (k+1)] = 1;
                    else if (neighbors >= 4)
                        secondMapData[((j+1) * size) + (k+1)] = 0;
                    neighbors = 0;
                }
            }
        }
        
        
        //Doughnut
        if (gameMode == 2){
            for (int j(0); j < Y; ++j){
                for (int k(0); k < X; ++k){
                    if ((j == 0) || (k == 0) || (j == (Y-1)) || (k == (X-1)))
                        secondMapData[(j*size) + k] = 0;
                    
                    //Top to bottom
                    if (j == 0){
                        secondMapData[(j*size) + k] = firstMapData[((Y-2)*size) + k];
                    }
                    //Bottom to Top
                    if (j == (Y-1)){
                        secondMapData[(j*size) + k] = firstMapData[(size + k)];
                    }
                    //Left to Right
                    if (k == 0){
                        secondMapData[(j*size) + k] = firstMapData[(j*size) + (X-2)];
                    }
                    //Right to Left
                    if (k == (X-1)){
                        secondMapData[(j*size) + k] = firstMapData[(j*size)+1];
                    }
                    //Top Left
                    else if ((j == 0) && (k == 0)){
                        secondMapData[(j*size) + k] = firstMapData[((Y-2)*size) + (X-2)];
                    }
                    //Bottom Right
                    else if ((j == (Y-1)) && (k == (X-1))){
                        secondMapData[(j*size) + k] = firstMapData[((size)+1)];
                    }
                    //Bottom Left
                    else if ((j == (Y-1)) && (k == 0)){
                        secondMapData[(j*size) + k] = firstMapData[(size)+k];
                    }
                    //Top Right
                    else if ((j == 0) && (k == (X-1))){
                        secondMapData[(j*size) + k] = firstMapData[((Y-2)*size) + 1];
                    }
                    
                    neighbors = firstMapData[(j * size) + (k + 1)] + firstMapData[((j+2) * size) + (k+2)] + firstMapData[((j+2) * size) + k] + firstMapData[((j+2) * size) + (k+1)] + firstMapData[(j * size) + (k+2)] + firstMapData[((j+1) * size) + (k+2)] + firstMapData[((j+1) * size) + k] + firstMapData[(j * size) + k];
                    
                    if (neighbors <= 1)
                        secondMapData[((j+1) * size) + (k+1)] = 0;
                    else if (neighbors == 2){
                        if (firstMapData[((j+1) * size) + (k+1)] == 1)
                            secondMapData[((j+1) * size) + (k+1)] = 1;
                        else
                            secondMapData[((j+1) * size) + (k+1)] = 0;
                    }
                    else if (neighbors == 3)
                        secondMapData[((j+1) * size) + (k+1)] = 1;
                    else if (neighbors >= 4)
                        secondMapData[((j+1) * size) + (k+1)] = 0;
                    neighbors = 0;
                }
            }
        }
        
        //Mirrored
        if (gameMode == 3){
            for (int j(0); j < Y; ++j){
                for (int k(0); k < X; ++k){
                    if ((j == 0) || (k == 0) || (j == (Y-1)) || (k == (X-1)))
                        secondMapData[(j*size) + k] = 0;
                    
                    //TopSide
                    if (j == 0){
                        secondMapData[(j*size) + k] = firstMapData[size + k];
                    }
                    //RightSide
                    if (k == (X-1)){
                        secondMapData[(j*size) + k] = firstMapData[((j * size) + k)];
                    }
                    //BottomSide
                    if (j == (Y-1)){
                        secondMapData[(j*size) + k] = firstMapData[(((Y-2) * size)) + k];
                    }
                    //LeftSide
                    if (k == 0){
                        secondMapData[(j*size) + k] = firstMapData[(j*size)+1];
                    }
                    //TL Corner
                    else if ((j == 0) && (k == 0)){
                        secondMapData[(j*size) + k] = firstMapData[((size)+1)];
                    }
                    //BR Corner
                    else if ((j == (Y-1)) && (k == (X-1))){
                        secondMapData[(j*size) + k] = firstMapData[((Y-2)*size) + (X-2)];

                    }
                    //BL Corner
                    else if ((j == (Y-1)) && (k == 0)){
                        secondMapData[(j*size) + k] = firstMapData[((Y-2)*size) + 1];

                    }
                    //TR Corner
                    else if ((j == 0) && (k == (X-1))){
                        secondMapData[(j*size) + k] = firstMapData[(size)+k];

                    }
                    
                    neighbors = firstMapData[(j * size) + (k + 1)] + firstMapData[((j+2) * size) + (k+2)] + firstMapData[((j+2) * size) + k] + firstMapData[((j+2) * size) + (k+1)] + firstMapData[(j * size) + (k+2)] + firstMapData[((j+1) * size) + (k+2)] + firstMapData[((j+1) * size) + k] + firstMapData[(j * size) + k];
                    
                    if (neighbors <= 1)
                        secondMapData[((j+1) * size) + (k+1)] = 0;
                    else if (neighbors == 2){
                        if (firstMapData[((j+1) * size) + (k+1)] == 1)
                            secondMapData[((j+1) * size) + (k+1)] = 1;
                        else
                            secondMapData[((j+1) * size) + (k+1)] = 0;
                    }
                    else if (neighbors == 3)
                        secondMapData[((j+1) * size) + (k+1)] = 1;
                    else if (neighbors >= 4)
                        secondMapData[((j+1) * size) + (k+1)] = 0;
                    neighbors = 0;
                }
            }
        }
        
        
        //Print out generated second map
        
        for (int j(0); j < Y; ++j){
            for (int k(0); k < X; ++k){
                if ((j != 0) && (k != 0) && (j != (Y-1)) && (k != (X-1))){
                    if (secondMapData[(j * size) + k] == 1){
                        cout << "X ";
                        if (reportStyle == 2)
                            outputFile << "X ";
                    }
                    else{
                        cout << "- ";
                        if (reportStyle == 2)
                            outputFile << "- ";
                    }
                    
                }
            }
            cout << "\n";
            if (reportStyle == 2)
                outputFile << "\n";
        }
        
        
        //Check to see if maps are duplicates of each other for game ending
        for (int i(0); i < sizeOfData; ++i){
            if (firstMapData[i] == secondMapData[i])
                ++replicaCount;
        }
        
        if (replicaCount == sizeOfData){
            string gameOver{"-----------------------\n\tGame Has Ended!\n-----------------------\n"};
            cout << gameOver;
            if (reportStyle == 2)
                outputFile << gameOver;
            break;
        }
        replicaCount = 0;
        infiniteTracker = 0;
        
        if (reportStyle == 1){
            cin.ignore();
        }
        else if (reportStyle == 3){
            usleep(10000);
        }
        
            
        for (int i(0); i < sizeOfData; ++i)
            firstMapData[i] = secondMapData[i];
        ++genNum;
        ++infiniteCounter;
    }
    //This deletion is necessary to prevent memleak from mapCreation funcs
    delete[] originalMap;

    delete[] firstMapData;
    delete[] secondMapData;
    delete[] infiniteArray;
    outputFile.close();
    gameOfLife::~gameOfLife();
    
}
//====================================================================================
#pragma mark
#pragma mark Game Execution
//====================================================================================
//Flow of the Game Itself
void gameOfLife::setUpGame (int gameMode, bool randomSelected){
    int mapHeight(0);
    int mapWidth(0);
    int reportStyle(0);
    char answer;
    int fillSelection(1);
    string txtFileName;
    
    
    
    //User wants a random map. DO IT
    if (randomSelected){
        
        do {
            cout << "Please enter desired dimensions of game board.\nHeight?: ";
            cin >> mapHeight;
            cout << "\nWidth?: ";
            cin >> mapWidth;
            cout << "\nYou have chosen a height of " << mapHeight << " and width of " << mapWidth << ". Is this correct? (y/n)";
            cin >> answer;
            
            if ((answer != 'y') && (answer != 'Y'))
            cout << "\nSorry. Let's try those dimensions again.";
            
        }while ((answer != 'y') && (answer != 'Y'));
        
        reportStyle = reportStyleSelection();

        runSimulations(mapHeight, mapWidth, mapFromRandom(mapHeight, mapWidth, fillSelection), gameMode, reportStyle);
    }
    
    //User wants to input a file. DO IT
    else if (!randomSelected){
        
        cout << "\nPlease type the name of the .txt file including .txt at the end of file name\nPress ENTER when finished:  ";
        cin >> txtFileName;
        ifstream inputFile(txtFileName, ios::in);
        string heightString{""};
        string widthString{""};
        string dataString{""};
        int getWidth(0);
        
        
        //Iterate through lines and chars to see what we got
        string line ;
        for(string line; getline(inputFile, line); )
        {
            
            for (int i(0); i < line.length(); ++i)
            {
                if ((line[i] == ('0')) || (line[i] == '1') ||(line[i] == '2') ||(line[i] == '3') ||(line[i] == '4') ||(line[i] == '5') ||(line[i] == '6') ||(line[i] == '7') ||(line[i] == '8') ||(line[i] == '9')){
                    
                    if (getWidth == 0){
                        heightString+=line[i];
                    }
                    else if (getWidth == 1){
                        widthString+=line[i];
                    }
                    
                }
                if (getWidth == 3){
                    dataString+=line[i];
                }
            }
            if (heightString != ""){
                getWidth = 1;
            }
            if (widthString != ""){
                getWidth = 3;
            }
        }
        
//        cout << heightString << "\n" << widthString << "\n" << dataString << "\n\n";
        
        
        mapWidth = stoi(widthString);
        mapHeight = stoi(heightString);
        int sizeOfData = mapHeight * mapWidth;
        int *listOfData = new int[sizeOfData];
        for (int i(0); i < dataString.length(); ++i){
            if (dataString[i] == 'X'){
            listOfData[i] = 1;
//            cout << "X";
            }
            else if (dataString[i] == '-'){
            listOfData[i] = 0;
//            cout << "-";
            }
        }
        
        reportStyle = reportStyleSelection();

        runSimulations(mapHeight, mapWidth, mapFromInput(mapHeight, mapWidth, reportStyle, listOfData), gameMode, reportStyle);
    }
    
}

