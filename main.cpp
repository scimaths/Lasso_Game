#include <simplecpp>
#include <fstream> // For reading from and writing to files.
#include <cstdlib>
#include <array>
#include <cstdio>
#include <string>
#include "MovingObject.h"
#include "number.h"
#include "lasso.h"
#include "coin.h"
#include "menu.h"
#include "modes.h"

using namespace simplecpp;

main_program {

    initCanvas("Lasso", WINDOW_X, WINDOW_Y); // 800, 600
    int stepCount = 0;
    float stepTime = STEP_TIME;
    float runTime = -1; // sec; -ve means infinite
    float currTime = 0;
    bool newScreen = 0;
    // START Window
    string wlcm = "Welcome to Lasso Fun!";
    Rectangle R1 (WINDOW_X/2,WINDOW_Y/2, textWidth(wlcm)+15, textHeight()+10);
    R1.setFill(true);
    R1.setColor(COLOR("yellow"));
    Text T1 (WINDOW_X/2, WINDOW_Y/2,wlcm);
    wait(3);
    T1.hide(); R1.hide();

    // To ensure changed chars work
    map<char,char> mapOfChar;
    string fileName = "command-change.txt";
    ifstream commandTest(fileName);
    if (!commandTest) {
        ofstream commandOut (fileName);
        commandOut << "l l\ny y\nt t\nq q\n[ [\n] ]\n= =\n- -\nEND";
        commandOut.close();
    }
    string input;
    ifstream commandChange (fileName);
    getline(commandChange, input);
    while (input != "END") {
        mapOfChar[input[0]] = input[2];
        getline(commandChange, input);
    }

    // While not exiting the program, continue in the loop.
    while (true) {
        bool newScreen = 0;
        bool started = 0;
        if (!showMenu(started, mapOfChar)) {return 0;}
        int which_mode = modeChoice();
        switch (which_mode) {
            case 1: coinToss(newScreen, stepTime, runTime, currTime, stepCount, mapOfChar); runTime = -1; currTime = 0; stepTime = STEP_TIME; break;
            case 0: mathToss(newScreen, stepTime, runTime, currTime, stepCount, mapOfChar); runTime = -1; currTime = 0; stepTime = STEP_TIME; break;
        }
    }
} // End main_program
