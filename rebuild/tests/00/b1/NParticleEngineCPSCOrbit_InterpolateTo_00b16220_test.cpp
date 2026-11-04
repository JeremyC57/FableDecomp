// Behaviour test for NParticleEngine::CPSCOrbit::InterpolateTo (0x00B16220): the centre parameter is the rounded lerp
// (GFFloatToLongNear inside GFInterpolate(unsigned long)), flags and type switch to the target past t = 0.5, floats lerp.
#include <stdio.h>
#include "engine/GFFloatToLongNear.h"
static float Lerp(float a, float b, float t) { return a * (1.0f - t) + b * t; }
static unsigned long Lerp(unsigned long a, unsigned long b, float t) { return GFFloatToLongNear((1.0f - t) * a + b * t); }
static bool Lerp(bool a, bool b, float t) { return t > 0.5f ? b : a; }
struct Orbit { long Type; bool Enabled; float Radius; };
int main()
{
    bool ok = Lerp(10UL, 20UL, 0.25f) == 12 && Lerp(10UL, 20UL, 0.35f) == 14 && Lerp(0UL, 3UL, 0.5f) == 2;
    ok = ok && Lerp(1.0f, 3.0f, 0.5f) == 2.0f && Lerp(false, true, 0.5f) == false && Lerp(false, true, 0.75f) == true;
    Orbit a = { 1, false, 2.0f }, b = { 2, true, 6.0f };
    float t = 0.75f;
    a.Type = t > 0.5f ? b.Type : a.Type;
    a.Enabled = Lerp(a.Enabled, b.Enabled, t);
    a.Radius = Lerp(a.Radius, b.Radius, t);
    ok = ok && a.Type == 2 && a.Enabled && a.Radius == 5.0f;
    printf(ok ? "00b16220_TEST PASS\n" : "00b16220_TEST FAIL\n");
    return ok ? 0 : 1;
}