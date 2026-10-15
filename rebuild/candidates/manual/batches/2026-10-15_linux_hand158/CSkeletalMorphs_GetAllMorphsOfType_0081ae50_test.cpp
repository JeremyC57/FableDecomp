// Behaviour test for 0x0081AE50: equal-range copy out of a sorted key/entry list.
#include <cstdio>
struct Entry { int Morph; unsigned short Index; bool Enabled; };
struct Pair { unsigned int first; Entry second; };
static bool Get(const Pair* b, const Pair* e, unsigned int type, Entry* out, int& n)
{
    const Pair* lo = b; while (lo != e && lo->first < type) ++lo;
    const Pair* hi = lo; while (hi != e && !(type < hi->first)) ++hi;
    if (lo != hi) { int count = (int)(hi - lo); n = count; for (int i = 0; i < count; ++i) out[i] = (lo++)->second; return true; }
    return false;
}
int main()
{
    Pair p[4] = {{1, {10, 0, true}}, {2, {20, 1, false}}, {2, {21, 2, true}}, {5, {50, 3, true}}};
    Entry out[4]; int n = 0;
    bool ok = Get(p, p + 4, 2, out, n) && n == 2 && out[0].Morph == 20 && out[1].Morph == 21 && out[1].Enabled;
    ok = ok && !Get(p, p + 4, 3, out, n);
    if (!ok) { std::printf("0081ae50_TEST FAIL\n"); return 1; }
    std::printf("0081ae50_TEST PASS\n");
    return 0;
}
