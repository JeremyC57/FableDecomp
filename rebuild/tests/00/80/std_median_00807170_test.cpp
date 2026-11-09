// Behaviour test for std::__median<pair<C3DVector,long>, CCompareTargetedPositionsForFirstPersonTargeting> 0x00807170: returns
// a reference to the median of the three entries under comp(a, b) = score(a.first) > score(b.first) (best score first), with
// STLport's tie behaviour, for all six orderings. The score is a test stand-in for 0x007F9660.
#include <cstdio>

class C3DVector { public: float X, Y, Z; };
struct Entry { C3DVector first; long second; };
struct Comp {
    float AnchorX;
    float Score(const C3DVector& p) const { return p.X - AnchorX; }
    bool operator()(const Entry& a, const Entry& b) const { float av = Score(a.first); float bv = Score(b.first); return av > bv; }
};
static const Entry& median(const Entry& a, const Entry& b, const Entry& c, Comp comp)
{
    if (comp(a, b))
        if (comp(b, c))
            return b;
        else if (comp(a, c))
            return c;
        else
            return a;
    else if (comp(a, c))
        return a;
    else if (comp(b, c))
        return c;
    else
        return b;
}
static int fail(int n) { std::printf("00807170_TEST FAIL %d\n", n); return 1; }
int main()
{
    Entry e[3];
    for (int i = 0; i < 3; ++i) { e[i].first.X = (float)(i + 1); e[i].first.Y = e[i].first.Z = 0.0f; e[i].second = 100 + i; }
    Comp comp; comp.AnchorX = 0.5f;
    const int perm[6][3] = { {0,1,2}, {0,2,1}, {1,0,2}, {1,2,0}, {2,0,1}, {2,1,0} };
    for (int p = 0; p < 6; ++p) {
        const Entry& m = median(e[perm[p][0]], e[perm[p][1]], e[perm[p][2]], comp);
        if (&m != &e[1]) return fail(p + 1);
    }
    e[2].first.X = 2.0f;                               // tie between e[1] and e[2]
    const Entry& t = median(e[0], e[1], e[2], comp);
    if (&t != &e[1]) return fail(7);                   // !comp(a,b), !comp(a,c), !comp(b,c): the tie keeps b
    std::printf("00807170_TEST PASS\n");
    return 0;
}