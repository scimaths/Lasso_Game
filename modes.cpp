#include <simplecpp>
#include <fstream> // For reading from and writing to files.
#include <cstdlib>
#include <array>
#include <cstdio>
#include <string>
#include <stdlib.h>
#include <string>
#include <time.h>
#include "MovingObject.h"
#include "lasso.h"
#include "coin.h"
#include "menu.h"
#include "hs.h"
#include "modes.h"
#include "number.h"
#include "equation.h"

void coinToss(bool &newScreen, float &stepTime, float &runTime, float &currTime, int &stepCount, map<char,char> mapOfChar) {

    int level = selectLevel();

    map<char, char> mapOfCharInv;
    for (auto it: mapOfChar) {mapOfCharInv[it.second] = it.first;}
    string dummy = "";
    Text Instr (PLAY_X_START + 300, INSTR_Y_START, "Commands");
    string loopInstr = dummy + mapOfCharInv['l'] + "- Loop lasso";
    string yankInstr = dummy + mapOfCharInv['y'] + "- Yank lasso";
    string quitInstr = dummy + mapOfCharInv['q'] + "- Save and Exit";
    string launchInstr = dummy + mapOfCharInv['t'] + "- Launch lasso";
    Text instrLaunching (PLAY_X_START + 240, INSTR_Y_START + textHeight()+5, launchInstr);
    Text instrLooping (PLAY_X_START + 360, INSTR_Y_START + textHeight()+5, loopInstr);
    Text instrYanking (PLAY_X_START + 240, INSTR_Y_START + 2*(textHeight()+5), yankInstr);
    Text instrQuitting (PLAY_X_START + 360, INSTR_Y_START + 2*(textHeight()+5), quitInstr);

    // Draw lasso at start position
    double release_speed = INIT_RELEASE_SPEED; // m/s - 100
    double release_angle_deg = INIT_RELEASE_ANGLE_DEG; // degrees - 45
    double lasso_ax = 0;
    double lasso_ay = LASSO_G; // 30
    bool paused = true;
    bool rtheta = true;
    Lasso lasso(release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta, level);

    // Construction of axes
    Line b1(0, PLAY_Y_HEIGHT, WINDOW_X, PLAY_Y_HEIGHT);
    b1.setColor(COLOR("blue"));
    Line b2(PLAY_X_START, 0, PLAY_X_START, WINDOW_Y);
    b2.setColor(COLOR("blue"));

    // Point count
    string msg("Cmd: _");
    Text charPressed(PLAY_X_START+50, PLAY_Y_HEIGHT+20, msg);
    char coinScoreStr[256];
    sprintf(coinScoreStr, "Coins: %d", lasso.getNumCoins());
    Text coinScore(PLAY_X_START+50, PLAY_Y_HEIGHT+50, coinScoreStr);

    // Drawing 3 lives (filled squares for now) at top
    Rectangle Live1 (6.5*WINDOW_X/8, WINDOW_Y/12, LIVELENGTH, LIVELENGTH);
    Rectangle Live2 (6.5*WINDOW_X/8 + 2*LIVELENGTH, WINDOW_Y/12, LIVELENGTH, LIVELENGTH);
    Rectangle Live3 (6.5*WINDOW_X/8 + 4*LIVELENGTH, WINDOW_Y/12, LIVELENGTH, LIVELENGTH);
    Live1.setFill(true); Live2.setFill(true); Live3.setFill(true);
    Live1.setColor(COLOR("red")); Live2.setColor(COLOR("blue")); Live3.setColor(COLOR("green"));
    string hlth = "Health: ";
    Text Health (6.5*WINDOW_X/8-1.5*textWidth(hlth), WINDOW_Y/12, hlth);

    // Draw coin at start position
    bool necessary = 0;
    paused = true; rtheta = true;
    double coin_speed = COIN_SPEED; // 120
    double coin_angle_deg = COIN_ANGLE_DEG; // 90
    double coin_ax = 0;
    double coin_ay = COIN_G; // 30
    Coin coin(coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, rtheta, level, necessary);

    int lives = 3;
    int coinsLaunched = 0, coinsMissed = 0;

    // After every COIN_GAP sec, make the coin jump
    double last_coin_jump_end = 0;

    // When t is pressed, throw lasso
    // If lasso within range, make coin stick
    // When y is pressed, yank lasso
    // When l is pressed, loop lasso
    // When q is pressed, quit

    bool necessary_launched = 0; bool colorset = 0; bool neededCaught = 0;

    for(;;) {
        if((runTime > 0) && (currTime > runTime)) { break; }
        coinsLaunched = coinsMissed + lasso.getNumCoins();
        if (coinsLaunched%3==0 && coinsLaunched > 0 && colorset == 0) {
            coin.setClr(COLOR("red"));
            coin.setNeeded(1);
            colorset = 1;
            necessary_launched = 1;
            neededCaught = 0;
        }
        else if (colorset == 1 && necessary_launched == 0) {
            coin.setClr(COLOR("blue"));
            coin.setNeeded(0);
            colorset = 0;
        }
        XEvent e;
        bool pendingEv = checkEvent(e);
        if(pendingEv) {
            char c = charFromEvent(e);
            msg[msg.length()-1] = c;
            charPressed.setMessage(msg);
            c=mapOfChar[c];
            switch(c) {
                case 't':
                lasso.unpause();
                break;
                case 'y':
                lasso.yank(coin.getNeeded(), neededCaught);
                break;
                case 'l':
                lasso.loopit();
                lasso.check_for_coin(&coin);
                wait(STEP_TIME*5);
                break;
                case '[':
                if(lasso.isPaused()) { lasso.addAngle(-RELEASE_ANGLE_STEP_DEG);	}
                break;
                case ']':
                if(lasso.isPaused()) { lasso.addAngle(+RELEASE_ANGLE_STEP_DEG); }
                break;
                case '-':
                if(lasso.isPaused()) { lasso.addSpeed(-RELEASE_SPEED_STEP); }
                break;
                case '=':
                if(lasso.isPaused()) { lasso.addSpeed(+RELEASE_SPEED_STEP); }
                break;
                case 'q':
                overScreen(lasso.getNumCoins()); HSUpdate(0, lasso.getNumCoins()); newScreen = 1; break;
                default:
                break;
          }
        }

        if (neededCaught == 1) {
            necessary_launched = 0;
            coin.setNeeded(0);
        }

        lasso.nextStep(stepTime, neededCaught);

        coin.nextStep(stepTime);
        if(coin.isPaused()) {
            if((currTime-last_coin_jump_end) >= COIN_GAP) {
                coin.unpause();
            }
        }

        if(coin.getYPos() > PLAY_Y_HEIGHT) {
            coin.resetCoin();
            coinsMissed++;
            if (colorset == 1 && neededCaught == 0) {
                switch (lives) {
                    case 3: Live3.setFill(false); break;
                    case 2: Live2.setFill(false); break;
                    case 1: Live1.setFill(false); overScreen(lasso.getNumCoins()); HSUpdate(0, lasso.getNumCoins()); newScreen = 1; break;
                }
                lives--;
                necessary_launched = 0;
                coin.setNeeded(0);
            }
            last_coin_jump_end = currTime;
        }
        if (newScreen == 1) {break;}
        sprintf(coinScoreStr, "Coins: %d", lasso.getNumCoins());
        coinScore.setMessage(coinScoreStr);

        stepCount++;
        currTime += stepTime;
        wait(stepTime);
    } // End for(;;)
}

