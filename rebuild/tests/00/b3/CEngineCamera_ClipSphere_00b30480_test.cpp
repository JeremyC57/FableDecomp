// Behaviour test for 0x00B30480 CEngineCamera::ClipSphere(const C3DVector&, float, unsigned long*) const
#include <cstdio>

struct V3 { float X, Y, Z; };
struct Plane { V3 N; float D; };
struct Camera { V3 Pos; Plane P[4]; };
static float Dot(const V3& a, const V3& b) { return a.X * b.X + a.Y * b.Y + a.Z * b.Z; }
static bool ClipSphere(const Camera& c, const V3& centre, float radius, unsigned long* clipFlags)   // the row's body
{
    unsigned long flags = *clipFlags;
    if (flags & 0x40) return false;
    if ((flags & 0x3C) == 0x3C) return true;
    V3 d = { centre.X - c.Pos.X, centre.Y - c.Pos.Y, centre.Z - c.Pos.Z };
    static const int order[4] = { 2, 3, 0, 1 };
    for (int k = 0; k < 4; ++k) {
        unsigned long bit = 4ul << order[k];
        if (!(*clipFlags & bit)) {
            float distance = Dot(d, c.P[order[k]].N);
            if (distance > radius) { *clipFlags |= 0x40; return false; }
            if (distance < -radius) *clipFlags |= bit;
        }
    }
    return true;
}

int main()
{
    int ok = 1;
    const float k = 0.70710678f;
    Camera c = { {0, 0, 0}, { {{k, 0, -k}, 0}, {{-k, 0, -k}, 0}, {{0, k, -k}, 0}, {{0, -k, -k}, 0} } };
    V3 ahead = { 0, 0, 10 }, left = { -50, 0, 10 }, edge = { 12, 0, 10 };
    unsigned long f = 0;
    ok &= ClipSphere(c, ahead, 1.0f, &f) && f == 0x3C;                 // wholly inside all four planes
    f = 0;
    ok &= !ClipSphere(c, left, 1.0f, &f) && (f & 0x40);                 // outside: the 0x40 flag
    ok &= !ClipSphere(c, ahead, 1.0f, &f);                              // already outside: no test
    f = 0;
    ok &= ClipSphere(c, edge, 2.0f, &f) && !(f & 0x04) && (f & 0x38) == 0x38;   // straddles plane 0 only
    f = 0x3C;
    ok &= ClipSphere(c, left, 1.0f, &f) && f == 0x3C;                   // all planes passed before: trivially in
    if (ok) std::printf("00b30480_TEST PASS\n"); else std::printf("00b30480_TEST FAIL\n");
    return ok ? 0 : 1;
}