#include <cstdio>
// Upload flow: vertex size from the format (or 0x24 for repeating meshes), count = sum/product, buffer set up, filled, flag bit 0 set.
static unsigned int flagsAfter(unsigned int initFlags, bool setupOk) { if (!setupOk) return initFlags; return initFlags | 1; }
int main() {
    unsigned int counts[3] = { 4, 5, 6 }; unsigned int total = 0; for (int i = 0; i < 3; ++i) total += counts[i];
    long reps = 0; if (reps < 1) reps = 1; unsigned int staticCount = 10u * (unsigned int)reps;
    if (total != 15 || staticCount != 10 || flagsAfter(2, true) != 3 || flagsAfter(2, false) != 2) { std::printf("00a931f0_TEST FAIL\n"); return 1; }
    std::printf("00a931f0_TEST PASS\n"); return 0; }
