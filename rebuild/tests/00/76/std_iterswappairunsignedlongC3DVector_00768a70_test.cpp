// Behaviour test for 0x00768A70 std::iter_swap<pair<unsigned long, C3DVector>*> (see the source): the two entries are exchanged whole.
#include <cstdio>
#include <algorithm>
#include <utility>
struct V { float X, Y, Z; };
int main()
{
    std::pair<unsigned long, V> a, b;
    a.first = 7; a.second.X = 1; a.second.Y = 2; a.second.Z = 3;
    b.first = 9; b.second.X = -1; b.second.Y = -2; b.second.Z = -3;
    std::iter_swap(&a, &b);
    bool ok = a.first == 9 && a.second.X == -1 && a.second.Z == -3 && b.first == 7 && b.second.Y == 2;
    std::iter_swap(&a, &a);
    ok = ok && a.first == 9 && a.second.Y == -2;
    std::printf(ok ? "00768a70_TEST PASS\n" : "00768a70_TEST FAIL\n");
    return ok ? 0 : 1;
}