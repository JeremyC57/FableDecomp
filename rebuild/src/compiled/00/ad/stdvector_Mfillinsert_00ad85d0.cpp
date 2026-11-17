// std::vector<QuadricOptimiserInternals::COptimisedVertex>::_M_fill_insert(COptimisedVertex* position, unsigned int n, const COptimisedVertex& x) -- STLport 4.6: insert n copies of x at position, preserving the old tail (through _M_insert_overflow when the capacity is short)
// Flags: /O2 /Oy /W3
// Leaf: ?_M_fill_insert@?$vector@VCOptimisedVertex@QuadricOptimiserInternals@@V?$allocator@VCOptimisedVertex@QuadricOptimiserInternals@@@std@@@std@@QAEXPAVCOptimisedVertex@QuadricOptimiserInternals@@IABV34@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AD85D0, 325 bytes, thiscall, `ret 0Ch`; reached from vector::resize (catalog resize_COptimisedVertex).
// Identity: STLport 4.6 stl/_vector.c vector<T>::_M_fill_insert (descriptive-by-library: retail is STLport, both PDBs are Dinkumware
//   builds); the element is QuadricOptimiserInternals::COptimisedVertex (stride 0xB8, engine/QuadricOptimiserTypes.h asserts it; copy
//   ctor 0x00AD77F0 and ~COptimisedVertex 0x00AD7400, FableWin.pdb names, out of line).
// Port (NF68, round 7): the namespaced N00ad85d0 stand-in (local vector, COptimisedVertex, trees, sets, Buffer / String models and
//   same-TU helper bodies) is replaced by stlport/fable_containers.h's vector body (FABLE_RETAIL_STLP_VECTOR) over the shared
//   COptimisedVertex (FABLE_RETAIL_QUADRIC_COPTIMISEDVERTEX_GROUPS). The four STLport helpers retail keeps out of line as __fastcall
//   COMDATs come from the header's same-TU noinline bodies: __uninitialized_copy (FABLE_STLP_OUTOFLINE_UNINIT_COPY, 0x00AD7B50 `ret 8`)
//   and the NF68 overlay opt-ins FABLE_RETAIL_STLP_COPY_BACKWARD_NOINLINE (0x00AD7120 `ret 0Ch`), FABLE_RETAIL_STLP_FILL_NOINLINE
//   (0x00AD78D0 `ret 4`) and FABLE_RETAIL_STLP_UNINITIALIZED_FILL_N_NOINLINE (0x00AD7B90 `ret 4`); _M_insert_overflow 0x00AD81F0 is
//   declaration-only (FABLE_RETAIL_STLP_VECTOR_OVERFLOW_DECL_ONLY).
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

template void std::vector<QuadricOptimiserInternals::COptimisedVertex>::_M_fill_insert(QuadricOptimiserInternals::COptimisedVertex*, unsigned int, const QuadricOptimiserInternals::COptimisedVertex&);
