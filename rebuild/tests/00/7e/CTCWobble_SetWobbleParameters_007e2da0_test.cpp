#include <cstdio>
#ifdef _MSC_VER
#pragma optimize("y", off)
#include "engine/GFFloatToLongCeil.h"
#else
#include <math.h>
inline long GFFloatToLongCeil(float value) { return (long)ceil((double)value); }   // the FPU block is ceil() for the magnitudes used here
#endif
long g_FramesPerSecond = 15;
class CTCWobble { public:
    unsigned char _pad00[0x24]; long FramesOfWobbleRemaining; long DirectionLastRotated; float AngleOfWobble;
    void SetWobbleParameters(float seconds, float angle) { AngleOfWobble = angle; FramesOfWobbleRemaining = GFFloatToLongCeil(g_FramesPerSecond * seconds); }
};
int main()
{
    CTCWobble w; w.FramesOfWobbleRemaining = -1; w.AngleOfWobble = 0.0f;
    w.SetWobbleParameters(2.1f, 0.25f);
    bool ok = w.FramesOfWobbleRemaining == 32 && w.AngleOfWobble == 0.25f;
    w.SetWobbleParameters(1.0f, -3.0f); ok = ok && w.FramesOfWobbleRemaining == 15 && w.AngleOfWobble == -3.0f;
    std::printf(ok ? "007e2da0_TEST PASS\n" : "007e2da0_TEST FAIL\n");
    return ok ? 0 : 1;
}