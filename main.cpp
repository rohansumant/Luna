#include <iostream>
#include <string>
#include <cassert>
#include "stmt.cpp"
using namespace std;

vector<pair<string,map<string,int>>> stack;
map<string,pair<vector<string>,string>> func_map;

void parse_fn(const string &a,int l,int starting_brace,int r) {
    assert(a[l] == '@');
    pair<string,vector<string>> name_args = read_fn_call_noneval(a,l+1,starting_brace-1);
    auto it = func_map.find(name_args.first);
    if(it != func_map.end()) {
        cerr << "Function " << name_args.first << " redefined" << endl;
        return;
    }
    string name = name_args.first;
    vector<string> args = name_args.second;
    func_map[name] = {args,a.substr(starting_brace+1,(r-1)-(starting_brace+1)+1)};
}

void parse_prog(const string &a,int l,int r) {
    if(a[l] == '@') {
        int i = l;
        while(i <= r && a[i] != '{') i++;
        if(i > r) {
            cerr << "Failed to parse program" << endl;
            return;
        }
        int ix = get_matching_brace(a,i,r);
        parse_fn(a,l,i,ix);
        parse_prog(a,ix+1,r);
    } else {
        parse_exp(a,l,r);
    }
}

int main() {
    stack.push_back({"main",{}});
    string program; cin >> program;
    parse_prog(program,0,program.length()-1);
}
