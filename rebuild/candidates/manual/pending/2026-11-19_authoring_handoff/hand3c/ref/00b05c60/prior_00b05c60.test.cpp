// Behaviour test for NParticleEngine::CPSCSingleSprite::GetComponentCost: render sizes are quantised to two decimals with GFFloatToLongNear (parked row 0x00B05C60).
#include <stdio.h>
#include "engine/GFFloatToLongNear.h"
static float Round2(float v) { float s = 100.0f; return GFFloatToLongNear(v * s) / s; }
int main()
{
    bool ok = Round2(1.234f) == 1.23f && Round2(0.5f) == 0.5f && Round2(19.999f) == 20.0f;
    printf(ok ? "00b05c60_TEST PASS\n" : "00b05c60_TEST FAIL\n");
    return ok ? 0 : 1;
}
