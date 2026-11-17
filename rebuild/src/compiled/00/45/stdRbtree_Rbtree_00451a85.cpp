// std::_Rb_tree<EObjectAugmentationType, pair<const EObjectAugmentationType, float>, ...>::~_Rb_tree() -- STLport 4.6 tree destructor: clear()
//   (the out-of-line clear 0x00451240), then _Rb_tree_base frees the header node
// Leaf: ??1?$_Rb_tree@W4EObjectAugmentationType@@U?$pair@$$CBW4EObjectAugmentationType@@M@std@@U?$_Select1st@U?$pair@$$CBW4EObjectAugmentationType@@M@std@@@3@U?$less@W4EObjectAugmentationType@@@3@V?$allocator@U?$pair@$$CBW4EObjectAugmentationType@@M@std@@@3@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00451A85, 23 bytes, a COMDAT in a size-optimised TU (`pop ecx` after free).
// Identity (NF86, q11100; NF84's evidence checked): the destructor of the map<EObjectAugmentationType, float> tree. Its callee 0x00451240 is
//   that tree's clear (the landed GFSerialiseMapBinaryIn<float, EObjectAugmentationType, ..> 0x004616D6 binds it so; NF86 re-authored the self
//   row), and its landed caller CArmourDef::~CArmourDef 0x004585BD (NF84) binds this symbol for the Ego_r.pdb member CArmourDef::AugmentationResponse
//   (map<EObjectAugmentationType, float>, retail +0x3C; engine/CArmourDef.h). Byte shape = the EDamageAttribute twin 0x00451AC6 (NF83).
//   Descriptive by library (STLport; both PDBs are Dinkumware builds). Was the stand-in _global::StdList_Dtor_00451a85 (leaf
//   ?CActiveFile_OnReadFinished@@YIXPAUCActiveFile@@@Z, clear bound as ?CActiveFile_Prep@@YIXPAUCActiveFile@@@Z).
// Port (NF86): one explicit instantiation on stlport/fable_map.h (the 0x00451AC6 recipe) with FABLE_RETAIL_STLP_TREE_CLEAR_OUTOFLINE.
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_TREE_CLEAR_OUTOFLINE
#include "engine/EObjectAugmentationType.h"
#include "stlport/fable_map.h"

template std::_Rb_tree<EObjectAugmentationType, std::pair<const EObjectAugmentationType, float>, std::_Select1st<std::pair<const EObjectAugmentationType, float> >, std::less<EObjectAugmentationType>, std::allocator<std::pair<const EObjectAugmentationType, float> > >::~_Rb_tree();
