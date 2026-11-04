// Behaviour test for 0x00969E70 CAIStateGroup_SleepOnFloor::IsRunnable(): a model. The sleep points in the 40x40 box round the
// creature's X/Y (half-size 20) that are this creature's sleep points and not occupied are gathered; with none the group is not
// runnable and the latched point is unchanged; otherwise one is picked with the world seed step (seed*0x24A1+0x24DF, rotated
// right 13, modulo the count) and latched.
#include <cstdio>
#include <vector>

struct Point { float x, y; int owner; bool occupied; };
static unsigned long Ror13(unsigned long v) { return (v >> 13) | (v << 19); }
static unsigned long Random(unsigned long range, unsigned long* seed)
{
    *seed = *seed * 0x24A1 + 0x24DF;
    *seed = Ror13(*seed);
    return range ? *seed % range : 0;
}
static bool IsRunnable(const std::vector<Point>& world, float cx, float cy, int me, unsigned long* seed, const Point** latched)
{
    std::vector<const Point*> points;
    for (size_t i = 0; i < world.size(); ++i) {
        const Point& p = world[i];
        if (p.x >= cx - 20.0f && p.x <= cx + 20.0f && p.y >= cy - 20.0f && p.y <= cy + 20.0f && p.owner == me && !p.occupied)
            points.push_back(&p);
    }
    if (points.empty())
        return false;
    *latched = points[Random(points.size(), seed)];
    return true;
}

int main()
{
    int ok = 1;
    std::vector<Point> w;
    Point a = { 5, 5, 1, false }, b = { 100, 0, 1, false }, c = { 0, 0, 2, false }, d = { -10, 12, 1, true }, e = { 15, -19, 1, false };
    w.push_back(a); w.push_back(b); w.push_back(c); w.push_back(d); w.push_back(e);
    unsigned long seed = 1234;
    const Point* latched = 0;
    ok &= !IsRunnable(w, 500, 500, 1, &seed, &latched) && latched == 0 && seed == 1234;
    ok &= !IsRunnable(w, 0, 0, 3, &seed, &latched) && latched == 0;
    unsigned long s2 = 1234;
    unsigned long idx = Random(2, &s2);
    ok &= IsRunnable(w, 0, 0, 1, &seed, &latched) && seed == s2 && latched == (idx == 0 ? &w[0] : &w[4]);
    ok &= IsRunnable(w, 100, 0, 1, &seed, &latched) && latched == &w[1];
    std::printf(ok ? "00969E70_TEST PASS\n" : "00969E70_TEST FAIL\n");
    return ok ? 0 : 1;
}