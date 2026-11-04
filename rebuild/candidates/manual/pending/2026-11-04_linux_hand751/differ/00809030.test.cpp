// Behaviour test for CThingFilter_IsInTargetingRangeAndFOV ctor (0x00809030, parked DIFFER row): a model of the row -- the facing is flattened (unless 3D) and normalised; the target lock comes from the targeter's targeting component.
#include <stdio.h>
#include <math.h>
struct V { float X, Y, Z; };
struct F { V Pos, Facing; void* Lock; F(const V& p, const V& f, bool angle3D, void* lock) : Pos(p), Facing(f), Lock(0) { if (!angle3D) Facing.Z = 0; float l = sqrtf(Facing.X * Facing.X + Facing.Y * Facing.Y + Facing.Z * Facing.Z); Facing.X /= l; Facing.Y /= l; Facing.Z /= l; Lock = lock; } };
static int Check() { V p = { 1, 1, 1 }, f = { 3, 0, 4 }; int x; F a(p, f, false, &x); F b(p, f, true, 0); return a.Facing.X == 1.0f && a.Facing.Z == 0 && a.Lock == &x && fabsf(b.Facing.Z - 0.8f) < 1e-5f && b.Lock == 0; }
int main()
{
    int ok = Check();
    printf(ok ? "00809030_TEST PASS\n" : "00809030_TEST FAIL\n");
    return ok ? 0 : 1;
}
