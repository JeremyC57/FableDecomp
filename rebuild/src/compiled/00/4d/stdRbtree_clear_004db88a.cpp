// std::_Rb_tree<long, pair<const long, CDefString>, _Select1st<..>, less<long>, allocator<..> >::clear() -- STLport 4.6 _tree.h: when the
//   tree holds nodes, erase them from the root (_M_erase 0x004DA27F), relink the header to itself and zero the count
// Leaf: ?clear@?$_Rb_tree@JU?$pair@$$CBJVCDefString@@@std@@U?$_Select1st@U?$pair@$$CBJVCDefString@@@std@@@2@U?$less@J@2@V?$allocator@U?$pair@$$CBJVCDefString@@@std@@@2@@std@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DB88A, size TU.
// Identity (NF93, q11244): the retail callers are the ported ~_Rb_tree 0x004DD21C (NF92, explicit instantiation with TREE_CLEAR_OUTOFLINE:
//   its only call is this address, bound under this spelling) and the ported tree operator= 0x004F9207 (NF92: clear() first); the body is
//   STLport's clear verbatim (count test, _M_erase(root), leftmost / root / rightmost reset, count = 0) over the map<long, CDefString>
//   tree of the CHeroExperienceDef members (Ego_r.pdb). Descriptive-by-library (STLport; both PDBs are Dinkumware builds). Was the
//   propagated stand-in _global::RbTree_Clear_004db88a (leaf OnKill over a local AutoTinyOwnedList model, callee
//   AutoTinyFreeListAllocation_004da27f).
// Port (NF93): one explicit instantiation of stlport/fable_map.h's inline clear (default path), the 0x004DD21C recipe.
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#include "fable_string.h"
#include "stlport/fable_map.h"
#include "engine/CDefString.h"
template void std::_Rb_tree<long, std::pair<const long, CDefString>, std::_Select1st<std::pair<const long, CDefString> >, std::less<long>, std::allocator<std::pair<const long, CDefString> > >::clear();
