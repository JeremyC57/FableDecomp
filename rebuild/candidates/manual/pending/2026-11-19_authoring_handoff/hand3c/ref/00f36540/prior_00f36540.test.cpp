// Behaviour test for NFractal::CPerlinNoise::GetGaussian2DNoise: lattice coordinates are floored (GFFloatToLongFloor), the fraction is in [0,1) (parked row 0x00F36540).
#include <stdio.h>
#include "engine/GFFloatToLongFloor.h"
static float Frac(float x) { return x - (float)GFFloatToLongFloor(x); }
int main()
{
    bool ok = GFFloatToLongFloor(2.75f) == 2 && GFFloatToLongFloor(-0.25f) == -1 && GFFloatToLongFloor(3.0f) == 3 && Frac(1.25f) == 0.25f;
    printf(ok ? "00f36540_TEST PASS\n" : "00f36540_TEST FAIL\n");
    return ok ? 0 : 1;
}
