// behaviour test for 0x007F9F60 std::__median<CThing*, CThingCompare_BestForTargeting2 >: every ordering of three things returns the middle one; ties fall through to b
#include <cstdio>
struct Thing { float v; };
struct Comp { bool operator()(const Thing* a, const Thing* b) const { return a->v > b->v; } };
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
    Thing t[3] = { { 1.0f }, { 2.0f }, { 3.0f } };
    Thing* p[3] = { &t[0], &t[1], &t[2] };
    for (int i = 0; i < 6; ++i)
        if (median(p[perm[i][0]], p[perm[i][1]], p[perm[i][2]], Comp()) != &t[1]) fails |= 1;
    Thing* e0 = &t[0]; Thing* e1 = &t[0]; Thing* e2 = &t[0];
    if (&median(e0, e1, e2, Comp()) != &e1) fails |= 2;      // all equal: b
    Thing* f0 = &t[2]; Thing* f1 = &t[2]; Thing* f2 = &t[0];
    if (&median(f0, f1, f2, Comp()) != &f0 && &median(f0, f1, f2, Comp()) != &f1) fails |= 4;
    if (fails) { std::printf("007f9f60_TEST FAIL %d\n", fails); return 1; }
    std::printf("007f9f60_TEST PASS\n");
    return 0;
}