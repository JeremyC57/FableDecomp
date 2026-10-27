#include <cstdio>
// behaviour test of SecondsToMixBlocks (0x00C59B00): seconds * 44100 / block frames, truncated toward zero
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
typedef __int64 int64;
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
typedef long long int64;
#endif
static const int64 kDefaultBlockFrames = 128;
unsigned long g_useBlockFrames = 0;
unsigned long g_blockFrames = 0;
int64 g_performanceFrequency = 3579545;
long SecondsToMixBlocks(float seconds)
{
    float blockFrames = g_useBlockFrames ? (float)g_blockFrames : (float)kDefaultBlockFrames;
    unsigned long rate = (unsigned long)(g_performanceFrequency * 44100 / g_performanceFrequency);
    float samples = (float)rate * seconds;
    return FloatToLongTrunc(samples / blockFrames);
}
int main()
{
    bool ok = SecondsToMixBlocks(1.0f) == 344                 // 44100 / 128 = 344.53
           && SecondsToMixBlocks(0.0f) == 0
           && SecondsToMixBlocks(-1.0f) == -344
           && SecondsToMixBlocks(0.01f) == 3;                 // 441 / 128 = 3.44
    g_useBlockFrames = 1; g_blockFrames = 441;
    ok = ok && SecondsToMixBlocks(1.0f) == 100 && SecondsToMixBlocks(0.5f) == 50 && SecondsToMixBlocks(-0.25f) == -25;
    std::printf(ok ? "00c59b00_TEST PASS\n" : "00c59b00_TEST FAIL\n");
    return ok ? 0 : 1;
}