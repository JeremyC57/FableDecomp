// Behaviour test for 0x00771C70 CTCHeroPodium::GetPositionForMember(member) const
#include <cstdio>
#include <math.h>

struct V { float X, Y, Z; };

static V PositionForMember(long member, const V& ex, const V& ey, const V& pos)
{
    const float PI = 3.14159265f;
    float fraction = (float)member / 8.0f;
    float angle = (float)((fraction * 0.5f - 0.25f) * (2.0 * PI));
    float c = (float)cos(angle), s = (float)sin(angle);
    V r;
    r.X = (ey.X * c + ex.X * s) * 4.0f + pos.X;
    r.Y = (ey.Y * c + ex.Y * s) * 4.0f + pos.Y;
    r.Z = (ey.Z * c + ex.Z * s) * 4.0f + pos.Z;
    return r;
}

static bool Near(float a, float b) { float d = a - b; return d < 0.001f && d > -0.001f; }

int main()
{
    V ex = { 1, 0, 0 }, ey = { 0, 1, 0 }, pos = { 10, 20, 5 };
    V p0 = PositionForMember(0, ex, ey, pos);          // angle -pi/2: sin -1, cos 0 -> -4 along E_X
    if (!Near(p0.X, 6.0f) || !Near(p0.Y, 20.0f) || !Near(p0.Z, 5.0f)) { std::printf("00771c70_TEST FAIL\n"); return 1; }
    V p4 = PositionForMember(4, ex, ey, pos);          // angle 0: cos 1 -> +4 along E_Y
    if (!Near(p4.X, 10.0f) || !Near(p4.Y, 24.0f)) { std::printf("00771c70_TEST FAIL\n"); return 1; }
    V p8 = PositionForMember(8, ex, ey, pos);          // angle pi/2: +4 along E_X
    if (!Near(p8.X, 14.0f) || !Near(p8.Y, 20.0f)) { std::printf("00771c70_TEST FAIL\n"); return 1; }
    V p16 = PositionForMember(16, ex, ey, pos);        // a full turn after member 0
    if (!Near(p16.X, p0.X) || !Near(p16.Y, p0.Y)) { std::printf("00771c70_TEST FAIL\n"); return 1; }
    std::printf("00771c70_TEST PASS\n");
    return 0;
}