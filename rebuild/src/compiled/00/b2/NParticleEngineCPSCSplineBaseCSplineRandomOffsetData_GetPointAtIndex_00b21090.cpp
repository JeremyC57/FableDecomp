// NParticleEngine::CPSCSplineBase::CSplineRandomOffsetData::GetPointAtIndex(C3DVector& out, long index, float time) -- copy control point `index` of the current spline (or of last frame's spline) into `out`
// Leaf: ?GetPointAtIndex@CSplineRandomOffsetData@@QAEXAAVC3DVector@@JM@Z
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B21090, 197 bytes, `ret 0Ch` (the time argument is unused).
// Identity: Ego_r.pdb `?GetPointAtIndex@CSplineRandomOffsetData@CPSCSplineBase@NParticleEngine@@QAEXAAVC3DVector@@JM@Z`.
// Callee: CArray<C3DVector>::operator= @ 0x0075B8D0 (the inlined CCardinalSpline assignment).
// Shape: the sibling of the landed GetPointAtPos 0x00B20FD0 with a control point read instead of an evaluation:
//   an offset-at-startup spline reads ThisSpline directly; otherwise, when the previous frame's spline lost sync
//   with the current one (control-point counts differ, ThisSpline's count computed first), it is re-snapshotted
//   (array, Tension, EndPointsConnected) and the current point is returned; else last frame's point. The 12-byte
//   C3DVector copies are POD dword moves; the model is the native 20-byte CCardinalSpline of the landed siblings.
// Naming (NF126, 2026-11-20, q4802): the local CArray<T> (hand-declared operator=, bound 0x0075B8D0 as ??4?$CArray@VC3DVector@@@@QAEAAV0@ABV0@@Z)
//   and CCardinalSpline models replaced by the shared engine/CCardinalSpline.h (the 0x00B211B0 recipe defines; GetControlPoint /
//   GetControlPointCount, opt-in FABLE_RETAIL_CCARDINALSPLINE_CONTROLPOINT): the spline copy is the implicit CCardinalSpline::operator=,
//   whose ControlPoints copy calls std::vector<C3DVector>::operator= 0x0075B8D0 (Ego_r.pdb + FableWin.pdb).
#include "fable_compiler.h"
#define FABLE_RETAIL_STLP_VECTOR_C
#define FABLE_RETAIL_STLP_VECTOR_COPY_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CCARDINALSPLINE_CONTROLPOINT
#include "engine/CCardinalSpline.h"
struct CSplineRandomOffsetData {
    CCardinalSpline* InitialSpline;
    CCardinalSpline ThisSpline;
    CCardinalSpline PreviousSpline;
    bool OffsetOnlyAtStartup;
    bool ResetEachFrame;
    CArray<C3DVector> SplinePosOffsetLimits;
    CArray<C3DVector> SplinePosOffsetSpeed;
    void GetPointAtIndex(C3DVector& out, long index, float time);
};
void CSplineRandomOffsetData::GetPointAtIndex(C3DVector& out, long index, float time)
{
    if (OffsetOnlyAtStartup) { out = ThisSpline.GetControlPoint(index); return; }
    if (PreviousSpline.GetControlPointCount() != ThisSpline.GetControlPointCount()) {
        PreviousSpline = ThisSpline;
        out = ThisSpline.GetControlPoint(index);
    }
    else
        out = PreviousSpline.GetControlPoint(index);
}