// behaviour test for 0x0043ce90 C3DVector::Initialise(x, y, z): stores the three components
#include <cstdio>
struct V3 { float X, Y, Z; void Initialise(float x, float y, float z) { X = x; Y = y; Z = z; } };
int main()
{
    int fails = 0;
    V3 v; v.X = v.Y = v.Z = 9.0f;
    v.Initialise(1.5f, -2.0f, 0.25f);
    if (v.X != 1.5f || v.Y != -2.0f || v.Z != 0.25f) fails |= 1;
    if (fails) { std::printf("0043ce90_TEST FAIL %d\n", fails); return 1; }
    std::printf("0043ce90_TEST PASS\n");
    return 0;
}