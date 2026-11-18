// std::__copy_backward<COptimisedVertex*, COptimisedVertex*, int>(first, last, result, const random_access_iterator_tag&, int*) -- STLport 4.6: the counted backward copy
// Leaf: ??$__copy_backward@PAVCOptimisedVertex@QuadricOptimiserInternals@@PAV12@H@std@@YIPAVCOptimisedVertex@QuadricOptimiserInternals@@PAV12@00ABUrandom_access_iterator_tag@0@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AD7120, `ret 0Ch` (first in ecx, last in edx; result, the tag and the Distance* on the stack). Callee: COptimisedVertex::operator=.
// NF72 (round 7, q10871): was the CopyBackward_CElementB8_00ad7050_00ad7120 stand-in (_global::). Descriptive-by-library: retail is STLport 4.6 (both PDBs are Dinkumware), and this is the
//   out-of-line __fastcall COMDAT of the STLport template that vector<QuadricOptimiserInternals::COptimisedVertex> uses; the landed caller _M_fill_insert 0x00AD85D0 (NF68) binds this symbol.
//   Re-authored as the explicit instantiation of the stlport/fable_containers.h body (FABLE_RETAIL_STLP_COPY_BACKWARD_NOINLINE) under the macro block of the
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

template COptimisedVertex* __fastcall std::__copy_backward<COptimisedVertex*, COptimisedVertex*, int>(COptimisedVertex*, COptimisedVertex*, COptimisedVertex*, const std::random_access_iterator_tag&, int*);
