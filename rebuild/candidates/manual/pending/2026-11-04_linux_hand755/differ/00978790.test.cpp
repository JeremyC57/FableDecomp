// Behaviour test for (anonymous namespace)::CThingCompare_InDirectionOfAndNearestToHeuristicBitch::operator() (0x00978790), g++ model of the parked source.
#include <cstdio>
#include <cmath>
struct V2 { float X, Y; };
struct Cmp {
    V2 Pos, Dir;
    float Weight(float dot, float distance) const { return (float)std::pow(dot, (double)((long)distance / 4 * 2 + 1)); }
    float Of(V2 p) const { V2 o = { p.X - Pos.X, p.Y - Pos.Y }; float l = std::sqrt(o.X * o.X + o.Y * o.Y); float r = 1.0f / l; o.X *= r; o.Y *= r; return Weight(o.X * Dir.X + o.Y * Dir.Y, l); }
    bool operator()(V2 a, V2 b) const { float wa = Of(a); return Of(b) < wa; }
};
int main()
{
    Cmp c = { { 0, 0 }, { 1, 0 } };
    V2 ahead = { 2, 0 }, side = { 0, 2 }, farAhead = { 9, 0.5f };
    int ok = c(ahead, side) && !c(side, ahead);
    ok &= std::fabs(c.Weight(0.5f, 5.0f) - 0.125f) < 1e-6f;   // (5/4)*2+1 = 3
    ok &= c(ahead, farAhead) == (c.Of(farAhead) < c.Of(ahead));
    std::printf(ok ? "00978790_TEST PASS\n" : "00978790_TEST FAIL\n");
    return ok ? 0 : 1;
}
