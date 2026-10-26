// behaviour test for 0x0082F860 _Rb_tree<unsigned __int64, ...>::insert_unique: a new key (smaller, larger, between, differing
// only in the high or the low half) is inserted and reported true; an existing key returns its node and false
#include <cstdio>
#include <set>
typedef unsigned long long U64;
static std::pair<std::set<U64>::iterator, bool> ins(std::set<U64>& s, U64 k) { return s.insert(k); }
int main()
{
    int fail = 0;
    std::set<U64> s;
    if (!ins(s, 5).second || !ins(s, 1).second) fail |= 1;
    U64 hi = ((U64)1 << 32) | 5u;                          // equal low half, larger high half
    if (!ins(s, hi).second) fail |= 2;
    std::pair<std::set<U64>::iterator, bool> r = ins(s, 5);
    if (r.second || *r.first != 5) fail |= 4;              // duplicate: the existing node, false
    if (!ins(s, 3).second || ins(s, 3).second) fail |= 8;
    if (s.size() != 4u) fail |= 16;
    U64 expect[4] = { 1, 3, 5, hi }; int i = 0;
    for (std::set<U64>::iterator it = s.begin(); it != s.end(); ++it, ++i) if (*it != expect[i]) fail |= 32;
    if (fail) { std::printf("0082f860_TEST FAIL %d\n", fail); return 1; }
    std::printf("0082f860_TEST PASS\n");
    return 0;
}