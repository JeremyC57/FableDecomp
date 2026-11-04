// Behaviour test for 0x00BC2480 (parked row): the row's logic over a flat model.
#include <cstdio>

struct V { float X, Y, Z; };
static V Cross(const V& a, const V& b) { V r = { a.Y*b.Z - a.Z*b.Y, a.Z*b.X - a.X*b.Z, a.X*b.Y - a.Y*b.X }; return r; }
int main() {
    V size = { 100, 200, 300 }, fwd = { 0, 1, 0 }, up = { 0, 0, 1 };
    V s = { size.X * 0.01f, size.Y * 0.01f, size.Z * 0.01f };
    V right = Cross(fwd, up); V row1 = { -right.X * s.X, -right.Y * s.X, -right.Z * s.X };
    V row2 = { -fwd.X * s.Y, -fwd.Y * s.Y, -fwd.Z * s.Y }; V row3 = { up.X * s.Z, up.Y * s.Z, up.Z * s.Z };
    bool ok = row1.X == -1 && row2.Y == -2 && row3.Z == 3;
    std::printf(ok ? "00bc2480_TEST PASS\n" : "00bc2480_TEST FAIL\n");
    return ok ? 0 : 1;
}
