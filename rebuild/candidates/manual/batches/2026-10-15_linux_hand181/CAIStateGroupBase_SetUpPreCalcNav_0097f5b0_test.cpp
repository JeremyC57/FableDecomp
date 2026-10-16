// behaviour test for CAIStateGroupBase::SetUpPreCalcNav (0x0097F5B0)
#include <cstdio>
struct V { float X, Y, Z; };
struct Route { bool has; V start, dest; };
struct Nav { int setups; V from, start, dest; void SetUp(const V& f, const V& s, const V& d) { ++setups; from = f; start = s; dest = d; } };
struct Thing { V pos; Nav* nav; Route* route; };
static V SetUp(Thing* creature, Thing* routeThing)
{
    if (creature->nav) {
        Route* r = routeThing->route;
        if (r && r->has) { creature->nav->SetUp(creature->pos, r->start, r->dest); return r->dest; }
    }
    V z = { 0, 0, 0 }; return z;
}
int main()
{
    Nav n = { 0 }; Route r = { true, { 1, 2, 3 }, { 7, 8, 9 } };
    Thing c = { { 4, 5, 6 }, &n, 0 }; Thing rt = { { 0, 0, 0 }, 0, &r };
    V d = SetUp(&c, &rt);
    bool ok = d.X == 7 && d.Z == 9 && n.setups == 1 && n.from.Y == 5 && n.start.Z == 3;
    r.has = false; d = SetUp(&c, &rt); ok = ok && d.X == 0 && n.setups == 1;
    if (!ok) { std::printf("0097f5b0_TEST FAIL\n"); return 1; }
    std::printf("0097f5b0_TEST PASS\n");
    return 0;
}
