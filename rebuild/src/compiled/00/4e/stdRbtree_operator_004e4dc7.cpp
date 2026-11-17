// std::_Rb_tree<long, pair<long const, long>, _Select1st<pair<long const, long> >, less<long >, allocator<pair<long const, long> > >::operator=(const _Rb_tree&) -- STLport 4.6 _tree.c tree assignment: unless self-assigned, clear 0x004DB91F, zero the count, then
//   reset the header to an empty tree or _M_copy 0x004E4E3A the source root under the header, walk leftmost / rightmost, copy the count
// Leaf: ??4?$_Rb_tree@JU?$pair@$$CBJJ@std@@U?$_Select1st@U?$pair@$$CBJJ@std@@@2@U?$less@J@2@V?$allocator@U?$pair@$$CBJJ@std@@@2@@std@@QAEAAV01@ABV01@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E4DC7, 115 bytes, size TU (`and dword ptr [..],0`), thiscall `ret 4`.
// NF64 (round 7, q10707): an explicit instantiation of stlport/fable_map.h operator= (overlay opt-in FABLE_RETAIL_STLP_TREE_ASSIGN_BODY:
//   STLport's _tree.c text, _M_copy declared only) for the tree whose _M_copy 0x004E4E3A NF61 re-authored; the value-type declarations
//   are that row's. It now binds _M_copy and clear under their STLport spellings. Was the stand-in `CRetailTree_004e4e3a::operator=` (bound
//   ?Copy@CRetailTree_004e4e3a / ?Clear@CRetailTree_004e4e3a). Descriptive-by-library (retail is STLport 4.6; both PDBs are Dinkumware builds; FableWin.pdb
//   lists the class-level ??4CHeroAbilityStatWeight at 0x0191AD74, 31 bytes before the next symbol, as its own function).
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_TREE_ASSIGN_BODY
#include "stlport/fable_map.h"

namespace std {
template _Rb_tree<long, pair<long const, long>, _Select1st<pair<long const, long> >, less<long >, allocator<pair<long const, long> > >& _Rb_tree<long, pair<long const, long>, _Select1st<pair<long const, long> >, less<long >, allocator<pair<long const, long> > >::operator=(const _Rb_tree<long, pair<long const, long>, _Select1st<pair<long const, long> >, less<long >, allocator<pair<long const, long> > >&);
}
