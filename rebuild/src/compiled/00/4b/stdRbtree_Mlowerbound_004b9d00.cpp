// std::_Rb_tree<CCharString, CCharString, std::_Identity<CCharString>, std::less<CCharString>, std::allocator<CCharString> >::_M_lower_bound(const CCharString& k) const -- STLport 4.6 set<CCharString> lower-bound walk: the last node whose key is not less than k (the header when none)
// Leaf: ?_M_lower_bound@?$_Rb_tree@VCCharString@@V1@U?$_Identity@VCCharString@@@std@@U?$less@VCCharString@@@3@V?$allocator@VCCharString@@@3@@std@@ABEPAU_Rb_tree_node_base@2@ABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004B9D00, 65 bytes, thiscall, `ret 4`; returns the node pointer.
// Identity (two PDB-named landed callers, both on an Ego_r.pdb set<CCharString>): CQuestManager::ActivateQuests 0x004B3CE0
//   (QuestsWithResourcesLoadedThatAreNotActivated, stlport/fable_map.h FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE) binds 0x004B9D00 as
//   this private const _M_lower_bound, and CSymbolMap::RemoveSymbolsOfGroupType 0x009E5360 calls it on NamesUsed. The catalog's
//   "std::_Tree<std::_Tmap_traits<CCharString,float,...>>::_Lbound" is a Dinkumware (FableWin) spelling; retail is STLport.
// Callee: CBasicString<char>::operator< 0x00429950 through the inline CCharString::operator< (equal data pointers -> false, null node
//   key -> true, null k -> false).
// NF80 (q10923): moved off the extern "C" Tree_Lbound stand-in and its local CCharString / node / tree models onto
//   stlport/fable_map.h (FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE + _BOUNDS_BODY_PASS22: the STLport _tree.c walk) and fable_string.h
//   (FABLE_RETAIL_CBASICSTRING_LESS_BODY).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE
#define FABLE_RETAIL_STLP_TREE_BOUNDS_BODY_PASS22
#include "stlport/fable_map.h"
#include "fable_string.h"
template std::_Rb_tree_node_base* std::_Rb_tree<CCharString, CCharString, std::_Identity<CCharString>, std::less<CCharString>, std::allocator<CCharString> >::_M_lower_bound(const CCharString&) const;
