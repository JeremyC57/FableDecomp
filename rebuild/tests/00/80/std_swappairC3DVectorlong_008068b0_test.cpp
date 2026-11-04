// Behaviour test for 0x008068B0 std::swap<pair<C3DVector, long> > (see the source): the two pairs are exchanged whole.
#include <cstdio>
#include <algorithm>
#include <utility>
struct V { float X, Y, Z; };
int main()
{
    std::pair<V, long> a, b;
    a.first.X = 1; a.first.Y = 2; a.first.Z = 3; a.second = 4;
    b.first.X = -1; b.first.Y = -2; b.first.Z = -3; b.second = 99;
    std::swap(a, b);
    bool ok = a.first.X == -1 && a.first.Z == -3 && a.second == 99 && b.first.Y == 2 && b.second == 4;
    std::swap(b, b);
    ok = ok && b.second == 4 && b.first.Z == 3;
    std::printf(ok ? "008068b0_TEST PASS\n" : "008068b0_TEST FAIL\n");
    return ok ? 0 : 1;
}