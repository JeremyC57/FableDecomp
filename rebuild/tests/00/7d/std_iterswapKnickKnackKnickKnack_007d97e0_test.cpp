// Behaviour test for 0x007D97E0 std::iter_swap over KnickKnack (see the source): the two records are exchanged whole.
#include <cstdio>
#include <algorithm>
struct V { float X, Y, Z; };
struct KnickKnack { V Pos; void* Thing; };
int main()
{
    KnickKnack a, b; int t1, t2;
    a.Pos.X = 1; a.Pos.Y = 2; a.Pos.Z = 3; a.Thing = &t1;
    b.Pos.X = -1; b.Pos.Y = -2; b.Pos.Z = -3; b.Thing = &t2;
    std::iter_swap(&a, &b);
    bool ok = a.Pos.X == -1 && a.Pos.Z == -3 && a.Thing == &t2 && b.Pos.Y == 2 && b.Thing == &t1;
    std::iter_swap(&a, &a);
    ok = ok && a.Thing == &t2 && a.Pos.Y == -2;
    std::printf(ok ? "007d97e0_TEST PASS\n" : "007d97e0_TEST FAIL\n");
    return ok ? 0 : 1;
}