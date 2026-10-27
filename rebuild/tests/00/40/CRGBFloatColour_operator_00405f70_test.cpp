// behaviour test for 0x00405f70 CRGBFloatColour::operator*=(float): scales all four channels and returns *this
#include <cstdio>
struct Col { float R, G, B, A; Col& operator*=(float s) { R *= s; G *= s; B *= s; A *= s; return *this; } };
int main()
{
    int fails = 0;
    Col c; c.R = 1.0f; c.G = 0.5f; c.B = 0.25f; c.A = 2.0f;
    Col& r = (c *= 2.0f);
    if (&r != &c) fails |= 1;
    if (c.R != 2.0f || c.G != 1.0f || c.B != 0.5f || c.A != 4.0f) fails |= 2;
    c *= 0.0f;
    if (c.R != 0.0f || c.A != 0.0f) fails |= 4;
    if (fails) { std::printf("00405f70_TEST FAIL %d\n", fails); return 1; }
    std::printf("00405f70_TEST PASS\n");
    return 0;
}