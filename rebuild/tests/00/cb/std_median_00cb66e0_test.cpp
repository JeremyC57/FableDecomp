// behaviour test for 0x00CB66E0 std::__median over std::pair<CCharString, CScriptInfo > (CCharString key compare with the null rules):
// every ordering of three distinct keys returns the middle one; ties and null keys follow the STLport branch order
#include <cstdio>
#include <cstring>
struct Key {
    const char* p;
    bool operator<(const Key& o) const
    {
        if (o.p == p) return false;
        if (p == 0) return true;
        if (o.p == 0) return false;
        return std::strcmp(p, o.p) < 0;
    }
};
struct Entry { Key first; int second; };
struct Less { bool operator()(const Entry& a, const Entry& b) const { return a.first < b.first; } };
static const Entry& median(const Entry& a, const Entry& b, const Entry& c, Less comp)
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
    static const char s1[] = "alpha", s2[] = "beta", s3[] = "gamma";
    Entry e[3] = { { { s1 }, 1 }, { { s2 }, 2 }, { { s3 }, 3 } };
    const int perm[6][3] = { { 0, 1, 2 }, { 0, 2, 1 }, { 1, 0, 2 }, { 1, 2, 0 }, { 2, 0, 1 }, { 2, 1, 0 } };
    for (int i = 0; i < 6; ++i)
        if (median(e[perm[i][0]], e[perm[i][1]], e[perm[i][2]], Less()).second != 2) fails |= 1;
    Entry n = { { 0 }, 9 };                                  // a null key sorts first
    if (median(n, e[0], e[1], Less()).second != 1) fails |= 2;
    if (&median(e[0], e[0], e[0], Less()) != &e[0]) fails |= 4;   // all equal: falls through to b (same object here)
    Entry x = { { s1 }, 7 }, y = { { s1 }, 8 }, z = { { s1 }, 9 };
    if (median(x, y, z, Less()).second != 8) fails |= 8;            // equal keys (distinct data pointers? same literal): b
    if (fails) { std::printf("00cb66e0_TEST FAIL %d\n", fails); return 1; }
    std::printf("00cb66e0_TEST PASS\n");
    return 0;
}