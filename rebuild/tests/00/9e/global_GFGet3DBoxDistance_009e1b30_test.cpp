// Behaviour test for GFGet3DBoxDistance (0x009E1B30): max(|dx|, |dy|, |dz|).
#include <cstdio>
#include <cmath>

class C3DVector { public: float X; float Y; float Z; };

template <class T> inline T GFMax(T a, T b) { return a > b ? a : b; }

float GFGet3DBoxDistance(const C3DVector& a, const C3DVector& b)
{
    float dz = (float)std::fabs(a.Z - b.Z);
    float dx = (float)std::fabs(a.X - b.X);
    float dy = (float)std::fabs(a.Y - b.Y);
    return GFMax(GFMax(dx, dy), dz);
}

int main()
{
    C3DVector p = { 1.0f, 2.0f, 3.0f };
    C3DVector q = { 2.0f, 2.5f, -7.0f };
    if (GFGet3DBoxDistance(p, q) != 10.0f) { std::printf("009e1b30_TEST FAIL\n"); return 1; }
    C3DVector r = { -4.0f, 2.0f, 3.0f };
    if (GFGet3DBoxDistance(p, r) != 5.0f) { std::printf("009e1b30_TEST FAIL\n"); return 1; }
    C3DVector s = { 1.0f, 8.0f, 4.0f };
    if (GFGet3DBoxDistance(p, s) != 6.0f) { std::printf("009e1b30_TEST FAIL\n"); return 1; }
    std::printf("009e1b30_TEST PASS\n");
    return 0;
}