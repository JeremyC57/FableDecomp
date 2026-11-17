// std::_Rb_tree<EDamageAttribute, pair<const EDamageAttribute, float>, ...>::~_Rb_tree() -- STLport 4.6 tree destructor: clear() (the
// out-of-line _M_erase 0x00450903 from the root, header relinked, count zeroed), then _Rb_tree_base frees the header node
// Leaf: ??1?$_Rb_tree@W4EDamageAttribute@@U?$pair@$$CBW4EDamageAttribute@@M@std@@U?$_Select1st@U?$pair@$$CBW4EDamageAttribute@@M@std@@@3@U?$less@W4EDamageAttribute@@@3@V?$allocator@U?$pair@$$CBW4EDamageAttribute@@M@std@@@3@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006DC020, 66 bytes (speed TU).
// Identity (NF81, round 7, q10986 binder): the destructor of the map<EDamageAttribute, float> tree (the erase callee 0x00450903 is that
//   tree's _M_erase, q10986); it sits in the CHeroExperienceDef serialiser TU (0x006DC350 / 0x006DCFA0, whose CComboMultiplierData
//   values hold that map) and nothing in Fable.exe calls or references it (an unreferenced COMDAT: no E8 / E9 / absolute reference).
//   Whether VC7.1 emitted it as map's implicit destructor (with ~_Rb_tree inlined) or as ~_Rb_tree itself is not provable from the
//   bytes; STLport's ~_Rb_tree is the function that holds this text (the landed 66-byte tree destructors 0x0056D3D0 / 0x006F1BA0 are
//   named so). Descriptive-by-library (STLport; both PDBs are Dinkumware). Was the per-erase stand-in
//   CRetailTree_erase00450903::~CRetailTree_erase00450903 (a local {head, size} model).
// Port (NF81): one explicit instantiation on stlport/fable_map.h (the 0x006F1BA0 recipe) with TREE_MERASE_NOINLINE (clear calls the
//   _M_erase COMDAT; its body is not expanded, as in 0x006DCFA0 / 0x006DC350).
#define FABLE_RETAIL_STLP_TREE_MERASE_NOINLINE
#include "engine/EDamageAttribute.h"
#include "stlport/fable_map.h"

template std::_Rb_tree<EDamageAttribute, std::pair<const EDamageAttribute, float>, std::_Select1st<std::pair<const EDamageAttribute, float> >, std::less<EDamageAttribute>, std::allocator<std::pair<const EDamageAttribute, float> > >::~_Rb_tree();
