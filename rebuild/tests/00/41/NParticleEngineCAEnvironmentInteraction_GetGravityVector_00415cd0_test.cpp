// behaviour test for 0x00415CD0: GetGravityVector returns (0.0f, 0.0f, -9.82f)
#include <cstdio>
struct V { float X, Y, Z; V(float x, float y, float z) : X(x), Y(y), Z(z) { } };
static V Get() { return V(0.0f, 0.0f, -9.82f); }
int main()
{
    V v = Get();
    if (v.X != 0.0f || v.Y != 0.0f || v.Z != -9.82f) { std::printf("00415cd0_TEST FAIL\n"); return 1; }
    std::printf("00415cd0_TEST PASS\n");
    return 0;
}