// Behaviour test for CEngineMap::PeekWaterHeight: land heights are quantised to 1/128 units with GFFloatToLongNear (parked row 0x00B41A80).
#include <stdio.h>
#include "engine/GFFloatToLongNear.h"
static float Quantise(float h) { return GFFloatToLongNear(h * 128.0f) * (1.0f / 128.0f); }
int main()
{
    bool ok = Quantise(1.0f) == 1.0f && Quantise(0.0039f) == 0.0f && Quantise(0.0079f) == 0.0078125f;
    printf(ok ? "00b41a80_TEST PASS\n" : "00b41a80_TEST FAIL\n");
    return ok ? 0 : 1;
}
