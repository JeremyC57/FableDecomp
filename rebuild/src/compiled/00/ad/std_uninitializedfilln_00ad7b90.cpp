// std::uninitialized_fill_n<COptimisedVertex*, unsigned int, COptimisedVertex>(first, n, x) -- STLport 4.6: copy-construct n copies of x into raw storage
// Leaf: ??$uninitialized_fill_n@PAVCOptimisedVertex@QuadricOptimiserInternals@@IV12@@std@@YIPAVCOptimisedVertex@QuadricOptimiserInternals@@PAV12@IABV12@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AD7B90, `ret 4` (first in ecx, n in edx, x on the stack: the public three-argument form). Callee: COptimisedVertex copy ctor 0x00AD77F0.
// NF72 (round 7, q10872): was the UninitFillN_CElement184_00ad77f0_00ad7b90 stand-in (_global::, no Leaf line). Descriptive-by-library: retail is STLport 4.6 (both PDBs are Dinkumware), and this is the
//   out-of-line __fastcall COMDAT of the STLport template that vector<QuadricOptimiserInternals::COptimisedVertex> uses; the landed caller _M_fill_insert 0x00AD85D0 (NF68) binds this symbol.
//   Re-authored as the explicit instantiation of the stlport/fable_containers.h body (FABLE_RETAIL_STLP_UNINITIALIZED_FILL_N_NOINLINE) under the macro block of the
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

template COptimisedVertex* __fastcall std::uninitialized_fill_n<COptimisedVertex*, unsigned int, COptimisedVertex>(COptimisedVertex*, unsigned int, const COptimisedVertex&);
