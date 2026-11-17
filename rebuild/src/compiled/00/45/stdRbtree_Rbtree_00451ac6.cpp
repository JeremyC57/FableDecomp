// std::_Rb_tree<EDamageAttribute, pair<const EDamageAttribute, float>, ...>::~_Rb_tree() -- STLport 4.6 tree destructor: clear() (the
// out-of-line clear 0x0045128D), then _Rb_tree_base frees the header node
// Leaf: ??1?$_Rb_tree@W4EDamageAttribute@@U?$pair@$$CBW4EDamageAttribute@@M@std@@U?$_Select1st@U?$pair@$$CBW4EDamageAttribute@@M@std@@@3@U?$less@W4EDamageAttribute@@@3@V?$allocator@U?$pair@$$CBW4EDamageAttribute@@M@std@@@3@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00451AC6, 23 bytes, a COMDAT in a size-optimised TU (`pop ecx`).
// Identity (NF83, round 7, q11011 binder): the destructor of the map<EDamageAttribute, float> tree: its callee 0x0045128D is that tree's
//   clear (NF81, RM; the landed tree operator= 0x004614C0 binds it so), and the pair<const long, CComboMultiplierData> scalar deleting
//   destructor 0x004E3625 (NF83, RM) compiles its +4 member destruction (CComboMultiplierData::ComboMultiplier) to a call of this
//   symbol bound here. Byte-for-byte the size-TU twin of the CComboMultiplierData tree destructor 0x004E81E4 (NF81). Its other
//   retail callers destroy the same map members (CHitParameters 0x00789358, 0x004585BD, 0x0045356D, 0x004E00ED, 0x004E1C4F).
//   Descriptive-by-library (STLport; both PDBs are Dinkumware builds). Was the stand-in _global::StdList_Dtor_00451ac6 (leaf
//   CActiveFile_OnReadFinished, a free __fastcall over a local CActiveFile model calling `CActiveFile_helper`).
//   Note: the landed 0x006DC020 (an unreferenced speed-TU COMDAT) also defines this leaf (NOTES / NAME_FIXES).
// Port (NF83): one explicit instantiation on stlport/fable_map.h (the 0x004E81E4 recipe) with FABLE_RETAIL_STLP_TREE_CLEAR_OUTOFLINE.
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_TREE_CLEAR_OUTOFLINE
#include "engine/EDamageAttribute.h"
#include "stlport/fable_map.h"

template std::_Rb_tree<EDamageAttribute, std::pair<const EDamageAttribute, float>, std::_Select1st<std::pair<const EDamageAttribute, float> >, std::less<EDamageAttribute>, std::allocator<std::pair<const EDamageAttribute, float> > >::~_Rb_tree();
