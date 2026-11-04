// Behaviour test for CCamera::MoveSideways (0x00A0C1B0): a model of the row -- right = Forward x Up scaled by the distance; with
// HeightLocked only X and Y move.
#include <stdio.h>
#include <math.h>
struct V { float X, Y, Z; };
static V Cross(const V& a, const V& b) { V r = { a.Y * b.Z - a.Z * b.Y, a.Z * b.X - a.X * b.Z, a.X * b.Y - a.Y * b.X }; return r; }
static void MoveSideways(V& pos, const V& f, const V& u, float d, bool locked)
{
    V r = Cross(f, u);
    pos.X += r.X * d; pos.Y += r.Y * d; if (!locked) pos.Z += r.Z * d;
}
int main()
{
    V f = { 0, 1, 0 }, u = { 0, 0, 1 }, p = { 0, 0, 0 };
    MoveSideways(p, f, u, 2, false); int ok = fabs(p.X - 2) < 1e-6 && p.Y == 0 && p.Z == 0;
    V f2 = { 0, 0.6f, 0.8f }, u2 = { 1, 0, 0 }, q = { 0, 0, 0 };
    MoveSideways(q, f2, u2, 1, true); ok = ok && fabs(q.Y - 0.8f) < 1e-6 && q.Z == 0;
    printf(ok ? "00a0c1b0_TEST PASS\n" : "00a0c1b0_TEST FAIL\n");
    return ok ? 0 : 1;
}
