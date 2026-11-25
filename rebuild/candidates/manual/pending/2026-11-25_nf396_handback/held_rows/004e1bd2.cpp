// std::_Rb_tree<unsigned __int64, pair<const unsigned __int64, CGroupMemberData>, ...>::clear() -- (map<unsigned __int64, CGroupMemberData>, CTCCreatureGroup::Members) STLport 4.6 _tree.h clear: when the tree has nodes, erase them from the root, reset the header's leftmost / root / rightmost, zero the count
// Leaf: ?clear@?$_Rb_tree@_KU?$pair@$$CB_KVCGroupMemberData@@@std@@U?$_Select1st@U?$pair@$$CB_KVCGroupMemberData@@@std@@@2@U?$less@_K@2@V?$allocator@U?$pair@$$CB_KVCGroupMemberData@@@std@@@2@@std@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E1BD2, 41 bytes, thiscall (this in esi from ecx), plain `ret`; size-optimised TU.
// Identity (NF391 trace, NF396 re-check): its only reference is the call at 0x004E3512 in 0x004E350F (`call clear ; free(header)`), the
//   tree destructor of CTCCreatureGroup::Members (Ego_r.pdb map<unsigned __int64, CGroupMemberData> at +24 = retail +0x18; FableWin
//   ~CTCCreatureGroup destroys that member); a tree destructor calls the tree's own clear (STLport `~_Rb_tree() { clear(); }`), never
//   map::clear, so the caller path names the _Rb_tree member. The body calls _M_erase 0x004DFB8F on the root. Both PDBs are Dinkumware
//   builds, so the spelling is the one the repo STLport emits (descriptive-by-library).
// NF396: was the free stand-in OnKill(AutoTinyOwnedList*) calling AutoTinyFreeListAllocation_004dfb8f (catalog RbTree_Clear, manifest
//   CTCCarryable::OnKill, propagated). Model: the shared stlport/fable_map.h body over engine/CGroupMemberData.h (its inline constructor needs the
//   CThingAICreature chain, the opt-ins of the landed 0x00813010 CTCCreatureGroup::AddNewMember that the chain needs), as 0x00579EA3;
//   _M_erase is declared only (FABLE_RETAIL_STLP_TREE_MERASE_DECL_ONLY), so the TU emits no copy of 0x004DFB8F.
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_TREE_MERASE_DECL_ONLY
#include "stlport/fable_map.h"
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CThingAICreature.h"
#include "engine/CGroupMemberData.h"
namespace std {
template void _Rb_tree<unsigned __int64, pair<const unsigned __int64, CGroupMemberData>, _Select1st<pair<const unsigned __int64, CGroupMemberData> >, less<unsigned __int64>, allocator<pair<const unsigned __int64, CGroupMemberData> > >::clear();
}
