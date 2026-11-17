// std::__linear_insert(pair<unsigned long, C3DMeshVolume>* first, pair<unsigned long, C3DMeshVolume>* last, pair<unsigned long, C3DMeshVolume> val, CKeyPairCompareLess<unsigned long, C3DMeshVolume> comp) -- insertion-sort step: a value smaller than the first entry shifts the run up one slot and goes first, anything else goes through the unguarded insert; the by-value val is destroyed
// Leaf: ??$__linear_insert@PAU?$pair@KVC3DMeshVolume@@@std@@U12@V?$CKeyPairCompareLess@KVC3DMeshVolume@@@@@std@@YIXPAU?$pair@KVC3DMeshVolume@@@0@0U10@V?$CKeyPairCompareLess@KVC3DMeshVolume@@@@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A34820, 179 bytes, `ret 14h` (__fastcall: first in ecx, last in edx, the
//   16-byte value and the empty comparator on the stack).
// Identity: called by __insertion_sort 0x00A34D10 (the C3DMeshStats volume-table sort). Callees: __copy_backward 0x00A33050, the
//   volume's implicit operator= COMDAT 0x00A32960, CCharString::CCharString(const CCharString&) 0x0099EC30,
//   __unguarded_linear_insert 0x00A33250, operator delete 0x00BFE9BC, CCharString::~CCharString 0x0099EAE0.
// NF91 (2026-11-17, q11193): moved off the row-local CCharString / C3DMeshVolume / std::pair / std::CKeyPairCompareLess models onto
//   the shared headers (engine/C3DMeshVolume.h, engine/CVectorMap.h, the STLport 4.6 body of stlport/fable_containers.h
//   FABLE_RETAIL_STLP_SORT_INTERNALS, the 0x00808AA0 recipe) by explicit instantiation; `// Flags: /Gr`. STLport instance name:
//   descriptive-by-library.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_STLP_SORT_INTERNALS
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/C3DMeshVolume.h"
#include "engine/CVectorMap.h"
template void FABLE_STLP_FASTCALL std::__linear_insert(std::pair<unsigned long,C3DMeshVolume>*,std::pair<unsigned long,C3DMeshVolume>*,std::pair<unsigned long,C3DMeshVolume>,CKeyPairCompareLess<unsigned long,C3DMeshVolume>);
