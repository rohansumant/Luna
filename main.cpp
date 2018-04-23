#include <iostream>
#include <string>
#include "stmt.cpp"
using namespace std;

vector<pair<string,map<string,int>>> stack;

int main() {
    stack.push_back({"main",{}});
    string block;
    cin >> block;
    parse_block(block,0,block.size()-1,"fn");
}
