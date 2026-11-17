// std::_Rb_tree<EQuakeLength, pair<EQuakeLength const, float>, _Select1st<pair<EQuakeLength const, float> >, less<EQuakeLength >, allocator<pair<EQuakeLength const, float> > >::operator=(const _Rb_tree&) -- STLport 4.6 _tree.c tree assignment: unless self-assigned, clear 0x004DBCD4, zero the count, then
//   reset the header to an empty tree or _M_copy 0x004E66B8 the source root under the header, walk leftmost / rightmost, copy the count
// Leaf: ??4?$_Rb_tree@W4EQuakeLength@@U?$pair@$$CBW4EQuakeLength@@M@std@@U?$_Select1st@U?$pair@$$CBW4EQuakeLength@@M@std@@@3@U?$less@W4EQuakeLength@@@3@V?$allocator@U?$pair@$$CBW4EQuakeLength@@M@std@@@3@@std@@QAEAAV01@ABV01@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E6645, 115 bytes, size TU (`and dword ptr [..],0`), thiscall `ret 4`.
// NF64 (round 7, q10714): an explicit instantiation of stlport/fable_map.h operator= (overlay opt-in FABLE_RETAIL_STLP_TREE_ASSIGN_BODY:
//   STLport's _tree.c text, _M_copy declared only) for the tree whose _M_copy 0x004E66B8 NF61 re-authored; the value-type declarations
//   are that row's. It now binds _M_copy and clear under their STLport spellings. Was the stand-in `CRetailTree_004e66b8::operator=` (bound
//   ?Copy@CRetailTree_004e66b8 / ?Clear@CRetailTree_004e66b8). Descriptive-by-library (retail is STLport 4.6; both PDBs are Dinkumware builds; FableWin.pdb
//   lists the class-level ??4CHeroAbilityStatWeight at 0x0191AD74, 31 bytes before the next symbol, as its own function).
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_TREE_ASSIGN_BODY
#include "stlport/fable_map.h"
#include "engine/NCreatureActionEvents.h"
namespace std {
template _Rb_tree<EQuakeLength, pair<EQuakeLength const, float>, _Select1st<pair<EQuakeLength const, float> >, less<EQuakeLength >, allocator<pair<EQuakeLength const, float> > >& _Rb_tree<EQuakeLength, pair<EQuakeLength const, float>, _Select1st<pair<EQuakeLength const, float> >, less<EQuakeLength >, allocator<pair<EQuakeLength const, float> > >::operator=(const _Rb_tree<EQuakeLength, pair<EQuakeLength const, float>, _Select1st<pair<EQuakeLength const, float> >, less<EQuakeLength >, allocator<pair<EQuakeLength const, float> > >&);
}
