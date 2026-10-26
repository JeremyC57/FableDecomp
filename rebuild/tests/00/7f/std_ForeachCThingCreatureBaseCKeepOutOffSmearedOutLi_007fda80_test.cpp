#include <cstdio>
// Behaviour test: std::_For_each<CThingCreatureBase**, CKeepOutOffSmearedOutLine> -- applies the keep-out functor to every creature in the range and returns a copy of it.
#define CHECK(c) if (!(c)) { std::printf("007fda80_TEST FAIL\n"); return 1; }
class C3DVector { public: float X, Y, Z; };
class CThing { public: int pushed; };
class CThingCreatureBase : public CThing { public: };
static const CThing* g_order[8]; static int g_n = 0;
class CKeepOutOffSmearedOutLine { public:
    C3DVector* StartPos; C3DVector* EndPos; float Radius;
    CKeepOutOffSmearedOutLine(const C3DVector& s, const C3DVector& e, float r) : StartPos((C3DVector*)&s), EndPos((C3DVector*)&e), Radius(r) { }
    void operator()(CThing* thing) const { ++thing->pushed; g_order[g_n++] = thing; }
};
template <class It, class F> F ForEach(It first, It last, F func) { for (; first != last; ++first) func(*first); return func; }
int main()
{
    C3DVector s = { 0, 0, 0 }, e = { 1, 0, 0 };
    CThingCreatureBase a, b, c; a.pushed = b.pushed = c.pushed = 0;
    CThingCreatureBase* list[3] = { &a, &b, &c };
    CKeepOutOffSmearedOutLine r = ForEach(list, list + 3, CKeepOutOffSmearedOutLine(s, e, 2.0f));
    CHECK(a.pushed == 1 && b.pushed == 1 && c.pushed == 1 && g_n == 3 && g_order[0] == &a && g_order[2] == &c)
    CHECK(r.StartPos == &s && r.EndPos == &e && r.Radius == 2.0f)
    ForEach(list, list, r); CHECK(g_n == 3)
    std::printf("007fda80_TEST PASS\n");
    return 0;
}