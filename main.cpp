#include <iostream>
#include <string>
#include "exp.cpp"
using namespace std;

vector<map<string,int>> stack = {{{"aa",3}}};

int main() {
    string exp;
    cin >> exp;
    while(!cin.eof()) {
        cout << parse_exp(exp,0,exp.size()-1) << endl;
        cin >> exp;
    }
}
