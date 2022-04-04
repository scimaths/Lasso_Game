#ifndef __COIN_H__
#define __COIN_H__
#include <cstdlib>
#include "MovingObject.h"
#include "menu.h"

class Coin : public MovingObject {
    double coin_start_x;
    double coin_start_y;
    double release_speed;
    double release_angle_deg;
    double coin_ax;
    double coin_ay;
    bool needed;
    int coinlevel;
    // Moving parts
    Circle coin_circle;

 public:
 Coin(double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta, int level, bool necessary) : MovingObject(speed, angle_deg, argax, argay, argpaused, rtheta) {
    release_speed = speed;
    release_angle_deg = angle_deg;
    coin_ax = argax;
    coin_ay = argay;
    coinlevel = level;
    needed = necessary;
    initCoin();
    }
    void initCoin();
    void setClr (int s);
    void resetCoin();
    bool getNeeded();
    void setNeeded( bool toSetNeeded );
    Circle getCircle();
}; // End class Coin

#endif
