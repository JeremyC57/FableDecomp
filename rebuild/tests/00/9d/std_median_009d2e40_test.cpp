// behaviour test for 0x009D2E40 std::__median<unsigned long, CompareDataOffset >: every ordering of three entry indices returns the one with the middle data offset; ties fall through to b
#include <cstdio>
struct Comp { const unsigned int* offs; bool operator()(unsigned int a, unsigned int b) { return offs[a] < offs[b]; } };
template <class T, class C> static const T& median(const T& a, const T& b, const T& c, C comp)
{
    if (comp(a, b))
        if (comp(b, c)) return b;
        else if (comp(a, c)) return c;
        else return a;
    else if (comp(a, c)) return a;
    else if (comp(b, c)) return c;
    else return b;
}
int main()
{
    int fails = 0;
    const int perm[6][3] = { { 0, 1, 2 }, { 0, 2, 1 }, { 1, 0, 2 }, { 1, 2, 0 }, { 2, 0, 1 }, { 2, 1, 0 } };
    static const unsigned int offs[3] = { 300, 100, 200 };   // entry 2 has the middle data offset
    Comp c; c.offs = offs;
    const unsigned int idx[3] = { 0, 1, 2 };
    for (int i = 0; i < 6; ++i)
        if (median(idx[perm[i][0]], idx[perm[i][1]], idx[perm[i][2]], c) != 2) fails |= 1;
    unsigned int x = 1, y = 1, z = 1;
    if (&median(x, y, z, c) != &y) fails |= 2;                 // all equal: b
    if (fails) { std::printf("009d2e40_TEST FAIL %d\n", fails); return 1; }
    std::printf("009d2e40_TEST PASS\n");
    return 0;
}