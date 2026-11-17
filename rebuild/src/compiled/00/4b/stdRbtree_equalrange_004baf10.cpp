// std::_Rb_tree<CCharString, CCharString, std::_Identity<CCharString>, std::less<CCharString>, std::allocator<CCharString> >::equal_range(const CCharString& k) -- STLport 4.6 set<CCharString>: the [lower_bound, upper_bound) iterator pair for a string key
// Leaf: ?equal_range@?$_Rb_tree@VCCharString@@V1@U?$_Identity@VCCharString@@@std@@U?$less@VCCharString@@@3@V?$allocator@VCCharString@@@3@@std@@QAE?AU?$pair@U?$_Rb_tree_iterator@VCCharString@@U?$_Nonconst_traits@VCCharString@@@std@@@std@@U12@@2@ABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004BAF10, length 42 bytes, `ret 8` (hidden pair result + key).
// Callees: _M_upper_bound @ 0x004B9D50, _M_lower_bound @ 0x004B9D00 (set<CCharString>; NF80 / NF82: Ego_r.pdb
//   CQuestManager::QuestsWithResourcesLoadedThatAreNotActivated and CSymbolMap::NamesUsed are set<CCharString>, and their PDB-named
//   landed users 0x004B3CE0 / 0x009E5360 call these two addresses on them).
// Label: the catalog "StdMap_EqualRange" is behavioural; STLport 4.x `equal_range(k) { return pair<iterator,iterator>(
//   lower_bound(k), upper_bound(k)); }` -- VC evaluates the pair arguments right to left (upper bound first) and stores
//   {first, second}.
// Shape: default /O2 TU (the key is loaded once into edi and pushed for both calls); STLport's _Rb_tree_iterator has a
//   user copy constructor, which keeps `pop edi` between the two stores.
// NF82 (q10993 / q10994): moved off the row-local two-argument _Rb_tree<CCharString, pair<const CCharString, long> > / CCharString
//   stand-ins onto stlport/fable_map.h (FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE: lower_bound / upper_bound / equal_range are inline
//   wrappers over the out-of-line private _M_lower_bound / _M_upper_bound) and fable_string.h; the two calls now bind the private
//   set<CCharString> spellings of 0x004B9D00 / 0x004B9D50.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE
#include "stlport/fable_map.h"
#include "fable_string.h"
template std::pair<std::_Rb_tree<CCharString, CCharString, std::_Identity<CCharString>, std::less<CCharString>, std::allocator<CCharString> >::iterator, std::_Rb_tree<CCharString, CCharString, std::_Identity<CCharString>, std::less<CCharString>, std::allocator<CCharString> >::iterator> std::_Rb_tree<CCharString, CCharString, std::_Identity<CCharString>, std::less<CCharString>, std::allocator<CCharString> >::equal_range(const CCharString&);
