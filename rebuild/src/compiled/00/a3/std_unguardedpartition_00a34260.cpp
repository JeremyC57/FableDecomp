// std::__unguarded_partition(pair<unsigned long, C3DMeshVolume>* first, pair<unsigned long, C3DMeshVolume>* last, pair<unsigned long, C3DMeshVolume> pivot, CKeyPairCompareLess<unsigned long, C3DMeshVolume> comp) -- quicksort partition step of the mesh-volume table around the pivot's key: advance from both ends, swap out-of-place entries, return the split point, destroy the by-value pivot
// Leaf: ??$__unguarded_partition@PAU?$pair@KVC3DMeshVolume@@@std@@U12@V?$CKeyPairCompareLess@KVC3DMeshVolume@@@@@std@@YIPAU?$pair@KVC3DMeshVolume@@@0@PAU10@0U10@V?$CKeyPairCompareLess@KVC3DMeshVolume@@@@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A34260, 127 bytes, `ret 14h` (__fastcall: first in ecx, last in edx, the
//   16-byte pivot and the empty comparator on the stack).
// Identity: called by __introsort_loop 0x00A35490. Callees: swap(pair&, pair&) @ 0x00A331B0 (landed, out of line, a in ecx / b in
//   edx), the count block's DeleteFunc, operator delete @ 0x00BFE9BC, CCharString::~CCharString @ 0x0099EAE0.
// NF89 (2026-11-17, q11174): moved off the row-local CCharString / C3DMeshVolume / std::pair / std::CKeyPairCompareLess / TEntry models onto
//   the shared headers, as NF85 / NF87 did for the rest of the family (0x00A34420 __pop_heap, 0x00A344E0 __make_heap, 0x00A34D80
//   sort_heap, 0x00A35260 __partial_sort): the element is the STLport pair<unsigned long, C3DMeshVolume> (engine/C3DMeshVolume.h), the
//   comparator the global CKeyPairCompareLess<unsigned long, C3DMeshVolume> (engine/CVectorMap.h), and the STLport 4.6 _algo.c body is an
//   explicit instantiation of stlport/fable_containers.h FABLE_RETAIL_STLP_SORT_INTERNALS (iter_swap = swap(*a, *b), out of line), the
//   landed 0x0077E8F0 recipe. STLport instance name: descriptive-by-library.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_STLP_SORT_INTERNALS
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/C3DMeshVolume.h"
#include "engine/CVectorMap.h"
template std::pair<unsigned long,C3DMeshVolume>* FABLE_STLP_FASTCALL std::__unguarded_partition(std::pair<unsigned long,C3DMeshVolume>*,std::pair<unsigned long,C3DMeshVolume>*,std::pair<unsigned long,C3DMeshVolume>,CKeyPairCompareLess<unsigned long,C3DMeshVolume>);
