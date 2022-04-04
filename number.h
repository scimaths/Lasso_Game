#ifndef __NUMBER_INCLUDEDH__
#define __NUMBER_INCLUDEDH__
#include <cstdlib>
#include "MovingObject.h"
#include "menu.h"

#define PLAY_X_START 100
#define WINDOW_X 1200
#define WINDOW_Y 800
#define PLAY_Y_START 0
#define PLAY_X_WIDTH (WINDOW_X-PLAY_X_START)
#define PLAY_Y_HEIGHT (WINDOW_Y-100)
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


class Number : public MovingObject {
    double num_start_x;
    double num_start_y;
    double release_speed;
    double release_angle_deg;
    double num_ax;
    double num_ay;
    int numlevel;
    int index;
    bool needed;
    bool correct;
    // Moving parts
    Rectangle num_rect;
    Text stringNumber;

 public:
 Number(double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta, int level, bool necessary, int indexinit) : MovingObject(speed, angle_deg, argax, argay, argpaused, rtheta) {
    release_speed = speed;
    release_angle_deg = angle_deg;
    index = indexinit;
    num_ax = argax;
    num_ay = argay;
    numlevel = level;
    needed = necessary;
    correct=0;
    initNum();
    }
    void initNum();
    void setClr (int s);
    void resetNum();
    bool getNeeded();
    bool getCorrect();
    void setCorrect(bool b);
    void setNeeded( bool toSetNeeded );
    Rectangle getRect();
    Text getText();
    void setMsg (int setto_string);
}; // End class Coin

#endif
