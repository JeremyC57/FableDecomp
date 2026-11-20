// CCardinalSpline::AppendControlPoint(const C3DVector& point) -- push a control point onto the spline's point array and return its index
// Leaf: ?AppendControlPoint@CCardinalSpline@@QAEJABVC3DVector@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0075F980, 92 bytes, `ret 4`. No direct caller or reference (an
//   unreferenced out-of-line copy kept without /OPT:REF).
// Callee: vector<C3DVector>::_M_insert_overflow(pos, x, tag, 1, true) 0x0066E370 (the shared C3DVector growth
//   path, 65 call sites).
// Identity: Ego_r.pdb names `long CCardinalSpline::AppendControlPoint(const C3DVector&)` (?AppendControlPoint@
//   CCardinalSpline@@QAEJABVC3DVector@@@Z), the only PDB method returning long from a const C3DVector& on a class
//   whose first member is a C3DVector array (CCardinalSpline::ControlPoints, CArray<C3DVector> +0; the retail
//   STLport vector is 12 bytes). The catalog's "Vector_PushBackOrInsert" is a descriptive auto label.
// Shape: STLport `push_back` inlined (`_Construct` = placement new with its null test, three dword moves, `add
//   [esi+4],0Ch`; else `_M_insert_overflow(_M_finish, x, tag, 1, true)` with the empty tag homed in the dead `point`
//   slot), then `size() - 1` from the signed 0x2AAAAAAB divide by 12.
// Names (NF119, PRIORITY q6407): the growth path is STLport 4.6 vector<C3DVector>::_M_insert_overflow(pointer, const C3DVector&,
//   const __false_type&, size_type, bool) (protected; C3DVector has a user copy constructor, so __type_traits gives __false_type), the
//   spelling its self row 0x0066E370 and 16 landed callers bind; was a local CArray model with a __true_type tag. Now the shared
//   engine/CCardinalSpline.h AppendControlPoint body (FABLE_RETAIL_CCARDINALSPLINE_APPENDCONTROLPOINT, the QueueSwoop 0x007DDA30 opt-in)
//   over engine/CArray.h / stlport/fable_containers.h, the overflow declared only (VECTOR_OVERFLOW_DECL_ONLY: its owner 0x0066E370 defines it).
#define FABLE_RETAIL_CCARDINALSPLINE_APPENDCONTROLPOINT
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_DECL_ONLY
#include "fable_compiler.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CCardinalSpline.h"

namespace {
// Fidelity: scaffolding, not retail: the header's in-class AppendControlPoint body (an implicitly inline member) is emitted by this
// object as its pick-any COMDAT through the member's address, the form of retail's unreferenced out-of-line copy. Internal to this TU.
long (CCardinalSpline::*ReferenceAppendControlPoint())(const C3DVector&)
{
    return &CCardinalSpline::AppendControlPoint;
}
}
