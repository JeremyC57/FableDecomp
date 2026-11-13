// NParticleEngine::CPSCSplineBase::CSplineRandomOffsetData::OffsetSplinePositions() -- random-walk each control point of
//   ThisSpline away from InitialSpline: for every point whose offset limit is non-zero, step its current offset by a random
//   amount in [-speed, +speed) per axis, then pull the new offset back inside the limit ellipsoid (an axis whose limit is
//   (near) zero is pinned to 0 and drops out of the ellipsoid test), and store initial + offset back into ThisSpline.
// Leaf: ?OffsetSplinePositions@CSplineRandomOffsetData@CPSCSplineBase@NParticleEngine@@QAEXXZ
// Name: Ego_r.pdb + FableWin.pdb (same mangled name, public void(void) __fastcall).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B206B0, 773 bytes, plain `ret`, default flags (ebp holds `this`, no EH frame).
// Callees: GFFloatRandom(float, unsigned long*) 0x004978A0 (__fastcall, engine/GFFloatRandom.h) on the TU-local seed at
//   0x013999C4 (initialised 42; referenced only by this function, no PDB name -> descriptive SplineRandomSeed).
//   Inline (expanded in retail): CCardinalSpline::GetControlPointCount / GetControlPoint / SetControlPoint, C3DVector::IsNonZero
//   (X*X+Y*Y+Z*Z > DELTA_NOTIONAL_ZERO^2, 0x0129BA3C), C3DVector operator- / operator+ / operator+=, GFSqr (fld; fld st0; fmul).
//   Pool: 1e-4f 0x0129F0BC (axis-limit threshold, a literal), 0.0f 0x0122DEDC, 1.0f 0x0122DED8.
// Layout (retail 12-byte CArray, as the landed ctor 0x00B21DD0): InitialSpline +0, ThisSpline +4, PreviousSpline +0x18,
//   flags +0x2C/+0x2D, SplinePosOffsetLimits +0x30, SplinePosOffsetSpeed +0x3C. CPSCSplineBase has no shared header (local
//   nested model, as the landed sibling InterpolateTo 0x00B211B0).
// Shape: the random step is added onto the offset (`change += offset`, VC commutes the fadd so the offset loads first); each
//   ellipsoid term is a parenthesised (m*m)/l2 and the sum is built as running statements (`d = ...; d += ...;`), which keeps
//   the X, Y, Z evaluation order and lets the Z-limit branch and the full branch share the X+Y partial sum.
//   The clamp works through a reference alias `C3DVector& moved = change;` (the offset after the step), which retail needs:
//   spelling `change` directly instead regrades DIFFER(773v773) only in the inlined limit.IsNonZero() at +0x42..+0x4C
//   (0x00B206F2): retail `fld st0; fmul st1; fld st2; fmul st3; faddp` (X*X then Y*Y over the X,Y,Z loads) becomes
//   a `fld st1; fmul st2; ...` order with an extra fld/fmul pair after the faddp (+0x4A) -- the alias symbol shifts VC7.1's x87 stack scheduling tie for the loop.
//   (Most likely the original clamped through a C3DVector& in an inlined helper; no name for one is found, so none is invented.)
#include "fable_compiler.h"
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_NONZERO_SQUARED_XYZ
#define FABLE_RETAIL_CCARDINALSPLINE_CONTROLPOINT
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CARRAY
#include "engine/CCardinalSpline.h"
#include "engine/GFFloatRandom.h"
#include "engine/GFWorldDistance.h"
static unsigned long SplineRandomSeed = 42;   // 0x013999C4 (descriptive name: no PDB symbol found)
namespace NParticleEngine {
class CPSCSplineBase { public:
    class CSplineRandomOffsetData { public:
        CCardinalSpline*  InitialSpline;
        CCardinalSpline   ThisSpline;
        CCardinalSpline   PreviousSpline;
        bool              OffsetOnlyAtStartup;
        bool              ResetEachFrame;
        CArray<C3DVector> SplinePosOffsetLimits;
        CArray<C3DVector> SplinePosOffsetSpeed;
        void OffsetSplinePositions();
    };
};
void CPSCSplineBase::CSplineRandomOffsetData::OffsetSplinePositions()
{
    long count = ThisSpline.GetControlPointCount();
    for (long i = 0; i < count; ++i) {
        const C3DVector& limit = SplinePosOffsetLimits[i];
        if (limit.IsNonZero()) {
            const C3DVector& thisPoint = ThisSpline.GetControlPoint(i);
            const C3DVector& initialPoint = InitialSpline->GetControlPoint(i);
            C3DVector offset = thisPoint - initialPoint;
            const C3DVector& speed = SplinePosOffsetSpeed[i];
            C3DVector change;
            change.X = speed.X != 0.0f ? GFFloatRandom(speed.X * 2.0f, &SplineRandomSeed) - speed.X : 0.0f;
            change.Y = speed.Y != 0.0f ? GFFloatRandom(speed.Y * 2.0f, &SplineRandomSeed) - speed.Y : 0.0f;
            change.Z = speed.Z != 0.0f ? GFFloatRandom(speed.Z * 2.0f, &SplineRandomSeed) - speed.Z : 0.0f;
            change += offset;
            C3DVector& moved = change;
            float limitX2 = GFSqr(limit.X);
            float limitY2 = GFSqr(limit.Y);
            float limitZ2 = GFSqr(limit.Z);
            if (limitX2 < 0.0001f) {
                moved.X = 0.0f;
                float d = (moved.Y * moved.Y) / limitY2; d += (moved.Z * moved.Z) / limitZ2;
                if (d > 1.0f) { float scale = 1.0f / d; moved.Y *= scale; moved.Z *= scale; }
            } else if (limitY2 < 0.0001f) {
                moved.Y = 0.0f;
                float d = (moved.X * moved.X) / limitX2; d += (moved.Z * moved.Z) / limitZ2;
                if (d > 1.0f) { float scale = 1.0f / d; moved.X *= scale; moved.Z *= scale; }
            } else if (limitZ2 < 0.0001f) {
                moved.Z = 0.0f;
                float d = (moved.X * moved.X) / limitX2; d += (moved.Y * moved.Y) / limitY2;
                if (d > 1.0f) { float scale = 1.0f / d; moved.X *= scale; moved.Y *= scale; }
            } else {
                float d = (moved.X * moved.X) / limitX2; d += (moved.Y * moved.Y) / limitY2; d += (moved.Z * moved.Z) / limitZ2;
                if (d > 1.0f) { float scale = 1.0f / d; moved.X *= scale; moved.Y *= scale; moved.Z *= scale; }
            }
            ThisSpline.SetControlPoint(i, moved + initialPoint);
        }
    }
}
}