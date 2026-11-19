// Behaviour test for CTCWobble::OnInitialActivate (0x007E2DF0): the row's logic on stand-alone types; the frame count goes through the
// shared GFFloatToLongCeil (retail asm under VC7.1, the FABLE_PORTABLE twin elsewhere).
#ifndef _MSC_VER
#define FABLE_PORTABLE
#endif
#include <stdio.h>
#include <math.h>
#include "engine/GFFloatToLongCeil.h"
struct V3 { float X, Y, Z;
    void Rotate(const V3& axis, float angle) {   // rotate about an axis (unit), Rodrigues
        float c = cosf(angle), s = sinf(angle), d = X * axis.X + Y * axis.Y + Z * axis.Z;
        V3 k = { axis.Y * Z - axis.Z * Y, axis.Z * X - axis.X * Z, axis.X * Y - axis.Y * X };
        float x = X * c + k.X * s + axis.X * d * (1 - c), y = Y * c + k.Y * s + axis.Y * d * (1 - c), z = Z * c + k.Z * s + axis.Z * d * (1 - c);
        X = x; Y = y; Z = z; } };
struct RH { V3 Up, Forward; };
struct Physics { RH Current, Set; int setCalls; const RH& GetRHSet() const { return Current; } void SetRHSet(const RH& r) { Set = r; ++setCalls; } };
static long ConstantFPS;
struct Wobble {
    Physics* PhysicsTC; RH ParentRHSet; long FramesOfWobbleRemaining; float DirectionLastRotated; float AngleOfWobble;
    void OnInitialActivate() {
        AngleOfWobble = 0.004f;
        ParentRHSet = PhysicsTC->GetRHSet();
        RH new_rhset = ParentRHSet;
        V3 up = new_rhset.Up;
        up.Rotate(new_rhset.Forward, AngleOfWobble * 0.5f);
        new_rhset.Up = up;
        PhysicsTC->SetRHSet(new_rhset);
        DirectionLastRotated = 1.0f;
        FramesOfWobbleRemaining = GFFloatToLongCeil(ConstantFPS * 0.5f);
    }
};
int main()
{
    int fails = 0;
    Physics p = {}; p.Current.Up.Z = 1.0f; p.Current.Forward.X = 1.0f;
    Wobble w = {}; w.PhysicsTC = &p; w.DirectionLastRotated = -1.0f; w.FramesOfWobbleRemaining = 99;
    ConstantFPS = 15;
    w.OnInitialActivate();
    if (w.AngleOfWobble != 0.004f || w.DirectionLastRotated != 1.0f || w.FramesOfWobbleRemaining != 8) ++fails;   // ceil(7.5)
    if (w.ParentRHSet.Up.Z != 1.0f || w.ParentRHSet.Forward.X != 1.0f || p.setCalls != 1) ++fails;               // stored set unrotated
    if (fabs(p.Set.Up.Y + sinf(0.002f)) > 1e-6f || fabs(p.Set.Up.Z - cosf(0.002f)) > 1e-6f || p.Set.Forward.X != 1.0f) ++fails;
    ConstantFPS = 30; w.OnInitialActivate();
    if (w.FramesOfWobbleRemaining != 15) ++fails;                                                                  // exact: no round-up
    printf(fails ? "007e2df0_TEST FAIL\n" : "007e2df0_TEST PASS\n");
    return fails ? 1 : 0;
}