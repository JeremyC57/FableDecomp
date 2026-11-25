// std::_Rb_tree<unsigned __int64, pair<const unsigned __int64, CGroupMemberData>, ...>::_M_erase(x) -- (map<unsigned __int64, CGroupMemberData>, CTCCreatureGroup::Members) STLport 4.6 _tree.c: erase the subtree under x without rebalancing (recurse right, walk left, destroy each value, free each node)
// Leaf: ?_M_erase@?$_Rb_tree@_KU?$pair@$$CB_KVCGroupMemberData@@@std@@U?$_Select1st@U?$pair@$$CB_KVCGroupMemberData@@@std@@@2@U?$less@_K@2@V?$allocator@U?$pair@$$CB_KVCGroupMemberData@@@std@@@2@@std@@AAEXPAU?$_Rb_tree_node@U?$pair@$$CB_KVCGroupMemberData@@@std@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DFB8F, 55 bytes, thiscall (this -> ebx), `ret 4`; size-optimised TU.
// Identity (NF396 probe, NF398 re-check): three references, all direct calls: the recursion 0x004DFBA1, the tree's clear 0x004E1BD2
//   (0x004E1BE0, on the root; that clear's only caller is the tree destructor 0x004E350F, which ~CTCCreatureGroup 0x004E5C71 calls on
//   Members +0x18 = Ego_r.pdb map<unsigned __int64, CGroupMemberData> at +24) and the range erase 0x00813460 (0x0081348D, on the root,
//   inside its inlined clear; its caller erase(const key&) 0x00813640 is landed under this tree's STLport spelling). The per-node
//   value destructor it calls is ??_G of pair<const unsigned __int64, CGroupMemberData> (0x004DBE39, flag 0: ~CBaseIntelligentPointer
//   0x00A01C10 on node +0x18 = PMember). STL instances have no PDB names: the spelling is the repo STLport's (descriptive-by-library).
// NF398: was the stand-in ?BuildTreeArray@LTextTreeWalkThrough@@QAEXPAUCTextTreeNode@@@Z with a local node model (catalog
//   LTextTreeWalkThrough::BuildTreeArray, propagated). Explicit instantiation of the stlport/fable_map.h member over
//   engine/CGroupMemberData.h (the 0x00527950 form; the CThingAICreature chain is the define set of 0x004E1BD2, the same tree's clear).
// Opt-in FABLE_RETAIL_STLP_DESTROY_FASTCALL (stlport/fable_containers.h): the TU's pick-any _Destroy<pair<..>> copy is the __fastcall one
//   retail keeps at 0x004DD6F3 (`push 0 ; call 0x004DBE39 ; ret`, 8 bytes); the cdecl default (12 bytes) has no retail copy.
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_DESTROY_FASTCALL
#include "stlport/fable_map.h"
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CThingAICreature.h"
#include "engine/CGroupMemberData.h"
namespace std {
template void _Rb_tree<unsigned __int64, pair<const unsigned __int64, CGroupMemberData>, _Select1st<pair<const unsigned __int64, CGroupMemberData> >, less<unsigned __int64>, allocator<pair<const unsigned __int64, CGroupMemberData> > >::_M_erase(_Rb_tree_node<pair<const unsigned __int64, CGroupMemberData> >*);
}
