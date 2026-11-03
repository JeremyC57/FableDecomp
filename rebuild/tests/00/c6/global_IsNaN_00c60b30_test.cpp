// behaviour test for 0x00C60B30 IsAnyNaN: NaN in any component
#include <cstdio>
struct EAXVECTOR { float x, y, z; };
static int IsNaN(double x) { return x != x; }
static bool IsAnyNaN(const EAXVECTOR& v) { return IsNaN(v.x) || IsNaN(v.y) || IsNaN(v.z); }
int main()
{
    float zero = 0.0f; float nan = zero / zero;
    EAXVECTOR a = { 1.0f, 2.0f, 3.0f };
    if (IsAnyNaN(a)) { std::printf("00c60b30_TEST FAIL\n"); return 1; }
    EAXVECTOR b = a; b.x = nan; if (!IsAnyNaN(b)) { std::printf("00c60b30_TEST FAIL\n"); return 1; }
    b = a; b.y = nan; if (!IsAnyNaN(b)) { std::printf("00c60b30_TEST FAIL\n"); return 1; }
    b = a; b.z = nan; if (!IsAnyNaN(b)) { std::printf("00c60b30_TEST FAIL\n"); return 1; }
    std::printf("00c60b30_TEST PASS\n");
    return 0;
}