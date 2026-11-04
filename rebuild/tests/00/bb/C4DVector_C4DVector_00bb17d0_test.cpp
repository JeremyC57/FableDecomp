// Behaviour test for C4DVector::C4DVector(const CRGBColour&) (0x00BB17D0): a model of the row -- each byte channel of a BGRA
// colour times 1/255 lands in X=R, Y=G, Z=B, W=A.
#include <stdio.h>
#include <math.h>
struct Colour { unsigned char B, G, R, A; };
struct Vec4 { float X, Y, Z, W; };
static Vec4 FromColour(const Colour& c)
{
    float r = c.R * (1.0f / 255.0f), g = c.G * (1.0f / 255.0f), b = c.B * (1.0f / 255.0f), a = c.A * (1.0f / 255.0f);
    Vec4 v; v.X = r; v.Y = g; v.Z = b; v.W = a;
    return v;
}
int main()
{
    Colour c = { 0, 51, 255, 102 };
    Vec4 v = FromColour(c);
    int ok = fabs(v.X - 1.0f) < 1e-6 && fabs(v.Y - 0.2f) < 1e-6 && v.Z == 0.0f && fabs(v.W - 0.4f) < 1e-6;
    printf(ok ? "00bb17d0_TEST PASS\n" : "00bb17d0_TEST FAIL\n");
    return ok ? 0 : 1;
}