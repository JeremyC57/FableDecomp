// std::_Rb_tree<long, pair<const long, CComboMultiplierData>, ...>::~_Rb_tree() -- STLport 4.6 tree destructor: clear() (the out-of-line
// clear 0x004E7720), then _Rb_tree_base frees the header node
// Leaf: ??1?$_Rb_tree@JU?$pair@$$CBJVCComboMultiplierData@@@std@@U?$_Select1st@U?$pair@$$CBJVCComboMultiplierData@@@std@@@2@U?$less@J@2@V?$allocator@U?$pair@$$CBJVCComboMultiplierData@@@std@@@2@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E81E4, 23 bytes, a COMDAT in a size-optimised TU (`pop ecx`).
// Identity (NF81, round 7, q10975): the destructor of the map<long, CComboMultiplierData> tree (Ego_r.pdb
//   CHeroExperienceDef::ComboMultiplierMap): CHeroExperienceDef::~CHeroExperienceDef 0x004EBB1C calls it for that member, and its
//   callee 0x004E7720 is that tree's clear (NF78 q10949, RM; its other caller is the same tree's operator= 0x004F9660). The body is
//   STLport 4.6 _tree.h ~_Rb_tree() { clear(); } with ~_Rb_tree_base's header deallocate (free when non-null) inlined, as the landed
//   tree destructors 0x006F1BA0 / 0x0056D3D0 are named. Whether VC7.1 emitted this COMDAT as map's implicit destructor (with ~_Rb_tree
//   inlined) or as ~_Rb_tree itself is not provable from the bytes; the tree destructor is the repo's spelling for this body.
//   Descriptive-by-library (STLport; both PDBs are Dinkumware). Was the stand-in std::list<T_004e81e4>::~list (a list destructor
//   over an unrecoverable node type, leaf ??1list_T_004e81e4@std@@).
// Port (NF81): one explicit instantiation on stlport/fable_map.h (the 0x006F1BA0 recipe) with FABLE_RETAIL_STLP_TREE_CLEAR_OUTOFLINE
//   (clear called, not expanded, as in the 0x004E7720 / 0x00593235 TUs).
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_TREE_CLEAR_OUTOFLINE
#include "stlport/fable_map.h"
#include "engine/CComboMultiplierData.h"

template std::_Rb_tree<long, std::pair<const long, CComboMultiplierData>, std::_Select1st<std::pair<const long, CComboMultiplierData> >, std::less<long>, std::allocator<std::pair<const long, CComboMultiplierData> > >::~_Rb_tree();
