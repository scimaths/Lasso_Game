#include <bits/stdc++.h>
#include <simplecpp>
#include "menu.h"
#include <fstream> // For reading from and writing to files.
#include <cstdlib>
#include <cstdio>
#include <string>
#include "hs.h"
#include "MovingObject.h"
#include "lasso.h"
#include "coin.h"
#include "menu.h"
#define HIDE; {T2.hide(); R2.hide(); T3.hide(); R3.hide(); T4.hide(); R4.hide(); T5.hide(); R5.hide(); T6.hide(); R6.hide();}

using namespace std;

int inRect (int x, int y, Rectangle Ra, Rectangle Rb, Rectangle Rc, Rectangle Rd, Rectangle Re) {
    if (x<=Ra.getX()+Ra.getWidth()/2 &&
        x>=Ra.getX()-Ra.getWidth()/2 &&
        y<=Ra.getY()+Ra.getHeight()/2 &&
        y>=Ra.getY()-Ra.getHeight()/2) {return 1;}
    else if (x<=Rb.getX()+Rb.getWidth()/2 &&
        x>=Rb.getX()-Rb.getWidth()/2 &&
        y<=Rb.getY()+Rb.getHeight()/2 &&
        y>=Rb.getY()-Rb.getHeight()/2) {return 2;}
    else if (x<=Rc.getX()+Rc.getWidth()/2 &&
        x>=Rc.getX()-Rc.getWidth()/2 &&
        y<=Rc.getY()+Rc.getHeight()/2 &&
        y>=Rc.getY()-Rc.getHeight()/2) {return 3;}
    else if (x<=Rd.getX()+Rd.getWidth()/2 &&
        x>=Rd.getX()-Rd.getWidth()/2 &&
        y<=Rd.getY()+Rd.getHeight()/2 &&
        y>=Rd.getY()-Rd.getHeight()/2) {return 4;}
    else if (x<=Re.getX()+Re.getWidth()/2 &&
        x>=Re.getX()-Re.getWidth()/2 &&
        y<=Re.getY()+Re.getHeight()/2 &&
        y>=Re.getY()-Re.getHeight()/2) {return 5;}
    else {return 0;}
}

void backButton (int xPos, int yPos) {
    string bck = "Back to Main Menu";
    Text bk (xPos, yPos, bck);
    Rectangle R (xPos, yPos, textWidth(bck)+20, textHeight()+10);
    int x=0, y=0;
    while (!inRect(x,y,R,R,R,R,R)) {
        int clk = getClick();
        x = clk/clickratio;
        y = clk%clickratio;
    }
}

void savetoFile (map<char,char> &mapOfChar, string both[]) {
    if (both[0] == " "  || both[1] == " ") {
        Text fillCorrect (WINDOW_X/2, WINDOW_Y/8, "You haven't filled both!");
        wait(1);
        fillCorrect.hide();
        return;
    }
    else if (mapOfChar.find(both[0][0]) == mapOfChar.end()) {
        Text fillCorrect (WINDOW_X/2, WINDOW_Y/8, both[0] + " is not a command!");
        wait(1);
        fillCorrect.hide();
        return;
    }
    else if (both[0][0] == both[1][0]) {
        Text fillCorrect (WINDOW_X/2, WINDOW_Y/8, "Did you just try CHANGING " + both[0] + " to " + both[1] + "?");
        wait(1);
        fillCorrect.hide();
        return;
    }
    else if (mapOfChar.find(both[1][0])!=mapOfChar.end()) {
        Text fillCorrect (WINDOW_X/2, WINDOW_Y/8, both[1] + " is already a command!");
        wait(1);
        fillCorrect.hide();
        return;
    }
    mapOfChar[both[1][0]] = mapOfChar[both[0][0]];
    mapOfChar.erase(mapOfChar.find(both[0][0]));
    string fileName = "command-change.txt";
    ofstream outCommand (fileName);
    string towrite = "";
    for (auto it: mapOfChar) {
        towrite += it.first; towrite +=  " "; towrite += it.second; towrite += '\n';
    }
    towrite += "END";
    outCommand << towrite;
    outCommand.close();
    Text Correct (WINDOW_X/2, WINDOW_Y/8, "Changed " + both[0] + " to " + both[1] + "!");
    wait(2);
    return;
}

