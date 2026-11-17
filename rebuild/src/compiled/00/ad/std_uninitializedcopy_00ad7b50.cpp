// std::__uninitialized_copy<COptimisedVertex*, COptimisedVertex*>(first, last, result, const __false_type&) -- STLport 4.6: copy-construct [first, last) into raw storage at result
// Leaf: ??$__uninitialized_copy@PAVCOptimisedVertex@QuadricOptimiserInternals@@PAV12@@std@@YIPAVCOptimisedVertex@QuadricOptimiserInternals@@PAV12@00ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AD7B50, 52 bytes, `ret 8` (first in ecx, last in edx; result + the tag on the stack). Callees: COptimisedVertex copy ctor 0x00AD77F0.
// NF72 (round 7, q10870): was the UninitCopy_COptimisedVertex_00ad7b50 stand-in (_global::). Descriptive-by-library: retail is STLport 4.6 (both PDBs are Dinkumware), and this is the
//   out-of-line __fastcall COMDAT of the STLport template that vector<QuadricOptimiserInternals::COptimisedVertex> uses; the landed callers _M_insert_overflow 0x00AD81F0 and _M_fill_insert 0x00AD85D0 (NF68) bind this symbol.
//   Re-authored as the explicit instantiation of the stlport/fable_containers.h body (FABLE_STLP_OUTOFLINE_UNINIT_COPY) under the macro block of the
//   landed 0x00AD85D0 (NF68), on engine/QuadricOptimiserTypes.h (the local COptimisedVertex / __false_type models are gone).
// Flags: /O2 /Oy /W3
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_DECL_ONLY
#define FABLE_RETAIL_STLP_RANDOM_ACCESS_COPY_BACKWARD
#define FABLE_RETAIL_STLP_COPY_BACKWARD_NOINLINE
#define FABLE_RETAIL_STLP_FILL_NOINLINE
#define FABLE_RETAIL_STLP_UNINITIALIZED_FILL_N_NOINLINE
#define FABLE_STLP_OUTOFLINE_UNINIT_COPY
#include "stlport/fable_containers.h"
#define FABLE_RETAIL_QUADRIC_CUNIQUEVERTEX
#define FABLE_RETAIL_QUADRIC_COPTIMISEDVERTEX_GROUPS
#include "engine/QuadricOptimiserTypes.h"

typedef QuadricOptimiserInternals::COptimisedVertex COptimisedVertex;

template COptimisedVertex* __fastcall std::__uninitialized_copy<COptimisedVertex*, COptimisedVertex*>(COptimisedVertex*, COptimisedVertex*, COptimisedVertex*, const std::__false_type&);
