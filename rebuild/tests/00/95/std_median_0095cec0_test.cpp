// behaviour test for 0x0095CEC0 std::__median<CThing*, CThingCompare_Nearest >: the comparator returns the nearer pointer, so every non-null pair tests true and the result is b; null pairs fall through
#include <cstdio>
struct Thing { float x; };
struct Comp {
    float pos;
    Thing* operator()(Thing* a, Thing* b) const
    {
        if (!a) return b;
        if (!b) return a;
        float da = a->x - pos, db = b->x - pos;
        return da * da < db * db ? a : b;
    }
};
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
    Thing t[3] = { { 1.0f }, { 5.0f }, { 9.0f } };
    Comp c; c.pos = 0.0f;
    Thing* a = &t[0]; Thing* b = &t[1]; Thing* d = &t[2];
    if (&median(a, b, d, c) != &b) fails |= 1;      // non-null pairs always test true: b (the retail quirk)
    if (&median(d, a, b, c) != &a) fails |= 2;
    Thing* n0 = 0; Thing* n1 = 0; Thing* n2 = 0;
    if (&median(n0, n1, d, c) != &n0) fails |= 4;   // comp(a,b) null; comp(a,c) = c: a
    if (&median(n0, n1, n2, c) != &n1) fails |= 8;  // all null: b
    if (fails) { std::printf("0095cec0_TEST FAIL %d\n", fails); return 1; }
    std::printf("0095cec0_TEST PASS\n");
    return 0;
}