// Behaviour test for 0x009E2430 GFGet2DDistance(const C2DCoordI&, const C2DCoordI&).
#include <cstdio>
#include <math.h>

struct C2DCoordI { int X; int Y; };

static float GFGet2DDistance(const C2DCoordI& a, const C2DCoordI& b)
{
    int dx = a.X - b.X;
    int dy = a.Y - b.Y;
    return (float)sqrt((double)(dx * dx + dy * dy));
}

static bool Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }

int main()
{
    C2DCoordI a = { 3, 4 }, o = { 0, 0 }, p = { -2, 7 }, q = { 10, -5 };
    bool ok = Near(GFGet2DDistance(a, o), 5.0f) && Near(GFGet2DDistance(o, a), 5.0f)
           && Near(GFGet2DDistance(a, a), 0.0f) && Near(GFGet2DDistance(p, q), 16.970563f);
    if (!ok) { std::printf("009e2430_TEST FAIL\n"); return 1; }
    std::printf("009e2430_TEST PASS\n");
    return 0;
}