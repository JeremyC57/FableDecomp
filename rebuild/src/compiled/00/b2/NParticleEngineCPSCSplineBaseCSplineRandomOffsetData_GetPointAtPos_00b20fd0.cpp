// NParticleEngine::CPSCSplineBase::CSplineRandomOffsetData::GetPointAtPos(out,position,time) -- evaluate current point or previous-frame point, synchronizing changed spline counts
// Naming wave 35c (queue land0411j 0x00A567A0): calls 0x00A567A0 as CCardinalSpline::GetSplinePosition(C3DVector&, float) const (Ego_r.pdb ?GetSplinePosition@CCardinalSpline@@QBEXAAVC3DVector@@M@Z; the out vector by reference).
// Leaf: GetPointAtPos
// Retail00B20FD0,177 bytes through ret12 at00B2107E;15 trailing int3 bytes excluded.
// Same PDB/native20-byte CCardinalSpline model as canonical Update00B21160. No shared
// spline/vector headers exist. Array assignment0075B8D0; spline evaluator00A567A0.
// Stable-count path evaluates current into a discarded local before evaluating previous toout;
// changed count snapshots current and evaluates current. Third float parameter is unused.
// Name fix (NF124, 2026-11-20, q4802): the local C3DVector / CArray / CCardinalSpline models (CArray hand-declared operator= bound
//   0x0075B8D0 as ??4?$CArray@VC3DVector@@@@QAEAAV0@ABV0@@Z) replaced by the shared engine/CCardinalSpline.h (CArray.h, C3DVector.h;
//   opt-in FABLE_RETAIL_CCARDINALSPLINE_GETSPLINEPOSITION), the 0x00B211B0 InterpolateTo recipe: the spline copy is the implicit
//   CCardinalSpline::operator=, whose ControlPoints copy calls std::vector<C3DVector>::operator= 0x0075B8D0 (Ego_r.pdb + FableWin.pdb
//   ??4?$vector@VC3DVector@@V?$allocator@VC3DVector@@@std@@@std@@QAEAAV01@ABV01@@Z); the counts are GetControlPointCount().
// Shape (NF126, 2026-11-20): the previous spline's count is read into a local first and compared on the left
//   (`prevCount != ThisSpline.GetControlPointCount()`): retail spills This into ebx, Prev into eax and emits `cmp eax,ebx`; the 0x00B211B0
//   order (`This != prevCount`) gives `cmp ebx,eax` here (NF124 work/: DIFFER 177v177, the only difference).
#include "fable_compiler.h"
#define FABLE_RETAIL_STLP_VECTOR_C
#define FABLE_RETAIL_STLP_VECTOR_COPY_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CCARDINALSPLINE_GETSPLINEPOSITION
#include "engine/CCardinalSpline.h"
struct CSplineRandomOffsetData {
    CCardinalSpline* InitialSpline;
    CCardinalSpline ThisSpline;
    CCardinalSpline PreviousSpline;
    bool OffsetOnlyAtStartup;
    bool ResetEachFrame;
    CArray<C3DVector> SplinePosOffsetLimits;
    CArray<C3DVector> SplinePosOffsetSpeed;
    void GetPointAtPos(C3DVector* out,float position,float time);
};
void CSplineRandomOffsetData::GetPointAtPos(C3DVector* out,float position,float time) {
    if(OffsetOnlyAtStartup) {ThisSpline.GetSplinePosition(*out,position);return;}
    long prevCount=PreviousSpline.GetControlPointCount();
    if(prevCount!=ThisSpline.GetControlPointCount()) {PreviousSpline=ThisSpline;ThisSpline.GetSplinePosition(*out,position);}
    else {C3DVector current;ThisSpline.GetSplinePosition(current,position);PreviousSpline.GetSplinePosition(*out,position);}
}