#include <cstdio>
// Behaviour model: a changed value sets the dirty bit, an unchanged one leaves the flags alone.
static float stored = 0.0f; static unsigned long dirty = 0;
static long setv(float v) { if (v == stored) return 0; stored = v; return 1; }
static void Set(float v) { if (setv(v)) dirty |= 4; }
int main()
{
    Set(0.0f);
    if (dirty) { std::printf("00c20eb0_TEST FAIL\n"); return 1; }
    Set(0.5f);
    if (dirty != 4 || stored != 0.5f) { std::printf("00c20eb0_TEST FAIL\n"); return 1; }
    std::printf("00c20eb0_TEST PASS\n");
    return 0;
}