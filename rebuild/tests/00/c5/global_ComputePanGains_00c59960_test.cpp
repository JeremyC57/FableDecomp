// Behaviour test for ComputePanGains (0x00C59960): linear pan law, ignored in output mode 2.
#include <cstdio>
static long g_mode = 0;
static void ComputePanGains(float pan, float* left, float* right)
{
    if (g_mode == 2)
        pan = 0.0f;
    if (pan > 0.0f) {
        *left = 1.0f - pan;
        *right = 1.0f;
    } else {
        *left = 1.0f;
        *right = pan + 1.0f;
    }
}
static bool Near(float a, float b) { float d = a - b; return d < 1e-6f && d > -1e-6f; }
int main()
{
    float l = 0, r = 0;
    ComputePanGains(0.25f, &l, &r); int ok = Near(l, 0.75f) && Near(r, 1.0f);
    ComputePanGains(-0.5f, &l, &r); ok = ok && Near(l, 1.0f) && Near(r, 0.5f);
    ComputePanGains(0.0f, &l, &r); ok = ok && Near(l, 1.0f) && Near(r, 1.0f);
    g_mode = 2; ComputePanGains(0.9f, &l, &r); ok = ok && Near(l, 1.0f) && Near(r, 1.0f);
    if (!ok) { std::printf("00c59960_TEST FAIL\n"); return 1; }
    std::printf("00c59960_TEST PASS\n");
    return 0;
}