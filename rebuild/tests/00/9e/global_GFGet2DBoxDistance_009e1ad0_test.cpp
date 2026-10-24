// Behaviour test for GFGet2DBoxDistance (0x009E1AD0): max(|dx|, |dy|).
#include <cstdio>
#include <cmath>

class C2DVector { public: float X; float Y; };

template <class T> inline T GFMax(T a, T b) { return a > b ? a : b; }

float GFGet2DBoxDistance(const C2DVector& a, const C2DVector& b)
{
    float dx = (float)std::fabs(a.X - b.X);
    float dy = (float)std::fabs(a.Y - b.Y);
    return GFMax(dx, dy);
}

int main()
{
    C2DVector p = { 1.0f, 2.0f };
    C2DVector q = { 4.0f, -5.0f };
    if (GFGet2DBoxDistance(p, q) != 7.0f) { std::printf("009e1ad0_TEST FAIL\n"); return 1; }
    C2DVector r = { -9.0f, 1.0f };
    if (GFGet2DBoxDistance(p, r) != 10.0f) { std::printf("009e1ad0_TEST FAIL\n"); return 1; }
    if (GFGet2DBoxDistance(p, p) != 0.0f) { std::printf("009e1ad0_TEST FAIL\n"); return 1; }
    std::printf("009e1ad0_TEST PASS\n");
    return 0;
}