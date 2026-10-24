// Behaviour test for 0x006611F7 std::__copy(set<long> range, back_inserter(vector<long>)) (model copy)
#include <cstdio>
struct Vec { long data[8]; int n; void push_back(const long& v) { data[n++] = v; } };
struct Inserter { Vec* c; Inserter& operator=(const long& v) { c->push_back(v); return *this; } Inserter& operator*() { return *this; } Inserter& operator++() { return *this; } };
struct It { const long* p; long& operator*() const { return *(long*)p; } It& operator++() { ++p; return *this; } bool operator!=(const It& o) const { return p != o.p; } };
Inserter copy_model(It first, It last, Inserter result) { for (; first != last; ++result, ++first) *result = *first; return result; }
int main()
{
    long keys[4] = { 3, 5, 8, 13 };     // a set walks its keys in order
    Vec v; v.n = 0;
    It f = { keys }, l = { keys + 4 };
    Inserter ins = { &v };
    Inserter r = copy_model(f, l, ins);
    if (r.c != &v || v.n != 4 || v.data[0] != 3 || v.data[3] != 13) { std::printf("006611f7_TEST FAIL\n"); return 1; }
    It e = { keys }; r = copy_model(e, e, ins);
    if (v.n != 4) { std::printf("006611f7_TEST FAIL\n"); return 1; }
    std::printf("006611f7_TEST PASS\n");
    return 0;
}