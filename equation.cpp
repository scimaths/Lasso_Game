#include <bits/stdc++.h>
using namespace std;

string createLinearEquation (int ans) {
    srand(time(0));
    string toReturn = "";
    string toAdd = "";
    // Equation of the form ax+b=c
    int a = rand()%20+1;
    int b = rand()%20+1;
    int x = ans;
    int c = a*x+b;
    stringstream ss;
    ss << a;
    ss >> toAdd;
    if (toAdd!="1") {toReturn += toAdd;}
    toReturn += "x + ";
    stringstream ss2;
    ss2 << b;
    ss2 >> toAdd;
    toReturn += toAdd;
    toReturn += " = ";
    stringstream ss3;
    ss3 << c;
    ss3 >> toAdd;
    toReturn += toAdd;
    return toReturn;
}

string createQuadraticEquation (int ans) {
    srand(time(0));
    int root1 = ans;
    int root2 = -(rand()%25 + 1);
    int sum = root1 + root2;
    int product = root1 * root2;
    string toReturn = "x*x";
    stringstream ss;
    string toAdd = "";
    if (sum!=0) {
        if (sum>0) {toReturn+='-';}
        else if (sum<0) {toReturn+='+';}
        if (abs(sum)!=1) {
            ss << abs(sum);
            ss >> toAdd;
            toReturn += toAdd;
        }
        toReturn += 'x';
    }
    toAdd = "";
    if (product != 0) {
        if (product < 0) {toReturn+='-';}
        else {toReturn+='+';}
        stringstream ss2;
        ss2 << abs(product);
        ss2 >> toAdd;
        toReturn+=toAdd;
    }
    toReturn += "=0";
    return toReturn;
}
