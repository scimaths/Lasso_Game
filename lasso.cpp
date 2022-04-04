#include <simplecpp>
#include <string>
#include <cstdlib>
#include "MovingObject.h"
#include "lasso.h"
#include "coin.h"
#include "menu.h"
#include "number.h"

using namespace simplecpp;

void Lasso::draw_lasso_band() {
    double len = (release_speed/MAX_RELEASE_SPEED)*LASSO_BAND_LENGTH;
    double arad = release_angle_deg*PI/180.0;
    double xlen = len*cos(arad);
    double ylen = len*sin(arad);
    lasso_band.reset(lasso_start_x, lasso_start_y, (lasso_start_x-xlen), (lasso_start_y+ylen));
    lasso_band.setThickness(LASSO_THICKNESS);
} // End Lasso::draw_lasso_band()

void Lasso::initLasso() {
    lasso_start_x = (PLAY_X_START+LASSO_X_OFFSET);
    lasso_start_y = (PLAY_Y_HEIGHT-LASSO_Y_HEIGHT);
    lasso_circle.reset(lasso_start_x, lasso_start_y, LASSO_SIZE);
    lasso_circle.setColor(COLOR("red"));
    lasso_circle.setFill(true);
    lasso_loop.reset(lasso_start_x, lasso_start_y, LASSO_SIZE/2);
    lasso_loop.setColor(COLOR("green"));
    lasso_loop.setFill(true);
    addPart(&lasso_circle);
    addPart(&lasso_loop);
    lasso_looped = false;
    the_coin = NULL;
    num_coins = 0;
    isYankedForNumber = 0;
    num_nums = 0;
    foundNumInYank = 0;
    correctCaught = 0;
    multinums = 0;
    numAttached = 0;
    lasso_line.reset(lasso_start_x, lasso_start_y, lasso_start_x, lasso_start_y);
    lasso_line.setColor(COLOR("black"));
    lasso_band.setColor(COLOR("black"));
    draw_lasso_band();
} // End Lasso::initLasso()

void Lasso::yank (bool coinNeeded, bool &neededCaught) {
    bool paused = true, rtheta = true;
    reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
    lasso_loop.reset(lasso_start_x, lasso_start_y, LASSO_SIZE/2);
    lasso_loop.setFill(true);
    lasso_looped = false;
    if(the_coin != NULL && coinAttached == 1) {
        num_coins++;
        the_coin->resetCoin();
        coinAttached = 0;
        if (coinNeeded == 1) {neededCaught = 1;}
    }
} // End Lasso::yank()

void Lasso::yankIfEnd (bool &neededCaught) {
    bool paused = true, rtheta = true;
    reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
    lasso_loop.reset(lasso_start_x, lasso_start_y, LASSO_SIZE/2);
    lasso_loop.setFill(true);
    lasso_looped = false;
    if(the_coin != NULL && coinAttached == 1) {
        num_coins++;
        if (the_coin->getNeeded() == 1) {
            neededCaught = 1;
        }
        the_coin->resetCoin();
        coinAttached = 0;
    }
} // End Lasso::yankIfEnd()

// YankIfEndNum works when lasso crosses x-axis and checks if only the correct number has been caught (carried in neededCaught).
// decrlife is set to 1 if it was necessary and wrong/no numbers were caught.
void Lasso::yankIfEndNum (Number *ArrayNumbers[], bool &decrlife, bool &neededCaught) {
    bool paused = true, rtheta = true;
    reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
    lasso_loop.reset(lasso_start_x, lasso_start_y, LASSO_SIZE/2);
    lasso_loop.setFill(true);
    lasso_looped = false;
    if(numAttached > 0) {
        if (numAttached==1 && correctCaught==1) {num_nums++;}
        else {
            multinums++;
            if (ArrayNumbers[0]->getNeeded() == 1) {decrlife = 1;}
        }
        for (int cnter=0; cnter<numberOfNumbersTaken; ++cnter) {
            ArrayNumbers[cnter]->resetNum();
        }
        if (ArrayNumbers[0]->getNeeded() == 1 && correctCaught==1 && numAttached==1) {neededCaught = 1;}
        numAttached = 0;
        correctCaught = 0;
    }
} // End Lasso::yankIfEnd()

void Lasso::loopit() {
    if(lasso_looped) { return; } // Already looped
    lasso_loop.reset(getXPos(), getYPos(), (1.0 - (laslevel-1.0)/6)*LASSO_RADIUS);
    lasso_loop.setFill(false);
    lasso_looped = true;
} // End Lasso::loopit()

