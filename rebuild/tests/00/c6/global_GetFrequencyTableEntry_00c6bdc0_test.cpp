#include <cstdio>
// behaviour test of GetFrequencyTableEntry (0x00C6BDC0): table[round(|octaves| / 3)][round((level + 90) / 6)]
#ifdef _MSC_VER
// libpk's inline-assembly truncation helper
__forceinline long FloatToLongTrunc(float value)
{
    long result;
    value = value - 0.5f;
    __asm fld value
    __asm fistp result
    if (result < 0)
        ++result;
    return result;
}
#else
#include <math.h>
// the truncation helper: round-to-nearest-even of (x - 0.5), +1 when negative == truncation toward zero for non-half values
inline long FloatToLongTrunc(float value)
{
    double v = (double)(value - 0.5f);
    double r = floor(v + 0.5);
    if (r - v == 0.5 && ((long)r & 1)) r -= 1.0;          // ties to even
    long result = (long)r;
    if (result < 0) ++result;
    return result;
}
#endif
static const float kOctaveScale = 1.0f / 3.0f;
static const float kMinLevel = -90.0f;
static const float kLevelScale = 1.0f / 6.0f;
unsigned char g_frequencyTable[4][31];
unsigned long GetFrequencyTableEntry(float octaves, float level)
{
    long row;
    if (octaves < 0.0f)
        row = FloatToLongTrunc(0.5f - octaves * kOctaveScale);
    else
        row = FloatToLongTrunc(octaves * kOctaveScale + 0.5f);
    long column = FloatToLongTrunc((level - kMinLevel) * kLevelScale + 0.5f);
    return g_frequencyTable[row][column];
}
int main()
{
    for (int r = 0; r < 4; ++r) for (int c = 0; c < 31; ++c) g_frequencyTable[r][c] = (unsigned char)(r * 40 + c);
    bool ok = GetFrequencyTableEntry(0.0f, -90.0f) == 0            // row 0, column 0
           && GetFrequencyTableEntry(3.0f, -90.0f) == 40           // row 1
           && GetFrequencyTableEntry(-3.0f, -90.0f) == 40          // the magnitude selects the row
           && GetFrequencyTableEntry(-7.0f, -90.0f) == 80          // 7/3 = 2.33 -> 2
           && GetFrequencyTableEntry(8.0f, -90.0f) == 120          // 8/3 = 2.67 -> 3
           && GetFrequencyTableEntry(0.0f, 90.0f) == 30            // (90 + 90) / 6 = 30
           && GetFrequencyTableEntry(0.0f, -84.0f) == 1
           && GetFrequencyTableEntry(0.0f, -81.0f) == 2            // 9 / 6 = 1.5 -> 2 (half up on the positive side)
           && GetFrequencyTableEntry(0.0f, -77.0f) == 2;           // 13 / 6 = 2.17 -> 2
    std::printf(ok ? "00c6bdc0_TEST PASS\n" : "00c6bdc0_TEST FAIL\n");
    return ok ? 0 : 1;
}