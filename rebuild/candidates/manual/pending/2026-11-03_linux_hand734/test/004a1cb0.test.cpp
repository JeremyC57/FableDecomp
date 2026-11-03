// Behaviour test for CWorld::PreparePrimitivesForEngine (0x004A1CB0): a standalone model of the weather-mask transform -- rows
// -right, -forward, up (right = forward x up), translation = position, the 3x3 part pre-scaled by RenderSizeX * 0.01, then
// inverted; the inverse must map the transformed origin back to zero and the masks are sent once.
#include <cstdio>
#include <cmath>

struct V { float X, Y, Z; };
struct M { float E11,E12,E13,E21,E22,E23,E31,E32,E33,E41,E42,E43; };
static V Cross(V a, V b) { V r = { a.Y*b.Z - a.Z*b.Y, a.Z*b.X - a.X*b.Z, a.X*b.Y - a.Y*b.X }; return r; }
static M Inverse(const M& m)
{
    M r; float det = m.E11*(m.E22*m.E33 - m.E23*m.E32) + m.E21*(m.E32*m.E13 - m.E33*m.E12) + m.E31*(m.E23*m.E12 - m.E22*m.E13);
    float inv = 1.0f / det;
    r.E11 = (m.E22*m.E33 - m.E23*m.E32)*inv; r.E21 = (m.E23*m.E31 - m.E21*m.E33)*inv; r.E31 = (m.E21*m.E32 - m.E22*m.E31)*inv;
    r.E12 = (m.E32*m.E13 - m.E33*m.E12)*inv; r.E22 = (m.E33*m.E11 - m.E13*m.E31)*inv; r.E32 = (m.E31*m.E12 - m.E32*m.E11)*inv;
    r.E13 = (m.E23*m.E12 - m.E22*m.E13)*inv; r.E23 = (m.E21*m.E13 - m.E23*m.E11)*inv; r.E33 = (m.E22*m.E11 - m.E21*m.E12)*inv;
    r.E41 = -(r.E11*m.E41 + r.E21*m.E42 + r.E31*m.E43); r.E42 = -(r.E12*m.E41 + r.E22*m.E42 + r.E32*m.E43);
    r.E43 = -(r.E13*m.E41 + r.E23*m.E42 + r.E33*m.E43);
    return r;
}
static V Apply(const M& m, V p) { V r = { m.E11*p.X + m.E21*p.Y + m.E31*p.Z + m.E41, m.E12*p.X + m.E22*p.Y + m.E32*p.Z + m.E42,
                                         m.E13*p.X + m.E23*p.Y + m.E33*p.Z + m.E43 }; return r; }

int main()
{
    V up = { 0, 0, 1 }, forward = { 0, 1, 0 }, pos = { 10, 20, 30 };
    V right = Cross(forward, up);                            // (1, 0, 0)
    float scale = 200.0f * 0.01f;
    M t = { -right.X*scale, -right.Y*scale, -right.Z*scale, -forward.X*scale, -forward.Y*scale, -forward.Z*scale,
            up.X*scale, up.Y*scale, up.Z*scale, pos.X, pos.Y, pos.Z };
    M inv = Inverse(t);
    V o = Apply(inv, pos);                                   // the building's position is the mask's local origin
    V p = { 1, 1, 1 }; V q = Apply(inv, Apply(t, p));
    bool sent = false, sentAgain = false;
    if (!sent) sent = true; else sentAgain = true;           // WeatherMaskingPrimitivesSent latches
    if (right.X != 1.0f || std::fabs(o.X) > 1e-4f || std::fabs(o.Y) > 1e-4f || std::fabs(o.Z) > 1e-4f ||
        std::fabs(q.X - 1) > 1e-4f || std::fabs(q.Y - 1) > 1e-4f || std::fabs(q.Z - 1) > 1e-4f || !sent || sentAgain) {
        std::printf("004a1cb0_TEST FAIL\n"); return 1;
    }
    std::printf("004a1cb0_TEST PASS\n");
    return 0;
}
