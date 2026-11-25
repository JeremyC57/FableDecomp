// std::_Rb_tree<unsigned __int64, pair<const unsigned __int64, CGroupMemberData>, ...>::erase(iterator first, iterator last) -- (map<unsigned __int64, CGroupMemberData>, CTCCreatureGroup::Members) STLport 4.6 _tree.c range erase: clear() when the range is the whole tree, else erase(first++) node by node
// Leaf: ?erase@?$_Rb_tree@_KU?$pair@$$CB_KVCGroupMemberData@@@std@@U?$_Select1st@U?$pair@$$CB_KVCGroupMemberData@@@std@@@2@U?$less@_K@2@V?$allocator@U?$pair@$$CB_KVCGroupMemberData@@@std@@@2@@std@@QAEXU?$_Rb_tree_iterator@U?$pair@$$CB_KVCGroupMemberData@@@std@@U?$_Nonconst_traits@U?$pair@$$CB_KVCGroupMemberData@@@std@@@2@@2@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00813460, 196 bytes (+ int3 padding), `ret 8` (thiscall, this -> ebx; first, last by value).
// Identity: sole reference the call at 0x00813684 in erase(const key&) 0x00813640 (STLport `equal_range ; distance ; erase(first,
//   last)`), whose landed row binds this address under exactly this spelling; CTCCreatureGroup::Members = map<unsigned __int64,
//   CGroupMemberData> (Ego_r.pdb; node 0x30: key +0x10, CGroupMemberData +0x18 = PMember, IsActive +0x20, JoinedFrame +0x24,
//   DeactivatedFrame +0x28). STL instances have no PDB names: the spelling is the repo STLport's (descriptive-by-library).
// Callees: _M_erase 0x004DFB8F (this tree's, out of line), _Rb_global<bool>::_Rebalance_for_erase 0x0042A34E (__cdecl),
//   ~CBaseIntelligentPointer 0x00A01C10 (the value's PMember), free 0x00BFEA14 (the node allocator's deallocate).
// Shape: STLport 4.6 _tree.c `erase(iterator, iterator)`: clear() and erase(iterator) inline, `erase(first++)` with the successor
//   walk inline (_Rb_tree_base_iterator::_M_increment).
// NF398: moved off the row-local four-argument tree / iterator / CGroupMemberData models (whose self and _M_erase callee carried
//   four-argument spellings) onto stlport/fable_map.h (the 0x004B9C10 / 0x00756420 recipe: FABLE_RETAIL_STLP_TREE_ERASE + _ERASE_BODY
//   + _POSTINC + _ERASE_KEY, explicit instantiation) and engine/CGroupMemberData.h (the CThingAICreature chain of 0x004E1BD2, the same
//   tree's clear); _M_erase is declared only (FABLE_RETAIL_STLP_TREE_MERASE_DECL_ONLY): retail calls the owner 0x004DFB8F.
// Pick-any copies the TU emits beside the leaf, given their owners' bodies (leaf unchanged with or without each, RELOCATION_MATCH):
//   FABLE_RETAIL_STLP_RB_INCREMENT_SIZE_OWNER_PASS27 (_Rb_global<bool>::_M_increment = owner 0x004292C0, 44 bytes, not the /O2 54),
//   FABLE_RETAIL_STLP_GROUP_MEMBER_TREE_CLEAR_SIZE_OWNER (engine/CGroupMemberData.h: clear = owner 0x004E1BD2, 41 bytes, not the /O2 51),
//   FABLE_RETAIL_STLP_DESTROY_FASTCALL (stlport/fable_containers.h: _Destroy<pair<..>> = retail 0x004DD6F3, the __fastcall 8 bytes) with
//   FABLE_RETAIL_STLP_GROUP_MEMBER_DESTROY_SIZE_OWNER (engine/CGroupMemberData.h: that _Destroy under optimize("s"), so it and the
//   ??_G?$pair it calls are the /Os bodies of 0x004DD6F3 and 0x004DBE39, 8 and 31 bytes; the plain /O2 ??_G copy is 33 bytes).
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_ERASE
#define FABLE_RETAIL_STLP_TREE_ERASE_BODY
#define FABLE_RETAIL_STLP_TREE_POSTINC
#define FABLE_RETAIL_STLP_TREE_ERASE_KEY
#define FABLE_RETAIL_STLP_TREE_MERASE_DECL_ONLY
#define FABLE_RETAIL_STLP_RB_INCREMENT_SIZE_OWNER_PASS27
#define FABLE_RETAIL_STLP_DESTROY_FASTCALL
#define FABLE_RETAIL_STLP_GROUP_MEMBER_TREE_CLEAR_SIZE_OWNER
#define FABLE_RETAIL_STLP_GROUP_MEMBER_DESTROY_SIZE_OWNER
#include "stlport/fable_map.h"
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CThingAICreature.h"
#include "engine/CGroupMemberData.h"
template void std::_Rb_tree<unsigned __int64, std::pair<const unsigned __int64, CGroupMemberData>, std::_Select1st<std::pair<const unsigned __int64, CGroupMemberData> >, std::less<unsigned __int64>, std::allocator<std::pair<const unsigned __int64, CGroupMemberData> > >::erase(std::_Rb_tree_iterator<std::pair<const unsigned __int64, CGroupMemberData>, std::_Nonconst_traits<std::pair<const unsigned __int64, CGroupMemberData> > >, std::_Rb_tree_iterator<std::pair<const unsigned __int64, CGroupMemberData>, std::_Nonconst_traits<std::pair<const unsigned __int64, CGroupMemberData> > >);
