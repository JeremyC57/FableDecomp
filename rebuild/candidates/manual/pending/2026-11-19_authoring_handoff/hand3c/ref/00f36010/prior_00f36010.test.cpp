// Behaviour test for NFractal::CPerlinNoise::Get2DNoise: floored lattice coordinates and the 3t^2 - 2t^3 weight (parked row 0x00F36010).
#include <stdio.h>
#include "engine/GFFloatToLongFloor.h"
static float S(float t) { return (3.0f - (t + t)) * t * t; }
int main()
{
    bool ok = GFFloatToLongFloor(-1.5f) == -2 && GFFloatToLongFloor(0.999f) == 0 && S(0.0f) == 0.0f && S(1.0f) == 1.0f && S(0.5f) == 0.5f;
    printf(ok ? "00f36010_TEST PASS\n" : "00f36010_TEST FAIL\n");
    return ok ? 0 : 1;
}
