#ifndef __LASSO_H__
#define __LASSO_H__

#include "MovingObject.h"
#include "number.h"
#include "coin.h"
#include "menu.h"
#include <cstdlib>

//#define WINDOW_X 1200
//#define WINDOW_Y 960
#define WINDOW_X 800
#define WINDOW_Y 600
#define clickratio 65536
#define STEP_TIME 0.05
#define LIVELENGTH 20

#define PLAY_X_START 100
#define PLAY_Y_START 0
#define PLAY_X_WIDTH (WINDOW_X-PLAY_X_START)
#define PLAY_Y_HEIGHT (WINDOW_Y-100)
#define INSTR_Y_START 530

#define LASSO_X_OFFSET 100
#define LASSO_Y_HEIGHT 100
#define LASSO_BAND_LENGTH LASSO_X_OFFSET
#define LASSO_THICKNESS 5

#define COIN_GAP 1

#define RELEASE_ANGLE_STEP_DEG 5
#define MIN_RELEASE_ANGLE_DEG 0
#define MAX_RELEASE_ANGLE_DEG (360-RELEASE_ANGLE_STEP_DEG)
#define INIT_RELEASE_ANGLE_DEG 45

#define RELEASE_SPEED_STEP 20
#define MIN_RELEASE_SPEED 0
#define MAX_RELEASE_SPEED 200
#define INIT_RELEASE_SPEED 100

#define COIN_SPEED 120
#define COIN_ANGLE_DEG 90

#define LASSO_G 30
#define COIN_G 30

#define LASSO_SIZE 10
#define LASSO_RADIUS 50.0
#define COIN_SIZE 5

class Lasso : public MovingObject {
    double lasso_start_x;
    double lasso_start_y;
    double release_speed;
    double release_angle_deg;
    double lasso_ax;
    double lasso_ay;
    int laslevel;
    // Moving parts
    Circle lasso_circle;
    Circle lasso_loop;

    // Non-moving parts
    Line lasso_line;
    Line lasso_band;

    // State info
    bool lasso_looped;
    Coin *the_coin;
    Number *the_num1;
    Number *the_num2;
    Number *the_num3;
    Number *the_num4;
    int num_coins;
    int num_nums;
    int multinums;
    bool correctCaught;
    bool coinAttached;
    bool isYankedForNumber;
    bool foundNumInYank;
    int numAttached;
    void setLevel(int level);
    void initLasso();

 public:
 Lasso(double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta, int level) : MovingObject(speed, angle_deg, argax, argay, argpaused, rtheta) {
    release_speed = speed;
    release_angle_deg = angle_deg;
    lasso_ax = argax;
    lasso_ay = argay;
    laslevel = level;
    initLasso();
    }

    void draw_lasso_band();
    void yank(bool coinNeeded, bool &neededCaught);
    void yankIfEnd(bool &neededCaught);
    void yankIfEndNum(Number *ArrayNumbers[], bool &decrlife, bool &neededCaught);
    void loopit();
    void setNull();
    void addAngle(double angle_deg);
    void addSpeed(double speed);
    void nextStep(double t, bool &neededCaught);
    void nextStepNum(double t, Number* ArrayNumber[], bool &decrlife, bool &neededCaught);
    void check_for_coin(Coin *coin);
    void setNumAttached (int setToThis) {numAttached = setToThis;}
    void setCorrectCaught (int setToThis) {correctCaught = setToThis;}
    int getNumCoins() { return num_coins; }
    int getNums() { return num_nums; }
    bool getCorrectCaught() {return correctCaught;}
    int getmultiNums() {return multinums;}
    void check_for_num (Number *ArrayNumbers[], int totalNumbers);
    void yankNumber (Number *ArrayNumbers[], bool numNeeded, bool &neededCaught);
    bool getisYankedForNumber() {return isYankedForNumber;}
    void setisYankedForNumber (bool setToThis) {isYankedForNumber = setToThis;}
    bool getFoundNumInYank () {return foundNumInYank;}
    void setFoundNumInYank (bool setToThis) {foundNumInYank = setToThis;}
}; // End class Lasso

#endif
