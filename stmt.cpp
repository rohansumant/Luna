#include "exp.cpp"
#include <cassert>

string parse_stmt(const string &a,int l,int r) {
    assert(a[r] == ';');
    r--;

