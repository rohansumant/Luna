#include <string>
using namespace std;

int get_matching_paranthesis(const string &a,int l,int r) {
    assert(a[l] == '(');
    int cnt = 1;
    for(int i=l+1;i<=r;i++) {
        if(a[i] == '(') cnt++;
        else if(a[i] == ')') {
            cnt--;
            if(!cnt) return i;
        }
    }
    assert(0);
    return -1;
}



int get_matching_brace(const string &a,int l,int r) {
    assert(a[l] == '{');
    int cnt = 1;
    for(int i=l+1;i<=r;i++) {
        if(a[i] == '{') cnt++;
        else if(a[i] == '}') {
            cnt--;
            if(!cnt) return i;
        }
    }
    assert(0);
    return -1;
}


vector<string> split(const string &a,int l,int r,char ch) {
    int i = l;
    vector<string> ls;
    while(i <= r) {
        string curr;
        while(i <= r && a[i] != ch) {
            curr += a[i];
            i++;
        }
        ls.push_back(curr);
        i++;
    }
    return ls;
}
