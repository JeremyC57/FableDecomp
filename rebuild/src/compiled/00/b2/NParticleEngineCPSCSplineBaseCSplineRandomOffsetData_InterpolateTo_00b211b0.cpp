// NParticleEngine::CPSCSplineBase::CSplineRandomOffsetData::InterpolateTo(const CSplineRandomOffsetData& other, float t) -- blend the
//   random-offset state towards another: lerp both splines' tension and control points, re-snapshot the previous spline when its
//   point count no longer matches, take the other's flags past t = 0.5 and interpolate the offset limit/speed arrays.
// Leaf: ?InterpolateTo@CSplineRandomOffsetData@CPSCSplineBase@NParticleEngine@@QAEXABV123@M@Z
// Name: FableWin.pdb + Ego_r.pdb (same mangled name); called only from the landed CPSCSplineBase::InterpolateTo 0x00B22310.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B211B0, 634 bytes, `ret 8`, default flags (no frame pointer, no EH frame).
// Callees: std::vector<C3DVector> copy ctor 0x006F0810 / operator= 0x0075B8D0 (STLport bodies visible to the TU but called
//   out of line: FABLE_RETAIL_STLP_VECTOR_C + FABLE_RETAIL_STLP_VECTOR_COPY_OUTOFLINE), NParticleEngine::GFInterpolateArray
//   <C3DVector> 0x00B15BD0 (__fastcall), free 0x00BFEA14 (inline ~vector). Floats 1.0 0x0122DED8, 0.5 0x0122F59C.
// Layout (retail 12-byte CArray, as the landed ctor 0x00B21DD0): InitialSpline +0, ThisSpline +4, PreviousSpline +0x18, flags
//   +0x2C/+0x2D, SplinePosOffsetLimits +0x30, SplinePosOffsetSpeed +0x3C. CCardinalSpline via the shared header (private
//   members, the original inline accessors Get/SetTension, Get/SetControlPoints (by-value copy), GetControlPointCount).
// Shape: `1 - t` in a named float; the previous spline's point count is read into a local first (retail spills the counts
//   and compares This against it); `PreviousSpline = ThisSpline` is the compiler-generated CCardinalSpline::operator=.
#include "fable_compiler.h"
#define FABLE_RETAIL_STLP_VECTOR_C
#define FABLE_RETAIL_STLP_VECTOR_COPY_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CARRAY
#include "engine/CCardinalSpline.h"
namespace NParticleEngine {
template <class T> CArray<T> __fastcall GFInterpolateArray(const CArray<T>& a, const CArray<T>& b, float t, const T& defaultValue);   // 0x00B15BD0
class CPSCSplineBase { public:                       // local: no shared header for CPSCSplineBase
    class CSplineRandomOffsetData { public:
        CCardinalSpline*  InitialSpline;          // +0x00
        CCardinalSpline   ThisSpline;             // +0x04
        CCardinalSpline   PreviousSpline;         // +0x18
        bool              OffsetOnlyAtStartup;    // +0x2C
        bool              ResetEachFrame;         // +0x2D
        CArray<C3DVector> SplinePosOffsetLimits;  // +0x30
        CArray<C3DVector> SplinePosOffsetSpeed;   // +0x3C
        void InterpolateTo(const CSplineRandomOffsetData& other, float t);
    };
};
void CPSCSplineBase::CSplineRandomOffsetData::InterpolateTo(const CSplineRandomOffsetData& other, float t)
{
    float s = 1.0f - t;
    ThisSpline.SetTension(s * ThisSpline.GetTension() + t * other.ThisSpline.GetTension());
    ThisSpline.SetControlPoints(GFInterpolateArray(ThisSpline.GetControlPoints(), other.ThisSpline.GetControlPoints(), t,
        C3DVector(0.0f, 0.0f, 0.0f)));
    PreviousSpline.SetTension(s * PreviousSpline.GetTension() + t * other.PreviousSpline.GetTension());
    PreviousSpline.SetControlPoints(GFInterpolateArray(PreviousSpline.GetControlPoints(),
        other.PreviousSpline.GetControlPoints(), t, C3DVector(0.0f, 0.0f, 0.0f)));
    long prevCount = PreviousSpline.GetControlPointCount();
    if (ThisSpline.GetControlPointCount() != prevCount)
        PreviousSpline = ThisSpline;
    OffsetOnlyAtStartup = t > 0.5f ? other.OffsetOnlyAtStartup : OffsetOnlyAtStartup;
    ResetEachFrame = t > 0.5f ? other.ResetEachFrame : ResetEachFrame;
    SplinePosOffsetLimits = GFInterpolateArray(SplinePosOffsetLimits, other.SplinePosOffsetLimits, t,
        C3DVector(0.0f, 0.0f, 0.0f));
    SplinePosOffsetSpeed = GFInterpolateArray(SplinePosOffsetSpeed, other.SplinePosOffsetSpeed, t,
        C3DVector(0.0f, 0.0f, 0.0f));
}
}