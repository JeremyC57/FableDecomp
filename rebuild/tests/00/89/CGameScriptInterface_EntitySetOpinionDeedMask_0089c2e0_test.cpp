#include <cstdio>
// Behaviour model: the mask reaches the component only when the thing exists and has it.
static long got = -1;
static void setMask(void* thing, bool hasTC, long mask) { if (thing && hasTC) got = mask; }
int main()
{
    int t;
    setMask(0, true, 5); if (got != -1) { std::printf("0089c2e0_TEST FAIL\n"); return 1; }
    setMask(&t, false, 5); if (got != -1) { std::printf("0089c2e0_TEST FAIL\n"); return 1; }
    setMask(&t, true, 7); if (got != 7) { std::printf("0089c2e0_TEST FAIL\n"); return 1; }
    std::printf("0089c2e0_TEST PASS\n");
    return 0;
}