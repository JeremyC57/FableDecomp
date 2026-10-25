// Behaviour test for 00c1cab0: Atan2(x, y) -- the angle of (x, y) in (-pi, pi], octant by octant through atan of the smaller over the larger component
#include <cstdio>
#include <math.h>

static float Atan2(float x, float y)
{
    if (x >= y && -y <= x) return (float)atan(y / x);
    if (y >= x && -x <= y) return 1.5707964f - (float)atan(x / y);
    if (-y >= x && x < y) {
        if (y >= 0.0f) return (float)atan(y / x) + 3.1415927f;
        return (float)atan(y / x) - 3.1415927f;
    }
    return -1.5707964f - (float)atan(x / y);
}

static bool Near(float a, float b) { float d = a - b; return d < 0.0005f && d > -0.0005f; }

int main()
{
    float xs[12] = { 1, 1, 0, -1, -1, -1, 0, 1, 3, -2, 0.5f, -4 };
    float ys[12] = { 0, 1, 1, 1, 0.0001f, -1, -1, -1, 2, -5, -3, 3 };
    for (int i = 0; i < 12; ++i) {
        float want = (float)atan2((double)ys[i], (double)xs[i]);
        if (!Near(Atan2(xs[i], ys[i]), want)) { std::printf("00c1cab0_TEST FAIL\n"); return 1; }
    }
    if (!Near(Atan2(-1, 0), 3.1415927f)) { std::printf("00c1cab0_TEST FAIL\n"); return 1; }   // y >= 0 on the negative x axis: +pi
    std::printf("00c1cab0_TEST PASS\n");
    return 0;
}