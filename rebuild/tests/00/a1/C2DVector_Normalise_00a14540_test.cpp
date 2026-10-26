#include <cstdio>
#include <cstring>
#include <cmath>
unsigned int g_InvSqrtMantissaTable[128];
class C2DVector { public:
    float X;
    float Y;
    C2DVector& Normalise();
};
C2DVector& C2DVector::Normalise()
{
    float x = X;
    float y = Y;
    float lengthSquared = x * x + y * y;
    unsigned int bits;
    std::memcpy(&bits, &lengthSquared, 4);
    unsigned int estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                             (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
    float r;
    std::memcpy(&r, &estimate, 4);
    r = (3.0f - r * r * lengthSquared) * r * 0.5f;
    X *= r;
    Y *= r;
    return *this;
}
int main()
{
    // with an all-zero table the estimate is a power of two; one Newton step still lands near 1/len
    C2DVector v;
    v.X = 3.0f; v.Y = 4.0f;
    C2DVector& ref = v.Normalise();
    float len = std::sqrt(v.X * v.X + v.Y * v.Y);
    if (&ref != &v || len < 0.75f || len > 1.25f || v.X <= 0.0f || std::fabs(v.Y / v.X - 4.0f / 3.0f) > 1e-4f) {
        std::printf("00a14540_TEST FAIL\n"); return 1;
    }
    std::printf("00a14540_TEST PASS\n");
    return 0;
}