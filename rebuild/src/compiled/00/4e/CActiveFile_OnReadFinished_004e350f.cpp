// std::_Rb_tree<unsigned __int64, pair<const unsigned __int64, CGroupMemberData>, ...>::~_Rb_tree() -- (map<unsigned __int64, CGroupMemberData>, CTCCreatureGroup::Members) STLport 4.6 tree destructor: clear() (out of line), then free the header node (_Rb_tree_base / alloc_proxy)
// Leaf: ??1?$_Rb_tree@_KU?$pair@$$CB_KVCGroupMemberData@@@std@@U?$_Select1st@U?$pair@$$CB_KVCGroupMemberData@@@std@@@2@U?$less@_K@2@V?$allocator@U?$pair@$$CB_KVCGroupMemberData@@@std@@@2@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E350F, 23 bytes, thiscall (this in esi from ecx), plain `ret`; size-optimised TU (`pop ecx` after free).
// Identity (NF398; pair rule NF389, reference sites decide): two references. (1) ~CTCCreatureGroup 0x004E5C71 calls it on Members +0x18
//   (Ego_r.pdb map<unsigned __int64, CGroupMemberData> at +24; FableWin ~CTCCreatureGroup destroys that member); an evidence compile of
//   ~CTCCreatureGroup on engine/CTCCreatureGroup.h (probe/dtor_004e5c71.cpp) names ??1?$_Rb_tree at that call (VC7.1 inlines the
//   implicit ~map there) and emits this tree destructor as the same 23-byte body. (2) The unreferenced 5-byte `jmp 0x004E350F` at
//   0x004E409B is the implicit ~map the same compile emits (`jmp ??1?$_Rb_tree`). So this is ~_Rb_tree, not ~map. Its callee
//   0x004E1BD2 is the same tree's clear. STL instances have no PDB names: the spelling is the repo STLport's (descriptive-by-library).
// NF398: was the free stand-in @OnReadFinished_2@4 (catalog StdList_Dtor_004e350f; manifest CActiveFile::OnReadFinished, propagated)
//   calling the stand-in CActiveFile_helper_004e1bd2. Model: stlport/fable_map.h over engine/CGroupMemberData.h (the define set of
//   0x004E1BD2); the explicit instantiation emits the destructor as its own COMDAT (the 0x0042B7D7 form). clear and _M_erase are
//   declared only (FABLE_RETAIL_STLP_TREE_CLEAR_OUTOFLINE, FABLE_RETAIL_STLP_TREE_MERASE_DECL_ONLY): retail calls the owner 0x004E1BD2.
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_TREE_CLEAR_OUTOFLINE
#define FABLE_RETAIL_STLP_TREE_MERASE_DECL_ONLY
#include "stlport/fable_map.h"
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CThingAICreature.h"
#include "engine/CGroupMemberData.h"
template std::_Rb_tree<unsigned __int64, std::pair<const unsigned __int64, CGroupMemberData>, std::_Select1st<std::pair<const unsigned __int64, CGroupMemberData> >, std::less<unsigned __int64>, std::allocator<std::pair<const unsigned __int64, CGroupMemberData> > >::~_Rb_tree();
