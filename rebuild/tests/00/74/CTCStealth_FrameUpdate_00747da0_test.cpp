// behaviour test for 0x00747DA0 CTCStealth::FrameUpdate(): the four-second deed threshold (ceiling of fps * 4 through the
// engine's FPU helper: round(x + 0.5), one back when x was integral) and the keep-sneaking decision.
#include <cstdio>
#include <cmath>
static long CeilToLong(float value)
{
    float x = value + 0.5f;
    float f = (float)std::floor(x);                      // fistp: round to nearest (even) in the default FPU mode
    long result = (long)f;
    if (x - f > 0.5f || (x - f == 0.5f && (result & 1))) ++result;
    float back = (float)result - 1.0f;
    if (back == value) --result;                         // x was integral: the half rounded up
    return result;
}
static bool KeepSneaking(bool toggleMode, bool toggleSet, bool hero, long heroState, bool option, float radiusSq,
                         float speedSq, bool canSneak)
{
    if (toggleMode) { if (!toggleSet) canSneak = false; }
    else if (hero) { if (heroState == 3 || (heroState == 4 && !option)) canSneak = false; }
    else if (radiusSq - 1e-4f <= speedSq) canSneak = false;
    return canSneak;
}
int main()
{
    bool ok = CeilToLong(30.0f * 4.0f) == 120 && CeilToLong(7.25f) == 8 && CeilToLong(7.5f) == 8
        && CeilToLong(7.75f) == 8 && CeilToLong(8.0f) == 8;
    ok = ok && KeepSneaking(true, true, false, 0, false, 0, 0, true) && !KeepSneaking(true, false, false, 0, false, 0, 0, true)
        && !KeepSneaking(false, false, true, 3, true, 0, 0, true) && KeepSneaking(false, false, true, 4, true, 0, 0, true)
        && !KeepSneaking(false, false, true, 4, false, 0, 0, true) && KeepSneaking(false, false, false, 0, false, 4.0f, 1.0f, true)
        && !KeepSneaking(false, false, false, 0, false, 1.0f, 4.0f, true) && !KeepSneaking(true, true, false, 0, false, 0, 0, false);
    if (!ok) { std::printf("00747da0_TEST FAIL\n"); return 1; }
    std::printf("00747da0_TEST PASS\n");
    return 0;
}