void changeCommands (map<char,char> &mapOfChar) {

    Text change (WINDOW_X/2, WINDOW_Y/2 - 3*textHeight(), "You can change commands (lower case alphabet or digit or = - ] [ \ ' ; . / ) here. Be careful though!");
    Text changeSecond (WINDOW_X/2, WINDOW_Y/2 , "Click on the box you want to enter text into.");
    Rectangle Rleft(WINDOW_X/2 - 20, WINDOW_Y/2 + 3*textHeight(), textwidth("1000"), textHeight()+10);
    Rectangle Rright(WINDOW_X/2 + 20, WINDOW_Y/2 + 3*textHeight(), textwidth("1000"), textHeight()+10);
    Rectangle Save (WINDOW_X/2, WINDOW_Y/2 + 6*textHeight(), textwidth("   Save   "), textHeight()+5);
    Rectangle Back (WINDOW_X/2, WINDOW_Y/2 + 9*textHeight(), textwidth("   Back   "), textHeight()+5);
    Text textSave (WINDOW_X/2, WINDOW_Y/2 + 6*textHeight(), "Save");
    Text textBack (WINDOW_X/2, WINDOW_Y/2 + 9*textHeight(), "Back");
    string both[2] = {" ", " "};
    set<char> special = {'-','=','[',']','\\',';','\'','/','.'};
    // First element is the one being changed, second is the one it is changed into.
    Text isChanged (WINDOW_X/2 - 20, WINDOW_Y/2 + 3*textHeight(), both[0]);
    Text changeTo (WINDOW_X/2 + 20, WINDOW_Y/2 + 3*textHeight(), both[1]);
    int xpos = 0, ypos = 0, which = -1;
    XEvent e;
    while (true) {
        if (checkEvent(e)) {
            if (mouseButtonPressEvent(e)) {
                xpos = e.xbutton.x; ypos = e.xbutton.y;
                switch (inRect(xpos,ypos,Rleft, Rright, Save, Back, Back)) {
                    case 0: which = -1; break;
                    case 1: which = 0; break;
                    case 2: which = 1; break;
                    case 3: savetoFile (mapOfChar, both); break;
                    case 4: return;
                }
            }
            if (keyPressEvent(e)) {
                char taken = charFromEvent(e);
                if (which != -1) {
                    if ((taken <= 'z' && taken >='a') || (taken <= '9' && taken >= '0') || special.find(taken)!=special.end()) {
                        both[which] = "";
                        both[which] += taken;
                        if (which == 1) {changeTo.setMessage(both[which]);}
                        else {isChanged.setMessage(both[which]);}
                    }
                    else {
                        Text fillCorrect (WINDOW_X/2, WINDOW_Y/8, "Invalid input.");
                        wait(1);
                        fillCorrect.hide();
                    }
                }
            }
        }
    }
}

int selectLevel() {
    int level;
    string easy = "Easy";
    string medium = "Medium";
    string difficult = "Difficult";
    string choose = "Choose a Difficulty Level:";
    string insane = "Insane";
    Rectangle Ez (WINDOW_X/2,WINDOW_Y/2-2*textHeight(), textWidth(easy)+10, textHeight()+10);
    Rectangle Med (WINDOW_X/2,WINDOW_Y/2+1*textHeight(), textWidth(medium)+10, textHeight()+10);
    Rectangle Diff (WINDOW_X/2,WINDOW_Y/2+4*textHeight(), textWidth(difficult)+10, textHeight()+10);
    Rectangle Insen (WINDOW_X/2,WINDOW_Y/2+7*textHeight(), textWidth(insane)+10, textHeight()+10);
    Text Easy (WINDOW_X/2,WINDOW_Y/2-2*textHeight(), easy);
    Text Medium (WINDOW_X/2,WINDOW_Y/2+1*textHeight(), medium);
    Text Difficult (WINDOW_X/2,WINDOW_Y/2+4*textHeight(), difficult);
    Text choosing (WINDOW_X/2,WINDOW_Y/2-6*textHeight(), choose);
    Text Insane (WINDOW_X/2,WINDOW_Y/2+7*textHeight(), insane);
    bool trueClick = 0;
    while ( !trueClick ) {
        int l = getClick();
        int xclk = l/clickratio; int yclk = l%clickratio;
        if ( !inRect(xclk, yclk, Ez, Med, Diff, Insen, Insen) ) {continue;}
        else {
            trueClick = 1;
            level = inRect(xclk, yclk, Ez, Med, Diff, Insen, Insen);
        }
    }
    return level;
}

