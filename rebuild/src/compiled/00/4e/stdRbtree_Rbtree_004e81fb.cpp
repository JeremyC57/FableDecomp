// std::_Rb_tree<EDamageAttribute, pair<const EDamageAttribute, CHeroStatIncreaseData>, ...>::~_Rb_tree() -- STLport 4.6 tree destructor:
// clear() (the out-of-line clear 0x004E7749), then _Rb_tree_base frees the header node
// Leaf: ??1?$_Rb_tree@W4EDamageAttribute@@U?$pair@$$CBW4EDamageAttribute@@VCHeroStatIncreaseData@@@std@@U?$_Select1st@U?$pair@$$CBW4EDamageAttribute@@VCHeroStatIncreaseData@@@std@@@3@U?$less@W4EDamageAttribute@@@3@V?$allocator@U?$pair@$$CBW4EDamageAttribute@@VCHeroStatIncreaseData@@@std@@@3@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E81FB, 23 bytes, a COMDAT in a size-optimised TU (`pop ecx`).
// Identity (NF90, q11179 follow-on, claimed through the lock): the destructor of the map<EDamageAttribute, CHeroStatIncreaseData> tree
//   (Ego_r.pdb CHeroExperienceDef::StatIncreasePerHitType): CHeroExperienceDef::~CHeroExperienceDef 0x004EBB1C calls it for that member, and
//   its callee 0x004E7749 is that tree's clear (NF90). Byte twin of 0x004E81E4 (NF81, the CComboMultiplierData tree destructor beside it);
//   the tree-destructor spelling follows that row (STLport _tree.h ~_Rb_tree() { clear(); } with the header free inlined).
//   Descriptive-by-library (STLport; both PDBs are Dinkumware). Was the stand-in std::list<T_004e81fb>::~list (leaf
//   ??1list_T_004e81fb@std@@QAE@XZ, binding 0x004E7749 as ?clear@list_T_004e81fb@std@@QAEXXZ).
// Port (NF90): one explicit instantiation on stlport/fable_map.h with FABLE_RETAIL_STLP_TREE_CLEAR_OUTOFLINE (the 0x004E81E4 recipe).
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_TREE_CLEAR_OUTOFLINE
#include "stlport/fable_map.h"
#include "engine/EDamageAttribute.h"
#include "engine/CHeroStatIncreaseData.h"

template std::_Rb_tree<EDamageAttribute, std::pair<const EDamageAttribute, CHeroStatIncreaseData>, std::_Select1st<std::pair<const EDamageAttribute, CHeroStatIncreaseData> >, std::less<EDamageAttribute>, std::allocator<std::pair<const EDamageAttribute, CHeroStatIncreaseData> > >::~_Rb_tree();
