#ifndef STMT_CPP
#define STMT_CPP
#include "exp.cpp"
#include <cassert>

extern vector<pair<string,map<string,int>>> stack;

int parse_stmt(const string&,int,int,bool&);

int parse_block(const string &a,int l,int r,string type) {
    map<string,int> new_frame;
    stack.push_back({type,new_frame});
    vector<string> stmts = split(a,l,r,';');
    for(string stmt:stmts) {
        bool hit_return = 0;
        int val = parse_stmt(stmt,0,stmt.size()-1,hit_return);
        if(hit_return) {
            int ix = stack.size()-1;
            while(ix >= 0 && stack[ix].first != "fn") stack.pop_back();
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
    for(int i=l;i<=r;i++) if(a[l] == '=') {
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
            parse_block(a,b1+1,b2-1,"if");
        }
    } else if(len >= 6 && a.substr(l,6) == "while(") {
        int p1 = l+5;
        assert(a[p1] == '(');
        int p2 = get_matching_paranthesis(a,p1,r);
        int exp = parse_exp(a,p1+1,p2-1);
        while(exp) {
            int b1 = p2+1;
            assert(a[b1] == '{');
            int b2 = get_matching_brace(a,b1,r);
            parse_block(a,b1+1,b2-1,"while");
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
