// Behaviour test for CLipSyncSampleData::GetFrameInterpolate: the frame is floored and the fraction blends the phoneme weights (parked row 0x00A48F10).
#include <stdio.h>
#include "engine/GFFloatToLongFloor.h"
static float Blend(float a, float b, float time) { long f = GFFloatToLongFloor(time); float t = time - (float)f; return (1.0f - t) * a + t * b; }
int main()
{
    bool ok = Blend(0.0f, 1.0f, 3.25f) == 0.25f && Blend(2.0f, 4.0f, 7.5f) == 3.0f;
    printf(ok ? "00a48f10_TEST PASS\n" : "00a48f10_TEST FAIL\n");
    return ok ? 0 : 1;
}