void viewHelp () {
    string desc = "In this game, you throw a lasso to get the coin.";
    string cmd = "Following are the commands in the game.";
    string t = "t : Throw a lasso.";
    string l = "l : Loop the lasso. Coin is caught if in range.";
    string y = "y : Yank the lasso.";
    string q = "q : Quit from within the game.";
    string opencurly = "[ : Decrease throwing angle.";
    string closecurly = "] : Increase throwing angle.";
    string eq = "= : Increase speed at which lasso is thrown.";
    string dec = "- : Decrease speed at which lasso is thrown";
    string cmbnd = t+'\n'+l+'\n'+y+'\n'+q+'\n'+opencurly+'\n'+closecurly+'\n'+eq+'\n'+dec;
    Rectangle RDesc (WINDOW_X/2,WINDOW_Y/2-8*textHeight(),textWidth(desc)+15, textHeight()+10);
    Rectangle RPara (WINDOW_X/2,WINDOW_Y/2-5*textHeight(),textWidth(cmd)+15, textHeight()+10);
    Text Desc (WINDOW_X/2,WINDOW_Y/2-8*textHeight(), desc);
    Text Para (WINDOW_X/2,WINDOW_Y/2-5*textHeight(), cmd);
    Text T (WINDOW_X/2,WINDOW_Y/2+0*textHeight(), t);
    Text L (WINDOW_X/2,WINDOW_Y/2+1*textHeight(), l);
    Text Y (WINDOW_X/2,WINDOW_Y/2+2*textHeight(), y);
    Text Q (WINDOW_X/2,WINDOW_Y/2+3*textHeight(), q);
    Text OPEN (WINDOW_X/2,WINDOW_Y/2+4*textHeight(), opencurly);
    Text CLOSE (WINDOW_X/2,WINDOW_Y/2+5*textHeight(), closecurly);
    Text EQUAL (WINDOW_X/2,WINDOW_Y/2+6*textHeight(), eq);
    Text MINUS (WINDOW_X/2,WINDOW_Y/2+7*textHeight(), dec);
    backButton(WINDOW_X/2,WINDOW_Y/2+8*textHeight()+20);
}

int modeChoice () {
    string desc = "Which mode would you like to play?";
    string vanil = "Coin-Toss";
    string math = "Equation-Toss";
    Text Desc (WINDOW_X/2,WINDOW_Y/2-4*textHeight(), desc);
    Text Vanilla (WINDOW_X/2,WINDOW_Y/2+0*textHeight(), vanil);
    Text Math (WINDOW_X/2,WINDOW_Y/2+2*textHeight(), math);
    Rectangle plain (WINDOW_X/2,WINDOW_Y/2+0*textHeight(), textWidth(vanil+math), textHeight()+10);
    Rectangle eq (WINDOW_X/2,WINDOW_Y/2+2*textHeight(), textWidth(vanil+math), textHeight()+10);
    bool trueClick = 0;
    while ( !trueClick ) {
        int l = getClick();
        int xclk = l/clickratio; int yclk = l%clickratio;
        if ( !inRect(xclk, yclk, plain, eq, plain, eq, eq) ) {continue;}
        else {
            trueClick = 1;
            return inRect(xclk, yclk, plain, eq, plain, eq, eq)%2;
        }
    }
}

