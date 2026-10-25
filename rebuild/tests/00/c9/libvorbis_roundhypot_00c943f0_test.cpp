// Behaviour test for 0x00C943F0 libvorbis round_hypot (copy of the logic).
#include <cstdio>
#include <cmath>
static float round_hypot(float a, float b)
{
    if (a > 0.) {
        if (b > 0.) return (float)std::sqrt((double)(a * a + b * b));
        if (a > -b) return (float)std::sqrt((double)(a * a + b * b));
        return (float)-std::sqrt((double)(b * b + a * a));
    }
    if (b < 0.) return (float)-std::sqrt((double)(a * a + b * b));
    if (-a > b) return (float)-std::sqrt((double)(a * a + b * b));
    return (float)std::sqrt((double)(b * b + a * a));
}
static int near(float a, float b) { float d = a - b; return d < 1e-4f && d > -1e-4f; }
int main()
{
    int ok = near(round_hypot(3, 4), 5) && near(round_hypot(4, -3), 5) && near(round_hypot(3, -4), -5) &&
             near(round_hypot(-3, -4), -5) && near(round_hypot(-4, 3), -5) && near(round_hypot(-3, 4), 5);
    if (!ok) { std::printf("00c943f0_TEST FAIL\n"); return 1; }
    std::printf("00c943f0_TEST PASS\n");
    return 0;
}