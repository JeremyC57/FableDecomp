// Behaviour test for C3DAnimationSequenceData::GetKeyframeInterpolate: the slerp table index is GFFloatToLongNear(x - 0.5) (a floor for x >= 0) (parked row 0x00A4C1F0).
#include <stdio.h>
#include "engine/GFFloatToLongNear.h"
static long Index(float c) { float x = (c + 1.0f) * 512.5f; return GFFloatToLongNear(x - 0.5f); }
int main()
{
    bool ok = Index(-1.0f) == 0 && Index(1.0f) == 1024 && Index(0.0f) == 512;
    printf(ok ? "00a4c1f0_TEST PASS\n" : "00a4c1f0_TEST FAIL\n");
    return ok ? 0 : 1;
}