void mathToss(bool &newScreen, float &stepTime, float &runTime, float &currTime, int &stepCount, map<char,char> mapOfChar) {

    int level = selectLevel();

    map<char, char> mapOfCharInv;
    for (auto it: mapOfChar) {mapOfCharInv[it.second] = it.first;}
    string dummy = "";
    Text Instr (PLAY_X_START + 300, INSTR_Y_START, "Commands");
    string loopInstr = dummy + mapOfCharInv['l'] + "- Loop lasso";
    string yankInstr = dummy + mapOfCharInv['y'] + "- Yank lasso";
    string quitInstr = dummy + mapOfCharInv['q'] + "- Save and Exit";
    string launchInstr = dummy + mapOfCharInv['t'] + "- Launch lasso";
    Text instrLaunching (PLAY_X_START + 240, INSTR_Y_START + textHeight()+5, launchInstr);
    Text instrLooping (PLAY_X_START + 360, INSTR_Y_START + textHeight()+5, loopInstr);
    Text instrYanking (PLAY_X_START + 240, INSTR_Y_START + 2*(textHeight()+5), yankInstr);
    Text instrQuitting (PLAY_X_START + 360, INSTR_Y_START + 2*(textHeight()+5), quitInstr);

    // Draw lasso at start position
    double release_speed = INIT_RELEASE_SPEED; // m/s - 100
    double release_angle_deg = INIT_RELEASE_ANGLE_DEG; // degrees - 45
    double lasso_ax = 0;
    double lasso_ay = LASSO_G; // 30
    bool paused = true;
    bool rtheta = true;
    Lasso lasso(release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta, level);

    // Construction of axes
    Line b1(0, PLAY_Y_HEIGHT, WINDOW_X, PLAY_Y_HEIGHT);
    b1.setColor(COLOR("blue"));
    Line b2(PLAY_X_START, 0, PLAY_X_START, WINDOW_Y);
    b2.setColor(COLOR("blue"));

    // Point count
    string msg("Cmd: _");
    Text charPressed(PLAY_X_START+50, PLAY_Y_HEIGHT+20, msg);
    char numScoreStr[256];
    sprintf(numScoreStr, "Score: %d", lasso.getNums());
    Text numScore(PLAY_X_START+50, PLAY_Y_HEIGHT+50, numScoreStr);

    // Drawing 3 lives (filled squares for now) at top
    Rectangle Live1 (6.5*WINDOW_X/8, WINDOW_Y/12, LIVELENGTH, LIVELENGTH);
    Rectangle Live2 (6.5*WINDOW_X/8 + 2*LIVELENGTH, WINDOW_Y/12, LIVELENGTH, LIVELENGTH);
    Rectangle Live3 (6.5*WINDOW_X/8 + 4*LIVELENGTH, WINDOW_Y/12, LIVELENGTH, LIVELENGTH);
    Live1.setFill(true); Live2.setFill(true); Live3.setFill(true);
    Live1.setColor(COLOR("red")); Live2.setColor(COLOR("blue")); Live3.setColor(COLOR("green"));
    string hlth = "Health: ";
    Text Health (6.5*WINDOW_X/8-1.5*textWidth(hlth), WINDOW_Y/12, hlth);
    string equationShown = "";
    Text equationAtTop (WINDOW_X/2-textWidth("39x^2+40x=2000"), WINDOW_Y/12, "Equation:");

    // Draw numbers at start position
    bool necessary = 0;
    paused = true; rtheta = true;
    double num_speed = COIN_SPEED; // 120
    double num_angle_deg = COIN_ANGLE_DEG; // 90
    double num_ax = 0;
    double num_ay = COIN_G; // 30
    Number First (num_speed, num_angle_deg, num_ax, num_ay, paused, rtheta, level, necessary, 1);
    Number Second (num_speed, num_angle_deg, num_ax, num_ay, paused, rtheta, level, necessary, 2);
    Number Third (num_speed, num_angle_deg, num_ax, num_ay, paused, rtheta, level, necessary, 3);
    Number Fourth (num_speed, num_angle_deg, num_ax, num_ay, paused, rtheta, level, necessary, 4);

    // Storing the addresses of Numbers so that properties can be later changed.
    Number *ArrayNumbers[4] = {&First, &Second, &Third, &Fourth};
    int lives = 3;
    int numsLaunched = 0, numsMissed = 0;

    // After every COIN_GAP sec, make the number jump
    double last_num_jump_end = 0;

    // When t is pressed, throw lasso
    // If lasso within range, make number stick
    // When y is pressed, yank lasso
    // When l is pressed, loop lasso
    // When q is pressed, quit

    // To implement the "necessary" Number functionality.
    bool necessary_launched = 0; bool colorset = 0; bool neededCaught = 0; bool decrlife = 0;

    // For random number generation.
    std::srand(time(NULL));

    // An array to contain the four randomly generated options.
    int arrOptions[4]={0,0,0,0};
    int correctAnswer;

    // The dynamics of the game
    for(;;) {
        if((runTime > 0) && (currTime > runTime)) { break; }

        // Re-initializing properties of Lasso's yank in each cycle so that program doesn't take lasso as yanked in next cycle.
        lasso.setisYankedForNumber(false);
        lasso.setFoundNumInYank(false);

        // numsLaunched has not yet been updated. However numsMissed, getNums, getMultiNums have been.
        // So there will be a mismatch when we are about to launch next number.
        // Whenever this happens, generate four random *unequal* numbers into the array, choose one correct answer.
        // Send the corresponding answer to the equations function, update all texts accordingly.
        if (numsMissed+lasso.getNums()+lasso.getmultiNums()>numsLaunched) {
            lasso.setNumAttached(0);
            lasso.setCorrectCaught(0);
            arrOptions[0] = rand()%20+1;
            arrOptions[1] = rand()%20+1; while (arrOptions[1]==arrOptions[0]) {arrOptions[1] = rand()%20+1;}
            arrOptions[2] = rand()%20+1; while (arrOptions[2]==arrOptions[0]||
                                                arrOptions[2]==arrOptions[1]) {arrOptions[2] = rand()%20+1;}
            arrOptions[3] = rand()%20+1; while (arrOptions[3]==arrOptions[0]||
                                                arrOptions[3]==arrOptions[1]||
                                                arrOptions[3]==arrOptions[2]) {arrOptions[3] = rand()%20+1;}
            correctAnswer = rand()%4;
            if ((numsLaunched+1)%3 ==0 && numsLaunched+1 > 0 && colorset == 0) {
                equationAtTop.reset(WINDOW_X/2-textWidth("39x^2+40x=2000"), WINDOW_Y/12, "Equation: " + createQuadraticEquation(arrOptions[correctAnswer]));
            }
            else {
                equationAtTop.reset(WINDOW_X/2-textWidth("39x^2+40x=2000"), WINDOW_Y/12, "Equation: " + createLinearEquation(arrOptions[correctAnswer]));
            }
            First.setMsg(arrOptions[0]);
            Second.setMsg(arrOptions[1]);
            Third.setMsg(arrOptions[2]);
            Fourth.setMsg(arrOptions[3]);
            ArrayNumbers[correctAnswer]->setCorrect(1);
        }

        // Updating numsLaunched if its time for a new number, only after generating equation and answer.
        numsLaunched = numsMissed + lasso.getNums() + lasso.getmultiNums();

        // Necessary numbers that must be caught or else, there would be a deduction.
        // Logic for necessary -
        /*
            Whenever fixed interval is seen i.e. 3 here, number becomes necessary, color becomes red.
            When caught, neededCaught will become 1.
            Later on necessary_launched will be made 0 when working for this cycle is over.
            In next cycle of for, color is still set, it will be made blue again.
        */

        if (numsLaunched%3 == 0 && numsLaunched > 0 && colorset == 0) {
            // Setting color and property when necessity.
            for (int numcnter = 0; numcnter < numberOfNumbersTaken; ++numcnter ) {
                ArrayNumbers[numcnter]->setClr(COLOR("red"));
                ArrayNumbers[numcnter]->setNeeded(1);
            }
            colorset = 1;
            necessary_launched = 1;
            neededCaught = 0;
        }
        else if (colorset == 1 && necessary_launched == 0) {
            for (int numcnter = 0; numcnter < numberOfNumbersTaken; ++numcnter ) {
                ArrayNumbers[numcnter]->setClr(COLOR("blue"));
                ArrayNumbers[numcnter]->setNeeded(0);
            }
            colorset = 0;
        }

        XEvent e;
        bool pendingEv = checkEvent(e);
        if(pendingEv) {
            char c = charFromEvent(e);
            msg[msg.length()-1] = c;
            charPressed.setMessage(msg);
            c = mapOfChar[c];
            switch(c) {

                // Makes the lasso move.
                case 't':
                lasso.unpause();
                break;

                // yank has necessary arguments to know if number has been caught or not.
                case 'y':
                lasso.yankNumber(ArrayNumbers, (*ArrayNumbers[0]).getNeeded(), neededCaught);
                break;

                // check_for_num sets numAttached in lasso to count within range of lasso.
                case 'l':
                lasso.loopit();
                lasso.check_for_num(ArrayNumbers, numberOfNumbersTaken);
                wait(STEP_TIME*5);
                break;

                // For speed and angle change.
                case '[':
                if(lasso.isPaused()) { lasso.addAngle(-RELEASE_ANGLE_STEP_DEG);	}
                break;
                case ']':
                if(lasso.isPaused()) { lasso.addAngle(+RELEASE_ANGLE_STEP_DEG); }
                break;
                case '-':
                if(lasso.isPaused()) { lasso.addSpeed(-RELEASE_SPEED_STEP); }
                break;
                case '=':
                if(lasso.isPaused()) { lasso.addSpeed(+RELEASE_SPEED_STEP); }
                break;

                // For quitting the game.
                case 'q':
                overScreen(lasso.getNums()); newScreen = 1; HSUpdate(1, lasso.getNums()); newScreen = 1; break;
                default:
                break;
          }
        }

        // Lasso moves one step. It also checks if lasso crosses x-axis.
        // If crossed, it checks if only the correct number has been caught or something else.
        // This comes back here using the decrlife boolean.
        lasso.nextStepNum(stepTime, ArrayNumbers, decrlife, neededCaught);

        // If needed number has been caught when it was necessary, set needed parameter back to normal.
        if (neededCaught == 1) {
            necessary_launched = 0;
            for (int numcnter = 0; numcnter < numberOfNumbersTaken; ++numcnter ) {
                ArrayNumbers[numcnter]->setNeeded(0);
            }
        }

        // Moving each coin
        for (int numcnter = 0; numcnter < numberOfNumbersTaken; ++numcnter ) {
            ArrayNumbers[numcnter]->nextStep(stepTime);
        }

        // To include some waiting time before next jump
        if(ArrayNumbers[0]->isPaused()) {
            if((currTime-last_num_jump_end) >= COIN_GAP) {
                for (int numcnter = 0; numcnter < numberOfNumbersTaken; ++numcnter ) {
                    ArrayNumbers[numcnter]->unpause();
                }
            }
        }

        //    /\    If lasso is yanked, num was found and color was set and needed was not caught
        //   /--\   OR if life has to be decreased due to lasso having wrong number and crossing x-axis
        //  /    \  Decrease life, set necessary launched as zero, Needed as 0. If decrlife was used, set it back to 0.
        if ((lasso.getisYankedForNumber() == 1 && lasso.getFoundNumInYank() == 1 && colorset == 1 && neededCaught == 0)||decrlife == 1) {
            switch (lives) {
                case 3: Live3.setFill(false); break;
                case 2: Live2.setFill(false); break;
                case 1: Live1.setFill(false); overScreen(lasso.getNums()); HSUpdate(1, lasso.getNums()); newScreen = 1; break;
            }
            lives--;
            necessary_launched = 0;
            for (int numcnter = 0; numcnter < numberOfNumbersTaken; ++numcnter ) {
                ArrayNumbers[numcnter]->setNeeded(0);
                ArrayNumbers[numcnter]->setClr(COLOR("blue"));
            }
            if (decrlife == 1) {decrlife = 0;}
        }

        // B. If coins have crossed x-axis, set all coin pointers in lasso to null, reset coins,
        if(ArrayNumbers[0]->getYPos() > PLAY_Y_HEIGHT ||
           ArrayNumbers[1]->getYPos() > PLAY_Y_HEIGHT ||
           ArrayNumbers[2]->getYPos() > PLAY_Y_HEIGHT ||
           ArrayNumbers[3]->getYPos() > PLAY_Y_HEIGHT) {
            lasso.setNull();
            for (int numcnter = 0; numcnter < numberOfNumbersTaken; ++numcnter ) {
                ArrayNumbers[numcnter]->resetNum();
            }
            numsMissed++;
            // If the number was necessary to catch and wasn't caught, decrease life.
            if (colorset == 1) {
                if (neededCaught == 0) {
                    switch (lives) {
                        case 3: Live3.setFill(false); break;
                        case 2: Live2.setFill(false); break;
                        case 1: Live1.setFill(false); overScreen(lasso.getNums()); HSUpdate(1, lasso.getNums()); newScreen = 1; newScreen = 1; break;
                    }
                    lives--;
                }
                necessary_launched = 0;
                for (int numcnter = 0; numcnter < numberOfNumbersTaken; ++numcnter ) {
                    ArrayNumbers[numcnter]->setNeeded(0);
                    ArrayNumbers[numcnter]->setClr(COLOR("blue"));
                }
                colorset = 0;
            }
            last_num_jump_end = currTime;
        }

        // Updates for the score and the next cycle.
        if (newScreen == 1) {break;}
        sprintf(numScoreStr, "Coins: %d", lasso.getNums());
        numScore.setMessage(numScoreStr);

        stepCount++;
        currTime += stepTime;
        wait(stepTime);
    } // End for(;;)
}
