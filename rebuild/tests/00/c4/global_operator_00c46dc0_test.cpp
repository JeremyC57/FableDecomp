// Behaviour test for operator==(C2DVector, C2DVector) @ 0x00C46DC0: exact component equality (NaN never equal).
#include <cstdio>
class C2DVector { public: float X, Y; };
static bool Equal(const C2DVector& a, const C2DVector& b) { return a.X == b.X && a.Y == b.Y; }
int main()
{
    C2DVector a = { 1.0f, 2.0f }, b = { 1.0f, 2.0f }, c = { 1.0f, 3.0f }, d = { 0.5f, 2.0f };
    volatile float zero = 0.0f; C2DVector n; n.X = zero / zero; n.Y = 2.0f;
    if (!Equal(a, b) || Equal(a, c) || Equal(a, d) || Equal(n, n)) { std::printf("00c46dc0_TEST FAIL\n"); return 1; }
    std::printf("00c46dc0_TEST PASS\n");
    return 0;
}