void Lasso::addAngle(double angle_deg) {
    release_angle_deg += angle_deg;
    if(release_angle_deg < MIN_RELEASE_ANGLE_DEG) { release_angle_deg = MIN_RELEASE_ANGLE_DEG; }
    if(release_angle_deg > MAX_RELEASE_ANGLE_DEG) { release_angle_deg = MAX_RELEASE_ANGLE_DEG; }
    bool paused = true, rtheta = true;
    reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
} // End Lasso::addAngle()

void Lasso::addSpeed(double speed) {
    release_speed += speed;
    if(release_speed < MIN_RELEASE_SPEED) { release_speed = MIN_RELEASE_SPEED; }
    if(release_speed > MAX_RELEASE_SPEED) { release_speed = MAX_RELEASE_SPEED; }
    bool paused = true, rtheta = true;
    reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
} // End Lasso::addSpeed()

void Lasso::nextStep(double stepTime, bool &neededCaught) {
    draw_lasso_band();
    MovingObject::nextStep(stepTime);
    if(getYPos() > PLAY_Y_HEIGHT) { yankIfEnd(neededCaught); }
    lasso_line.reset(lasso_start_x, lasso_start_y, getXPos(), getYPos());
} // End Lasso::nextStep()

// Has the yankIfEndNum to ensure resetting if lasso crossed x-axis.
void Lasso::nextStepNum(double stepTime, Number* ArrayNumber[], bool &decrlife, bool &neededCaught) {
    draw_lasso_band();
    MovingObject::nextStep(stepTime);
    if(getYPos() > PLAY_Y_HEIGHT) { yankIfEndNum(ArrayNumber, decrlife, neededCaught); }
    lasso_line.reset(lasso_start_x, lasso_start_y, getXPos(), getYPos());
} // End Lasso::nextStep()

void Lasso::check_for_coin(Coin *coinPtr) {
    double lasso_x = getXPos();
    double lasso_y = getYPos();
    double coin_x = coinPtr->getXPos();
    double coin_y = coinPtr->getYPos();
    double xdiff = (lasso_x - coin_x);
    double ydiff = (lasso_y - coin_y);
    double distance = sqrt((xdiff*xdiff)+(ydiff*ydiff));
    if(distance <= LASSO_RADIUS*(1.0-(laslevel-1.0)/6.0)) {
        the_coin = coinPtr;
        the_coin->getAttachedTo(this);
        coinAttached = 1;
    }
} // End Lasso::check_for_coin()

// Yanks lasso, resets numbers if any number was caught inside.
void Lasso::yankNumber (Number *ArrayNumbers[], bool numNeeded, bool &neededCaught) {
    bool paused = true, rtheta = true;
    reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
    lasso_loop.reset(lasso_start_x, lasso_start_y, LASSO_SIZE/2);
    lasso_loop.setFill(true);
    lasso_looped = false;
    isYankedForNumber = 1;
    if(numAttached > 0) {
        foundNumInYank = 1;
        if (numAttached == 1 && correctCaught == 1) {num_nums++;}
        else {multinums++;}
        for (int cnter=0; cnter<numberOfNumbersTaken; ++cnter) {
            ArrayNumbers[cnter]->resetNum();
        }
        if (numNeeded == 1 && correctCaught==1 && numAttached==1) {neededCaught = 1;}
        numAttached = 0;
        correctCaught = 0;
    }
} // End Lasso::yank()

// Attaches numbers in range to lasso when looped, updates the very crucial variable numAttached.
void Lasso::check_for_num (Number *ArrayNumbers[], int totalNumbers) {
    double lasso_x = getXPos();
    double lasso_y = getYPos();
    int countingNumsinRange = 0;
    for (int cnter = 0; cnter < (int)totalNumbers; ++cnter) {
        double num_x = ArrayNumbers[cnter]->getXPos();
        double num_y = ArrayNumbers[cnter]->getYPos();
        double xdiff = (lasso_x - num_x);
        double ydiff = (lasso_y - num_y);
        double distance = sqrt((xdiff*xdiff)+(ydiff*ydiff));
        if(distance <= LASSO_RADIUS*(1.0-(laslevel-1.0)/6.0)) {
            switch (countingNumsinRange) {
                case 0: the_num1 = ArrayNumbers[cnter]; break;
                case 1: the_num2 = ArrayNumbers[cnter]; break;
                case 2: the_num3 = ArrayNumbers[cnter]; break;
                case 3: the_num4 = ArrayNumbers[cnter]; break;
            }
            countingNumsinRange++;
            ArrayNumbers[cnter]->getAttachedTo(this);
            if (ArrayNumbers[cnter]->getCorrect() == 1) {correctCaught=1;}
        }
    }
    numAttached = countingNumsinRange;
} // End Lasso::check_for_num()

// Sets the_num pointers to NULL
void Lasso::setNull() {
    the_num1 = NULL;
    the_num2 = NULL;
    the_num3 = NULL;
    the_num4 = NULL;
    numAttached = 0;
}