int modeChoiceHS () {
    string desc = "Which mode would you like to see high-scores of?";
    string vanil = "Coin-Toss";
    string math = "Equation-Toss";
    Text Desc (WINDOW_X/2,WINDOW_Y/2-4*textHeight(), desc);
    Text Vanilla (WINDOW_X/2,WINDOW_Y/2+0*textHeight(), vanil);
    Text Math (WINDOW_X/2,WINDOW_Y/2+2*textHeight(), math);
    Rectangle plain (WINDOW_X/2,WINDOW_Y/2+0*textHeight(), textWidth(vanil+math), textHeight()+10);
    Rectangle eq (WINDOW_X/2,WINDOW_Y/2+2*textHeight(), textWidth(vanil+math), textHeight()+10);
    bool trueClick = 0;
    while ( !trueClick ) {
        int l = getClick();
        int xclk = l/clickratio; int yclk = l%clickratio;
        if ( !inRect(xclk, yclk, plain, eq, plain, eq, plain) ) {continue;}
        else {
            trueClick = 1;
            return inRect(xclk, yclk, plain, eq, plain, eq, plain)%2;
        }
    }
}

bool showMenu ( bool started, map<char,char> &mapOfChar) {
    while ( !started ) {
        string strt = "Start New Game";
        string hs = "View High-scores";
        string quit = "Quit";
        string help = "How to play?";
        string commands = "Change command keys";
        Rectangle R2 (WINDOW_X/2,WINDOW_Y/2-6*textHeight(),textWidth(strt)+15, textHeight()+10);
        Rectangle R3 (WINDOW_X/2,WINDOW_Y/2-2*textHeight(),textWidth(help)+15, textHeight()+10);
        Rectangle R4 (WINDOW_X/2,WINDOW_Y/2+2*textHeight(),textWidth(hs)+15, textHeight()+10);
        Rectangle R5 (WINDOW_X/2,WINDOW_Y/2+6*textHeight(),textWidth(commands)+15, textHeight()+10);
        Rectangle R6 (WINDOW_X/2,WINDOW_Y/2+10*textHeight(),textWidth(quit)+15, textHeight()+10);
        Text T2 (WINDOW_X/2, WINDOW_Y/2-6*textHeight(),strt);
        Text T3 (WINDOW_X/2, WINDOW_Y/2-2*textHeight(),help);
        Text T4 (WINDOW_X/2, WINDOW_Y/2+2*textHeight(),hs);
        Text T5 (WINDOW_X/2, WINDOW_Y/2+6*textHeight(),commands);
        Text T6 (WINDOW_X/2, WINDOW_Y/2+10*textHeight(),quit);
        // Button Functioning
        bool trueClick = 0;
        while ( !trueClick ) {
            int l = getClick();
            int xclk = l/clickratio; int yclk = l%clickratio;
            if ( !inRect(xclk, yclk, R2, R3, R4, R5, R6) ) {continue;}
            else { trueClick = 1; }
            switch (inRect(xclk, yclk, R2, R3, R4, R5, R6)) {
                case 1 : started = 1; break;
                case 2 : HIDE; viewHelp(); break;
                case 3 : HIDE; viewHS(); break; // Hiding as a part of viewHS also.
                case 4 : HIDE; changeCommands(mapOfChar); break;
                case 5 : quit = 1; return 0;
            }
        }
        HIDE;
    }
    return 1;
}
bool overScreen (int score) {
    clearviewport();
    stringstream ss;
    ss << score;
    string score_string = ss.str();
    Rectangle over (WINDOW_X/2,WINDOW_Y/2, textWidth("Your score was 1000000")+15, 7*textHeight()+10);
    over.setFill(true);
    over.setColor(COLOR("yellow"));
    Text T1 (WINDOW_X/2, WINDOW_Y/2 - 2*textHeight(), "Game Over");
    Text T2 (WINDOW_X/2, WINDOW_Y/2 + 2*textHeight(), "Your score was " + score_string);
    backButton(WINDOW_X/2, WINDOW_Y/2 + 8*(textHeight())+20);
}
