#include <bits/stdc++.h>
#include <simplecpp>
#include "lasso.h"
#include "hs.h"
#include "menu.h"
using namespace std;

vector<char> getInputName() {

    Text enterName (WINDOW_X/2, WINDOW_Y/2-2*textHeight(), "Enter your name here (max 10 characters)");
    Rectangle Nameshown (WINDOW_X/2, WINDOW_Y/2, textwidth("Enter your name here (max 10 characters)"),textHeight()+5);
    Rectangle Button (WINDOW_X/2, WINDOW_Y/2+3*textHeight(), textwidth("Submit Name"),textHeight()+5);
    Text Vacant (WINDOW_X/2, WINDOW_Y/2, " ");
    Text SubmitButton (WINDOW_X/2, WINDOW_Y/2+3*textHeight(), "Submit Name");
    XEvent e;
    vector<char> v;
    //nextEvent(e);
    while (true) {
        if (checkEvent(e)) {
            if (mouseButtonPressEvent(e)) {
                if (e.xbutton.x <= WINDOW_X/2+textwidth("Submit Name")/2 && e.xbutton.x >= WINDOW_X/2-textwidth("Submit Name")/2
                 && e.xbutton.y <= WINDOW_Y/2+7*textHeight()/2 && e.xbutton.y >= WINDOW_Y/2-5*textHeight()/2
                 && (int)v.size() > 0) {
                    return v;
                 }
            }
            else if (keyPressEvent(e)) {
                char c = charFromEvent(e);
                if (c == 8) {if ((int)v.size()>0) {v.pop_back();}}
                else if (c == 13 && (int)v.size()>0) {return v;}
                else if ((int)v.size()<=10) {
                    if ((c<='z'&&c>='a')||(c<='Z'&&c>='A')||(c<='9'&&c>='0')||c==' '){
                        v.push_back(c);
                    }
                }
            }
        }
        string name = ""; for (int i=0; i<(int)v.size(); ++i) {name += v[i];}
        Vacant.setMessage(name);
    }

}

int stoi (string s, int sizeString) {
    int ans=0;
    for (int i=sizeString-1; i>=0; --i) {
        ans *= 10;
        ans += (s[i]-'0');
    }
    return ans;
}

bool cmp (vector<string> first, vector<string> second) {
    int firstScore = stoi(first[0], (int)first[0].size());
    int secondScore = stoi(second[0], (int)second[0].size());
    if (firstScore != secondScore) {return firstScore > secondScore;}
    else {
        return first[1] > second[1]; // According to Names
    }
}

void process (string combined, string &date, string &score, string &name, int stringSize) {
    int i=0;
    while (combined[i]!=' ') {i++;}
    date = combined.substr(0,i);
    while (combined[i]==' ') {i++;}
    int scoreStart=i; // For storing start of score
    while (combined[i]!=' ') {i++;}
    score = combined.substr(scoreStart,i-scoreStart);
    while (combined[i]==' ') {i++;}
    name = combined.substr(i,stringSize-i);
}

void viewHS () {
    int which_mode = modeChoiceHS();
    string scores = "";
    string fileName = "";
    if (which_mode == 1) {
        fileName = "High-scores-coin.txt";
    }
    else if (which_mode == 0) {
        fileName = "High-scores-num.txt";
    }
    ifstream HStest(fileName);
    if (!HStest) {
        ofstream outfile (fileName);
        outfile << "END";
        outfile.close();
    }
    ifstream HS (fileName);
    vector<vector<string>> ListIntake;
    getline(HS, scores);
    while (scores!="END") {
        string thisDate="", thisScore="", thisName="";
        process (scores, thisDate, thisScore, thisName, (int)scores.size());
        vector<string> EachIntake = {thisScore, thisName, thisDate};
        ListIntake.push_back(EachIntake);
        getline (HS,scores);
    }
    sort (ListIntake.begin(), ListIntake.end(), cmp);
    HS.close();

    int numEntries = ListIntake.size();
    int cnt = -8;
    Text nameTop (WINDOW_X/3, WINDOW_Y/2 + cnt*(textHeight()+5), "Name");
    Text scoreTop (WINDOW_X/2, WINDOW_Y/2 + cnt*(textHeight()+5), "Score");
    Text dateTop (WINDOW_X/1.5, WINDOW_Y/2 + cnt*(textHeight()+5), "Date");
    // Text sc (WINDOW_X/2, WINDOW_Y/2 + cnt*(textHeight()+5), scores);
    vector<vector<Text>> textsShown;
    cnt++;
    for (int i=0; i < min(numEntries,5); ++i) {
        vector<Text> example;
        string entryNumberString;
        stringstream ss; ss << i+1; ss >> entryNumberString;
        string thisScore = ListIntake[i][0];
        string thisName = ListIntake[i][1];
        string thisDate = ListIntake[i][2];
        Text entryNumber (WINDOW_X/3.3 - textwidth("1")/2, WINDOW_Y/2 + cnt*(textHeight()+5), entryNumberString);
        Text entryName (WINDOW_X/3, WINDOW_Y/2 + cnt*(textHeight()+5), thisName);
        Text entryScore (WINDOW_X/2, WINDOW_Y/2 + cnt*(textHeight()+5), thisScore);
        Text entryDate (WINDOW_X/1.5, WINDOW_Y/2 + cnt*(textHeight()+5), thisDate);
        example = {entryNumber, entryName, entryScore, entryDate};
        textsShown.push_back(example);
        cnt++;
    }
    cnt+=2;
    backButton(WINDOW_X/2, WINDOW_Y/2 + cnt*(textHeight()+5)+20);
}

void HSUpdate (int type, int score) {
    ostringstream txt;
    stringstream ss;
    string scorestring;
    ss << score;
    ss >> scorestring;
    string fileName = " ";
    if (type == 0) {
        fileName = "High-scores-coin.txt";
    }
    else if (type == 1) {
        fileName = "High-scores-num.txt";
    }
    ifstream HStest (fileName);
    if (!HStest) {
        ofstream outfile (fileName);
        outfile << "END";
        outfile.close();
    }
    ifstream HS (fileName);
    txt << HS.rdbuf();
    string contained = txt.str();
    vector<char> nameVec = getInputName();
    string name = ""; for (int i=0; i<(int)nameVec.size(); ++i) {name += nameVec[i];}
    string willchange = "";
    int i = 0;
    while (contained.substr(i,3)!="END") {
        willchange += contained[i];
        i++;
    }
    willchange += "20/02/2021 " + scorestring + " " + name + "\nEND";
    ofstream HSout (fileName);
    HSout << willchange;
}
