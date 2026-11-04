// Behaviour test for CFloatQuad::Interpolate (0x00C462E0, parked DIFFER row): a model of the row -- bilinear: lerp(lerp(c2, c3, u), lerp(c0, c1, u), v).
#include <stdio.h>
static float Lerp(float a, float b, float t) { return a + (b - a) * t; }
static float Interp(const float c[4], float u, float v) { return Lerp(Lerp(c[2], c[3], u), Lerp(c[0], c[1], u), v); }
static int Check() { float c[4] = { 0, 10, 20, 30 }; return Interp(c, 0, 0) == 20 && Interp(c, 1, 1) == 10 && Interp(c, 0.5f, 0.5f) == 15; }
int main()
{
    int ok = Check();
    printf(ok ? "00c462e0_TEST PASS\n" : "00c462e0_TEST FAIL\n");
    return ok ? 0 : 1;
}
