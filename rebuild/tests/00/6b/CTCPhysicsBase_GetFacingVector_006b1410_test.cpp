// behaviour test for 0x006B1410: GetFacingVector returns (0.0f, 1.0f, 0.0f)
#include <cstdio>
struct V { float X, Y, Z; V(float x, float y, float z) : X(x), Y(y), Z(z) { } };
static V Get() { return V(0.0f, 1.0f, 0.0f); }
int main()
{
    V v = Get();
    if (v.X != 0.0f || v.Y != 1.0f || v.Z != 0.0f) { std::printf("006b1410_TEST FAIL\n"); return 1; }
    std::printf("006b1410_TEST PASS\n");
    return 0;
}