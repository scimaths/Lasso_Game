#ifndef __INTRODUCE_MODES__
#define __INTRODUCE_MODES__

#include <bits/stdc++.h>
#include <simplecpp>
#include <fstream> // For reading from and writing to files.
#include <cstdlib>
#include <array>
#include <cstdio>
#include <string>
#include "MovingObject.h"
#include "lasso.h"
#include "coin.h"
#include "menu.h"

void coinToss(bool &newScreen, float &stepTime, float &runTime, float &currTime, int &stepCount, map<char,char> mapOfChar);
void mathToss(bool &newScreen, float &stepTime, float &runTime, float &currTime, int &stepCount, map<char,char> mapOfChar);

#endif // __INTRODUCE_MODES__
