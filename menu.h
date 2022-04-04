#ifndef __MENU__
#define __MENU__
#include <bits/stdc++.h>

int inRect (int x, int y, Rectangle Ra, Rectangle Rb, Rectangle Rc, Rectangle Rd, Rectangle Re);
void backButton (int xPos, int yPos);
void viewHelp ();
bool showMenu ( bool started , map<char,char> &mapOfChar);
int isLevel ();
int selectLevel();
int modeChoice();
int modeChoiceHS();
bool overScreen (int score);
#endif
