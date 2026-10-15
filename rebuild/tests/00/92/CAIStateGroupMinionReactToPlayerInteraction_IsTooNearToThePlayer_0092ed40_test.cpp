#include <cstdio>
// IsTooNearToThePlayer: squared distance < minDistance^2
struct V { float X, Y, Z; V(float x, float y, float z) : X(x), Y(y), Z(z) {} V operator-(const V& o) const { return V(X - o.X, Y - o.Y, Z - o.Z); } float MagSq() const { return X * X + (Y * Y + Z * Z); } };
static float g_min = 1.0f;
static bool IsTooNear(const V& creature, const V& player) { V d = player - creature; float m = g_min; return d.MagSq() < m * m; }
int main()
{
    V c(1, 2, 3);
    if (!IsTooNear(c, V(1.5f, 2, 3))) { std::printf("0092ed40_TEST FAIL\n"); return 1; }
    if (IsTooNear(c, V(2.5f, 2, 3))) { std::printf("0092ed40_TEST FAIL\n"); return 1; }
    g_min = 2.0f; if (!IsTooNear(c, V(2.5f, 2, 3))) { std::printf("0092ed40_TEST FAIL\n"); return 1; }
    if (IsTooNear(c, V(1, 4, 3))) { std::printf("0092ed40_TEST FAIL\n"); return 1; }
    std::printf("0092ed40_TEST PASS\n"); return 0;
}