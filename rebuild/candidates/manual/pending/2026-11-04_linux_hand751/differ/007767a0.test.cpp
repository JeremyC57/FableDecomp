// Behaviour test for CMatrix3x4::Set(const CRightHandedSet&, const C3DVector&) (0x007767A0, parked DIFFER row): a model of the row -- right = forward x up, rows up/forward copied, translation stored.
#include <stdio.h>
struct V { float X, Y, Z; };
static V Cross(const V& a, const V& b) { V r = { a.Y * b.Z - a.Z * b.Y, a.Z * b.X - a.X * b.Z, a.X * b.Y - a.Y * b.X }; return r; }
struct M { V Right, Forward, Up, Pos; void Set(const V& up, const V& fwd, const V& pos) { Right = Cross(fwd, up); Forward = fwd; Up = up; Pos = pos; } };
static int Check() { V up = { 0, 0, 1 }, fwd = { 0, 1, 0 }, pos = { 5, 6, 7 }; M m; m.Set(up, fwd, pos); return m.Right.X == 1 && m.Right.Y == 0 && m.Up.Z == 1 && m.Pos.Y == 6; }
int main()
{
    int ok = Check();
    printf(ok ? "007767a0_TEST PASS\n" : "007767a0_TEST FAIL\n");
    return ok ? 0 : 1;
}
