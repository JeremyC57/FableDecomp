// Behaviour test for 0x00876E10 NCameraModes::CTools::PotentialFieldBufferPoints(pos, points, strength, radius) const
// The model mirrors the source: the summed potential (strength * 0.1) / (distance - radius) of every buffer point is
// sampled at pos and one step (0.1) along each axis; the forward differences over the step form the gradient.
#include <cstdio>
#include <cmath>
#include <list>

struct V3 { float X, Y, Z; V3(float x, float y, float z) : X(x), Y(y), Z(z) { } };
static float g_step = 0.1f;
static const V3 g_offsets[4] = { V3(0, 0, 0), V3(1, 0, 0), V3(0, 1, 0), V3(0, 0, 1) };

static float Field(const V3& p, const V3& s, float strength, float radius)
{
    float dx = p.X - s.X, dy = p.Y - s.Y, dz = p.Z - s.Z;
    return (strength * 0.1f) / ((float)std::sqrt(dx * dx + dy * dy + dz * dz) - radius);
}
static V3 Gradient(const V3& pos, const std::list<V3>& points, float strength, float radius)
{
    float potential[4] = { 0, 0, 0, 0 };
    for (std::list<V3>::const_iterator it = points.begin(); it != points.end(); ++it)
        for (int i = 0; i < 4; ++i)
            potential[i] += Field(V3(pos.X + g_offsets[i].X * g_step, pos.Y + g_offsets[i].Y * g_step,
                                     pos.Z + g_offsets[i].Z * g_step), *it, strength, radius);
    return V3((potential[1] - potential[0]) / g_step, (potential[2] - potential[0]) / g_step, (potential[3] - potential[0]) / g_step);
}
static bool Near(float a, float b) { return std::fabs(a - b) < 1e-3f * (1.0f + std::fabs(b)); }

int main()
{
    int fail = 0;
    std::list<V3> none;
    V3 g0 = Gradient(V3(1, 2, 3), none, 5.0f, 0.5f);
    if (g0.X != 0 || g0.Y != 0 || g0.Z != 0) fail |= 1;                // no points: zero gradient
    std::list<V3> one; one.push_back(V3(0, 0, 0));
    V3 g = Gradient(V3(10, 0, 0), one, 10.0f, 1.0f);                     // field 1 / (d - 1): falls off along +X
    float want = (1.0f / (10.1f - 1.0f) - 1.0f / 9.0f) / 0.1f;
    if (!Near(g.X, want) || !(g.X < 0) || std::fabs(g.Y) > 1e-3f || std::fabs(g.Z) > 1e-3f) fail |= 2;
    std::list<V3> two; two.push_back(V3(-5, 0, 0)); two.push_back(V3(5, 0, 0));
    V3 s = Gradient(V3(0, 0, 0), two, 10.0f, 1.0f);                      // symmetric pair: X differences cancel to first order
    if (std::fabs(s.X) > 0.01f || std::fabs(s.Y) > 0.01f || std::fabs(s.Z) > 0.01f) fail |= 4;
    if (fail) { std::printf("00876e10_TEST FAIL %d\n", fail); return 1; }
    std::printf("00876e10_TEST PASS\n");
    return 0;
}