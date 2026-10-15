// Behaviour test for C3DVector::DotProduct(const C3DVector&) @ 0x00C256A0
#include <cstdio>

struct C3DVector {
    float X, Y, Z;
    float DotProduct(const C3DVector& other) const { return Z * other.Z + Y * other.Y + X * other.X; }
};

int main()
{
    C3DVector a = { 1.0f, 2.0f, 3.0f };
    C3DVector b = { 4.0f, -5.0f, 0.5f };
    if (a.DotProduct(b) != 4.0f - 10.0f + 1.5f) { std::printf("00c256a0_TEST FAIL\n"); return 1; }
    if (a.DotProduct(a) != 14.0f) { std::printf("00c256a0_TEST FAIL\n"); return 1; }
    std::printf("00c256a0_TEST PASS\n");
    return 0;
}