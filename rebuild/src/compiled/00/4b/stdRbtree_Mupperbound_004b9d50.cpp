// std::_Rb_tree<CCharString, CCharString, std::_Identity<CCharString>, std::less<CCharString>, std::allocator<CCharString> >::_M_upper_bound(const CCharString& k) const -- STLport 4.6 set<CCharString> upper-bound walk: the first node whose key is greater than k, or the header
// Leaf: ?_M_upper_bound@?$_Rb_tree@VCCharString@@V1@U?$_Identity@VCCharString@@@std@@U?$less@VCCharString@@@3@V?$allocator@VCCharString@@@3@@std@@ABEPAU_Rb_tree_node_base@2@ABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004B9D50, 67 bytes, thiscall, `ret 4`; returns the node pointer.
// Identity: set<CCharString> (Ego_r.pdb CQuestManager::QuestsWithResourcesLoadedThatAreNotActivated and CSymbolMap::NamesUsed are
//   set<CCharString>); its PDB-named callers CQuestManager::ActivateQuests 0x004B3CE0 (landed, binds this exact private spelling) and
//   CSymbolMap::RemoveSymbolsOfGroupType 0x009E5360 use it on those members, with the same tree's _M_lower_bound 0x004B9D00 (NF80).
//   The catalog's map<CCharString, long> was a stand-in (one address, one tree).
// Callees: CBasicString<char>::operator< 0x00429950 (through the inline CCharString::operator<).
// Shape: STLport 4.6 _tree.c `_M_upper_bound` (fable_map.h FABLE_RETAIL_STLP_TREE_BOUNDS_BODY_PASS22), k's data pointer loaded once
//   because the operator< body is visible (fable_string.h FABLE_RETAIL_CBASICSTRING_LESS_BODY).
// NF82 (q10994): moved off the row-local CBasicString / CCharString / tree models onto stlport/fable_map.h (explicit instantiation,
//   the 0x004B9D00 recipe) and fable_string.h; the own symbol is now the private set<CCharString> spelling (was the public map one).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE
#define FABLE_RETAIL_STLP_TREE_BOUNDS_BODY_PASS22
#include "stlport/fable_map.h"
#include "fable_string.h"
template std::_Rb_tree_node_base* std::_Rb_tree<CCharString, CCharString, std::_Identity<CCharString>, std::less<CCharString>, std::allocator<CCharString> >::_M_upper_bound(const CCharString&) const;
