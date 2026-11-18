// std::__unguarded_linear_insert(pair<unsigned long, C3DMeshVolume>* last, pair<unsigned long, C3DMeshVolume> val, CKeyPairCompareLess<unsigned long, C3DMeshVolume> comp) -- insertion-sort inner step without a lower bound check for the mesh-volume table: shift every larger entry up one slot, drop the value into the gap, destroy the by-value copy
// Leaf: ??$__unguarded_linear_insert@PAU?$pair@KVC3DMeshVolume@@@std@@U12@V?$CKeyPairCompareLess@KVC3DMeshVolume@@@@@std@@YIXPAU?$pair@KVC3DMeshVolume@@@0@U10@V?$CKeyPairCompareLess@KVC3DMeshVolume@@@@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A33250, 200 bytes, `ret 14h` (__fastcall: last in ecx, the 16-byte value and
//   the empty comparator on the stack).
// Identity: called by __unguarded_insertion_sort_aux 0x00A33870 and __linear_insert 0x00A34820 (the C3DMeshStats volume-table
//   sort). Callees: CCharString::operator= 0x0099EFB0, the volume's implicit operator= COMDAT 0x00A32960, operator delete
//   0x00BFE9BC (the counted pointer's release), CCharString::~CCharString 0x0099EAE0.
// NF91 (2026-11-17, q11194): moved off the row-local CCharString / C3DMeshVolume / std::pair / std::CKeyPairCompareLess models onto
//   the shared headers (engine/C3DMeshVolume.h element, engine/CVectorMap.h global comparator, the STLport 4.6 body of
//   stlport/fable_containers.h FABLE_RETAIL_STLP_UNGUARDED_LINEAR_INSERT_BODY, the 0x004DD80B recipe) by explicit instantiation;
//   `// Flags: /Gr` (the algorithm TU; the old row spelt __fastcall by hand). STLport instance name: descriptive-by-library.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_STLP_SORT_INTERNALS
#define FABLE_RETAIL_STLP_UNGUARDED_LINEAR_INSERT_BODY
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/C3DMeshVolume.h"
#include "engine/CVectorMap.h"
template void FABLE_STLP_FASTCALL std::__unguarded_linear_insert(std::pair<unsigned long,C3DMeshVolume>*,std::pair<unsigned long,C3DMeshVolume>,CKeyPairCompareLess<unsigned long,C3DMeshVolume>);
