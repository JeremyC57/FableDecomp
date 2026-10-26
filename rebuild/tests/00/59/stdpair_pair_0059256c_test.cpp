// behaviour test for 0x0059256C std::pair::pair (pair<CCharString, CCharString>): the first member is copy-constructed before the second, each from the source's matching member, and the result is this
#include <cstdio>
static int gLog[8], gN = 0;
struct M {
    int V;
    M(int v) : V(v) { }
    M(const M& o) : V(o.V) { if (gN < 8) gLog[gN++] = 100 + o.V; }
    M& operator=(const M& o) { V = o.V; if (gN < 8) gLog[gN++] = 200 + o.V; return *this; }
};
struct P {                                                 // the function under test, member-wise
    M first, second;
    P(const M& a, const M& b) : first(a), second(b) { }
    P(const P& o) : first(o.first), second(o.second) { }
    P& operator=(const P& o) { first = o.first; second = o.second; return *this; }
};
int main()
{
    int fail = 0;
    P src(M(1), M(2));
    gN = 0;
    P dst(src);
    if (gN != 2 || gLog[0] != 101 || gLog[1] != 102) fail |= 1;
    if (dst.first.V != 1 || dst.second.V != 2) fail |= 2;
    if (fail) { std::printf("0059256c_TEST FAIL %d\n", fail); return 1; }
    std::printf("0059256c_TEST PASS\n");
    return 0;
}