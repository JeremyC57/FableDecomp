// Behaviour test for 00c46e90: C2DVector::GetLength -- sqrt(X*X + Y*Y)
#include <cstdio>
#include <cmath>

struct C2DVector {
    float X, Y;
    float GetLength() const { return (float)std::sqrt(X * X + Y * Y); }
};

int main()
{
    C2DVector a = { 3.0f, 4.0f };
    C2DVector b = { -6.0f, 8.0f };
    C2DVector z = { 0.0f, 0.0f };
    if (std::fabs(a.GetLength() - 5.0f) > 1e-5f || std::fabs(b.GetLength() - 10.0f) > 1e-5f || z.GetLength() != 0.0f) {
        std::printf("00c46e90_TEST FAIL\n"); return 1;
    }
    std::printf("00c46e90_TEST PASS\n");
    return 0;
}