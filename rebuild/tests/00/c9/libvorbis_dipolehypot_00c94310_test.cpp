// Behaviour test for 0x00C94310 libvorbis dipole_hypot (copy of the logic).
#include <cstdio>
#include <cmath>
static float dipole_hypot(float a, float b)
{
    if (a > 0.) {
        if (b > 0.) return (float)std::sqrt((double)(a * a + b * b));
        if (a > -b) return (float)std::sqrt((double)(a * a - b * b));
        return (float)-std::sqrt((double)(b * b - a * a));
    }
    if (b < 0.) return (float)-std::sqrt((double)(a * a + b * b));
    if (-a > b) return (float)-std::sqrt((double)(a * a - b * b));
    return (float)std::sqrt((double)(b * b - a * a));
}
static int near(float a, float b) { float d = a - b; return d < 1e-4f && d > -1e-4f; }
int main()
{
    int ok = near(dipole_hypot(3, 4), 5) && near(dipole_hypot(5, -3), 4) && near(dipole_hypot(3, -5), -4) &&
             near(dipole_hypot(-3, -4), -5) && near(dipole_hypot(-5, 3), -4) && near(dipole_hypot(-3, 5), 4) &&
             near(dipole_hypot(0, 0), 0);
    if (!ok) { std::printf("00c94310_TEST FAIL\n"); return 1; }
    std::printf("00c94310_TEST PASS\n");
    return 0;
}