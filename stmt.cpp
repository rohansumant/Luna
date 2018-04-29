#ifndef STMT_CPP
#define STMT_CPP
#include "exp.cpp"
#include <cassert>

extern vector<pair<string,map<string,int>>> stack;

int parse_stmt(const string&,int,int,bool&);

vector<string> read_stmts(const string &a,int l,int r) {
    int b_cnt = 0;
    vector<string> stmts;
    int base = l;
    while(base <= r) {
        int i = base;
        while(i <= r && !(a[i] == ';' && b_cnt == 0)) {
            if(a[i] == '{') b_cnt++;
            else if(a[i] == '}') b_cnt--;
            i++;
        }
        string stmt = a.substr(base,i-base);
        if(stmt.size()) stmts.push_back(stmt);
        base = i+1;
    }
    return stmts;
}

int parse_block(const string &a,int l,int r,string type,bool &hit_return,map<string,int> args = {}) {
    map<string,int> new_frame;
    if(args.size()) new_frame = args;
    stack.push_back({type,new_frame});
    vector<string> stmts = read_stmts(a,l,r); 
    for(string stmt:stmts) {
        int val = parse_stmt(stmt,0,stmt.size()-1,hit_return);
        if(hit_return) {
            stack.pop_back();
            return val;
        }
    }
    stack.pop_back();
    return 0;
}

int parse_stmt(const string &a,int l,int r,bool &hit_return) {
    int len = r-l+1;
    bool found_assignment = 0;
    for(int i=l;i<=r;i++) if(i > l && a[i] == '=' && a[i-1] != '<' && a[i-1] != '>') {
        found_assignment = 1;
        break;
    }
    if(len >= 3 && a.substr(l,3) == "if(") {
        int p1 = l+2;
        assert(a[p1] == '(');
        int p2 = get_matching_paranthesis(a,p1,r);
        int exp = parse_exp(a,p1+1,p2-1);
        if(exp) {
            int b1 = p2+1;
            assert(a[b1] == '{');
            int b2 = get_matching_brace(a,b1,r);
            int val = parse_block(a,b1+1,b2-1,"if",hit_return);
            if(hit_return) return val;
        }
    } else if(len >= 6 && a.substr(l,6) == "while(") {
        int p1 = l+5;
        assert(a[p1] == '(');
        int p2 = get_matching_paranthesis(a,p1,r);
        while(parse_exp(a,p1+1,p2-1)) {
            int b1 = p2+1;
            assert(a[b1] == '{');
            int b2 = get_matching_brace(a,b1,r);
            int val = parse_block(a,b1+1,b2-1,"while",hit_return);
            if(hit_return) return val;
        }
    } else if(len >= 7 && a.substr(l,7) == "return(") {
        int p1 = l+6;
        assert(a[p1] == '(');
        int p2 = get_matching_paranthesis(a,p1,r);
        int exp = parse_exp(a,p1+1,p2-1);
        hit_return = 1;
        return exp;
    } else if(len >= 6 && a.substr(l,6) == "print(") {
        int p1 = l+5;
        assert(a[p1] == '(');
        int p2 = get_matching_paranthesis(a,p1,r);
        vector<int> args = read_fn_args(a,p1+1,p2-1);
        for(int it:args) {
            cout << it << " ";
        }
        cout << endl;
    } else if(found_assignment) {
        vector<string> args = split(a,l,r,'=');
        assert(args.size() == 2);
        string var = args[0];
        int exp = parse_exp(args[1],0,args[1].size()-1);
        bool done = 0;
        for(int i=stack.size()-1;i>=0;i--) {
            bool end_of_frame = (stack[i].first == "fn");
            auto it = stack[i].second.find(var);
            if(it != stack[i].second.end()) {
                it->second = exp;
                done = 1;
                break;
            }
            if(end_of_frame) break;
        }
        if(!done) {
            stack.back().second[var] = exp;
        }
    } else {
        parse_exp(a,l,r);
    }
    return 0;
}
#endif
