// std::_Rb_tree<EDamageAttribute, pair<const EDamageAttribute, CHeroStatIncreaseData>, ...>::clear() -- STLport tree clear: when the tree
// holds nodes, erase them all (_M_erase from the root) and reset the header to an empty tree
// Leaf: ?clear@?$_Rb_tree@W4EDamageAttribute@@U?$pair@$$CBW4EDamageAttribute@@VCHeroStatIncreaseData@@@std@@U?$_Select1st@U?$pair@$$CBW4EDamageAttribute@@VCHeroStatIncreaseData@@@std@@@3@U?$less@W4EDamageAttribute@@@3@V?$allocator@U?$pair@$$CBW4EDamageAttribute@@VCHeroStatIncreaseData@@@std@@@3@@std@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E7749, 41 bytes, a COMDAT in a size-optimised TU (`and [x],0` stores).
// Identity (NF90, q11179 binder): the clear() of the map<EDamageAttribute, CHeroStatIncreaseData> tree (Ego_r.pdb
//   CHeroExperienceDef::StatIncreasePerHitType): its one call is that tree's _M_erase 0x004E6E43 (NF90); its callers are the same tree's
//   operator= 0x004F9831 and the member map's destructor 0x004E81FB. Byte twin of 0x004E7720 (NF78, the CComboMultiplierData tree, which
//   sits beside it), as 0x006DC9B0 is the speed-optimised map::clear twin of 0x006DC970. Descriptive-by-library (STLport; no PDB). Was
//   the AutoTiny stand-in _global::RbTree_Clear_004e7749 (leaf ?OnKill@@YIXPAUAutoTinyOwnedList@@@Z, binding 0x004E6E43 as
//   ?AutoTinyFreeListAllocation_004e6e43@@YGXPAX@Z).
// Port (NF90): one explicit instantiation of the STLport clear on stlport/fable_map.h (the 0x004E7720 recipe).
#pragma optimize("s", on)
#include "stlport/fable_map.h"
#include "engine/EDamageAttribute.h"
#include "engine/CHeroStatIncreaseData.h"
namespace std {
template void _Rb_tree<EDamageAttribute, pair<const EDamageAttribute, CHeroStatIncreaseData>, _Select1st<pair<const EDamageAttribute, CHeroStatIncreaseData> >, less<EDamageAttribute>, allocator<pair<const EDamageAttribute, CHeroStatIncreaseData> > >::clear();
}
