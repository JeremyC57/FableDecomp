// Behaviour test for CEngineSceneMap::Update (0x00B947E0): the row's logic on stand-alone types, rounding through the shared
// GFFloatToLongNear (retail asm under VC7.1, its FABLE_PORTABLE twin elsewhere) and the shared GFGetTopBit / GFLimit.
#ifndef _MSC_VER
#define FABLE_PORTABLE
#endif
#include <stdio.h>
#include <math.h>
#include "engine/GFFloatToLongNear.h"
// Copies of the shared header helpers (their headers carry the 32-bit ABI guard; the test also runs on 64-bit hosts).
static unsigned long GFGetTopBit(unsigned long value)
{
    if (value == 0) return (unsigned long)-1;
    if (value >= 0x80000000) return 31;
    ++value;
    long r = ((long)((1 << 16) - value) >> 31) & 16;
    r += ((long)((1 << (r + 8)) - value) >> 31) & 8;
    r += ((long)((1 << (r + 4)) - value) >> 31) & 4;
    r += ((long)((1 << (r + 2)) - value) >> 31) & 2;
    r += ((long)((1 << (r + 1)) - value) >> 31) & 1;
    return r;
}
template<class T, class L, class U> static void GFLimit(T& value, const L& lower, const U& upper)
{ if (value < lower) value = lower; else if (value > upper) value = upper; }
struct Map {
    unsigned long total, MaxLevels, currentLevels, resizedTo; int resizeCalls; float perCell;
    void ResizeLevels(long n) { resizedTo = (unsigned long)n; currentLevels = (unsigned long)n; ++resizeCalls; }
    void Update() {
        unsigned long total_count = total;
        long new_size = GFFloatToLongNear((float)sqrt(total_count / perCell)) * 2 - 1;
        unsigned long new_levels = GFGetTopBit(new_size > 1 ? new_size : 1) + 1;
        GFLimit(new_levels, 1UL, MaxLevels);
        if (new_levels != currentLevels) ResizeLevels(new_levels);
    }
};
static unsigned long Run(unsigned long total, float perCell, unsigned long maxLevels, unsigned long current, int* calls)
{
    Map m = { total, maxLevels, current, 0, 0, perCell };
    m.Update(); *calls = m.resizeCalls; return m.currentLevels;
}
int main()
{
    int fails = 0, c;
    if (Run(0, 4.0f, 8, 1, &c) != 1 || c != 0) ++fails;            // empty: size -1 -> clamped 1 -> 1 level, unchanged: no resize
    if (Run(64, 4.0f, 8, 1, &c) != 3 || c != 1) ++fails;           // side 4 -> 7 -> top bit 2 -> 3 levels
    if (Run(100, 4.0f, 8, 1, &c) != 4 || c != 1) ++fails;          // side 5 -> 9 -> top bit 3 -> 4 levels
    if (Run(1000000, 1.0f, 6, 2, &c) != 6 || c != 1) ++fails;      // 1000 -> 1999 -> bit 10 -> 11, limited to MaxLevels 6
    if (Run(9, 4.0f, 8, 2, &c) != 2 || c != 0) ++fails;            // sqrt(2.25)=1.5 -> round to even 2 -> 3 -> bit 1 -> 2 levels (unchanged)
    if (Run(25, 4.0f, 8, 9, &c) != 2 || c != 1) ++fails;           // sqrt(6.25)=2.5 -> 2 (ties to even) -> 3 -> 2 levels
    if (Run(1, 4.0f, 8, 3, &c) != 1 || c != 1) ++fails;            // sqrt(0.25)=0.5 -> 0 -> -1 -> clamped 1 -> 1 level
    printf(fails ? "00b947e0_TEST FAIL\n" : "00b947e0_TEST PASS\n");
    return fails ? 1 : 0;
}