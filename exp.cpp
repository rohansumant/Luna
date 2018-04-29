#ifndef EXP_CPP
#define EXP_CPP
#include <string>
#include <cassert>
#include <vector>
#include <map>
#include "common.cpp"
using namespace std;

extern vector<pair<string,map<string,int>>> stack;
extern map<string,pair<vector<string>,string>> func_map;

int parse_exp(const string&,int,int);
int parse_block(const string&,int,int,string,bool&,map<string,int>);

int read_fn_call_length(const string &a,int l,int r) {
    int i = l;
    while(i <= r && isalpha(a[i])) i++;
    assert(i <= r && a[i] == '(');
    int ix = get_matching_paranthesis(a,i,r);
    return ix-l+1;
}

int eval_fn(string name,vector<int> args) {
    auto it = func_map.find(name);
    if(it == func_map.end()) {
        cerr << "Function: " << name << " not defined" << endl;
        return -1;
    }
    vector<string> var_args = it->second.first;
    string body = it->second.second;
    if(var_args.size() != args.size()) {
        cerr << "Argument count mismatch for function " << name << endl;
        return -1;
    }
    map<string,int> arg_ls;
    for(int i=0;i<args.size();i++) {
        arg_ls[var_args[i]] = args[i];
    }
    bool hit_return = 0;
    int val = parse_block(body,0,body.size()-1,"fn",hit_return,arg_ls);
    assert(hit_return);
    return val;
}

int fetch(const string &var) {
    for(int i=stack.size()-1;i>=0;i--) {
        bool end_of_frame = stack[i].first == "fn";
        auto it = stack[i].second.find(var);
        if(it != stack[i].second.end()) return it->second;
        if(end_of_frame) break;
    }
    cerr << "No such variable found: " << var << endl;
    return -1;
}

string read_op(const string &a,int l,int r) {
    if(l < r && a.substr(l,2) == "==") return a.substr(l,2);
    else if(l < r && a.substr(l,2) == "<=") return a.substr(l,2);
    else if(l < r && a.substr(l,2) == ">=") return a.substr(l,2);
    else return a.substr(l,1);
}

string read_var(const string &a,int l,int r) {
    string var;
    for(int i=l;i <= r && isalpha(a[i]);i++) var += a[i];
    return var;
}

string read_num(const string &a,int l,int r) {
    string num;
    for(int i=l;i <= r && isdigit(a[i]);i++) num += a[i];
    return num;
}

bool read_maybe_fn_call(const string &a,int l,int r) {
    int i = l;
    while(i <= r && isalpha(a[i])) i++;
    if(i > r) return 0;
    return a[i] == '(';
}

vector<string> read_fn_args_noneval(const string &a,int l,int r) {
    int p_cnt = 0;
    int i = l, base = l;
    vector<string> args;
    while(i <= r) {
        if(a[i] == '(') p_cnt++;
        else if(a[i] == ')') p_cnt--;
        else if(a[i] == ',' && !p_cnt) {
            args.push_back(a.substr(base,i-base));
            base = i+1;
        }
        i++;
    }
    if(r >= base) args.push_back(a.substr(base,r-base+1));
    return args;
}



vector<int> read_fn_args(const string &a,int l,int r) {
    int p_cnt = 0;
    int i = l, base = l;
    vector<int> args;
    while(i <= r) {
        if(a[i] == '(') p_cnt++;
        else if(a[i] == ')') p_cnt--;
        else if(a[i] == ',' && !p_cnt) {
            args.push_back(parse_exp(a,base,i-1));
            base = i+1;
        }
        i++;
    }
    args.push_back(parse_exp(a,base,r));
    return args;
}


pair<string,vector<string>> read_fn_call_noneval(const string &a,int l,int r) {
    string name;
    int i = l;
    for(;i <= r && isalpha(a[i]);i++) name += a[i];
    assert(a[i] == '(');
    int ix = get_matching_paranthesis(a,i,r);
    assert(ix <= r);
    vector<string> args;
    if(ix == i+1) return {name,args};
    args = read_fn_args_noneval(a,i+1,ix-1);
    return {name,args};
}


pair<string,vector<int>> read_fn_call(const string &a,int l,int r) {
    string name;
    int i = l;
    for(;i <= r && isalpha(a[i]);i++) name += a[i];
    assert(a[i] == '(');
    int ix = get_matching_paranthesis(a,i,r);
    assert(ix <= r);
    vector<int> args;
    if(ix == i+1) return {name,args};
    args = read_fn_args(a,i+1,ix-1);
    return {name,args};
}


int eval_op(const string &op,int e1,int e2) {
    if(op == "*") return e1*e2;
    else if(op == "+") return e1+e2;
    else if(op == "-") return e1-e2;
    else if(op == "/") return e1/e2;
    else if(op == "%") return e1%e2;
    else if(op == "<") return e1 < e2;
    else if(op == "<=") return e1 <= e2;
    else if(op == ">=") return e1 >= e2;
    else if(op == "==") return e1 == e2;
    else if(op == ">") return e1 > e2;
    else cerr << "Unknown operator: " << op << endl;
    return 0;
}

int parse_exp(const string &a,int l,int r) {
    if(l > r) {
        cerr << "Error: expression length < 0 in: " << a << " " << l << " " << r << endl;
        return 0;
    }
    int e1,ix;
    if(a[l] == '(') {
        ix = get_matching_paranthesis(a,l,r);
        e1 = parse_exp(a,l+1,ix-1);
        if(ix == r) return e1;
        ix++;
    } else if(isalpha(a[l]) && !read_maybe_fn_call(a,l,r)) {
        string var = read_var(a,l,r);
        ix = l+var.length();
        e1 = fetch(var);
        if(ix == r+1) return e1;
    } else if(isdigit(a[l])) {
        string num = read_num(a,l,r);
        ix = l+num.length();
        e1 = stoi(num);
        if(ix == r+1) return e1;
    } else {
        pair<string,vector<int>> fn = read_fn_call(a,l,r);
        ix = l+read_fn_call_length(a,l,r);
        e1 = eval_fn(fn.first,fn.second);
        if(ix == r+1) return e1;
    }
    string op = read_op(a,ix,r);
    return eval_op(op,e1,parse_exp(a,ix+op.length(),r));
}
#endif
