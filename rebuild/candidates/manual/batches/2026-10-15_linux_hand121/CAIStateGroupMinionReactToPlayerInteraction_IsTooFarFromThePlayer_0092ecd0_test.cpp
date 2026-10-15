#include <cstdio>
// IsTooFarFromThePlayer: squared distance > 9
struct V { float X, Y, Z; V(float x, float y, float z) : X(x), Y(y), Z(z) {} V operator-(const V& o) const { return V(X - o.X, Y - o.Y, Z - o.Z); } float MagSq() const { return X * X + Y * Y + Z * Z; } };
static bool IsTooFar(const V& creature, const V& player) { static const float s = 3.0f * 3.0f; return (player - creature).MagSq() > s; }
int main()
{
    V c(1, 2, 3);
    if (IsTooFar(c, V(1, 2, 5))) { std::printf("0092ecd0_TEST FAIL\n"); return 1; }
    if (IsTooFar(c, V(4, 2, 3))) { std::printf("0092ecd0_TEST FAIL\n"); return 1; }
    if (!IsTooFar(c, V(4.5f, 2, 3))) { std::printf("0092ecd0_TEST FAIL\n"); return 1; }
    if (!IsTooFar(c, V(-1, 0, 1))) { std::printf("0092ecd0_TEST FAIL\n"); return 1; }
    std::printf("0092ecd0_TEST PASS\n"); return 0;
}
