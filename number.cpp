#include <simplecpp>
#include "number.h"
#include <cstdlib>
#include "menu.h"

void Number::initNum() {
//    num_start_x = PLAY_X_START+index*(WINDOW_X-PLAY_X_START)/(numberOfNumbersTaken+2); // Using 4 as total number of coins, hence 6.
    num_start_x = PLAY_X_START*1.5+index*(WINDOW_X-PLAY_X_START)/(2*numberOfNumbersTaken);
    num_start_y = 500;
    num_rect.reset(num_start_x, num_start_y, 20, 20);
    num_rect.setColor(COLOR("blue"));
    num_rect.setFill(true);
    stringNumber.reset(num_start_x, num_start_y, "00");
    addPart(&num_rect);
    addPart(&stringNumber);
}

void Number::resetNum() {
    double num_speed = COIN_SPEED;
    double num_angle_deg = COIN_ANGLE_DEG;
    num_ax = 3*(rand()%5-2)*numlevel*0.25;
    num_ay = COIN_G;
    correct=0;
    bool paused = true, rtheta = true;
    reset_all(num_start_x, num_start_y, num_speed, num_angle_deg, num_ax, num_ay, paused, rtheta);
}

Rectangle Number::getRect() {return num_rect;}
Text Number::getText() {return stringNumber;}
void Number::setMsg (int setto_string) {string argString; stringstream ss; ss << setto_string; ss >> argString; stringNumber.setMessage(argString);}

void Number::setClr (int s) {num_rect.setColor(s);}
bool Number::getCorrect() {return correct;}
void Number::setCorrect(bool b) {correct=b;}
bool Number::getNeeded () { return needed; }
void Number::setNeeded (bool toSetNeeded) { needed = toSetNeeded; }

