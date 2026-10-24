// Behaviour test for 0x009E2460 GFGet2DDistance(const C2DVector&, const C2DVector&).
#include <cstdio>
#include <math.h>

struct C2DVector { float X; float Y; };

static float GFGet2DDistance(const C2DVector& a, const C2DVector& b)
{
    float dx = a.X - b.X;
    float dy = a.Y - b.Y;
    return (float)sqrt(dx * dx + dy * dy);
}

static bool Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }

int main()
{
    C2DVector a = { 3.0f, 4.0f }, o = { 0.0f, 0.0f }, p = { 1.5f, -2.0f }, q = { -1.5f, 2.0f };
    bool ok = Near(GFGet2DDistance(a, o), 5.0f) && Near(GFGet2DDistance(o, a), 5.0f)
           && Near(GFGet2DDistance(p, p), 0.0f) && Near(GFGet2DDistance(p, q), 5.0f);
    if (!ok) { std::printf("009e2460_TEST FAIL\n"); return 1; }
    std::printf("009e2460_TEST PASS\n");
    return 0;
}