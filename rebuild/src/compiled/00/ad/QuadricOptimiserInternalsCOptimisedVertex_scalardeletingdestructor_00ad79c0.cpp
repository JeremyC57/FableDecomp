// QuadricOptimiserInternals::COptimisedVertex::`scalar deleting destructor'(unsigned int flags) -- run ~COptimisedVertex, free the block when bit 0 of the flags is set, return this
// Leaf: ??_GCOptimisedVertex@QuadricOptimiserInternals@@QAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AD79C0, 30 bytes (`ret 4`).
// Name (round 7, NF53): VC7.1 emits a scalar deleting destructor (??_G..@QAEPAXI@Z) for a class with a non-virtual destructor when an
//   object is deleted (checked with a scratch TU); the callee 0x00AD7400 is ??1COptimisedVertex@QuadricOptimiserInternals@@QAE@XZ
//   (Ego_r.pdb; the landed 0x00AD7400 row names this function as its scalar deleting destructor), on engine/QuadricOptimiserTypes.h
//   (FABLE_RETAIL_QUADRIC_COPTIMISEDVERTEX_GROUPS declares it out of line). Was the free stand-in
//   CLandscapeBackgroundPatch_vector_deleting_destructor over a local CLandscapeBackgroundPatch and CLandscapeBackgroundPatch_dtor_00ad7400.
#define FABLE_RETAIL_QUADRIC_CUNIQUEVERTEX
#define FABLE_RETAIL_QUADRIC_COPTIMISEDVERTEX_GROUPS
#include "engine/QuadricOptimiserTypes.h"

void Kill_00ad79c0(QuadricOptimiserInternals::COptimisedVertex* object)   // forces the scalar deleting destructor into the object; not part of the retail body
{
    delete object;
}
