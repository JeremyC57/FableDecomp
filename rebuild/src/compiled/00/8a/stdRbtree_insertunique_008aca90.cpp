// std::_Rb_tree::insert_unique(iterator position, const value_type& v) -- (map<CCharString, bool>) STLport 4.6 hinted unique insert over CCharString keys: the begin / end / interior hint cases, falling back on the unhinted insert_unique when the hint does not bracket v
// Leaf: ?insert_unique@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@_N@std@@U?$_Select1st@U?$pair@$$CBVCCharString@@_N@std@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@_N@std@@@3@@std@@QAE?AU?$_Rb_tree_iterator@U?$pair@$$CBVCCharString@@_N@std@@U?$_Nonconst_traits@U?$pair@$$CBVCCharString@@_N@std@@@2@@2@U32@ABU?$pair@$$CBVCCharString@@_N@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008ACA90, 668 bytes (+ int3 padding to 672), thiscall `ret 0Ch` (hidden iterator result,
//   position, v).
// Identity (proven): map<CCharString, bool>; evidence: its callers are the landed map<CCharString, bool>::operator[] 0x008ADF10 (CGameScriptInterface::SetCategoryActivity / SetIsGossipForPlayer / SetActiveGossipCategories, the gossip-category flags) and 0x008AD5C0; its operator-- 0x007AC920 is the landed map<CCharString, bool> iterator step and its _M_insert 0x007ACF70 mallocs 0x18-byte nodes (key at +0x10, the bool at +0x14); insert_unique(v) is 0x007AD060, operator++ 0x007AB8A0.
// Label: the manifest's "std::map::operator[]" is descriptive (family H006: seven rows that differ only in their call displacements;
//   retail has no identical-code folding, so each address is one instantiation); this is STLport 4.6
//   `_Rb_tree::insert_unique(iterator, const value_type&)` with a CCharString key at node +0x10.
// Callees (this instantiation): insert_unique(v) @ 0x007AD060 (pair result through a hidden pointer), _M_insert @ 0x007ACF70,
//   iterator operator++ @ 0x007AB8A0 / operator-- @ 0x007AC920 (out of line, thiscall on the iterator); shared:
//   CBasicString<char>::operator< 0x00429950 and less<CCharString>::operator() 0x0042AAB3 (VC7.1 inlines the key compare at
// Shape: the STLport 4.6 _tree.c hinted insert_unique; CCharString::operator< inline over the one CBasicString<char>::operator<
//   definition (fable_string.h), less<CCharString>::operator() called out of line where VC7.1 stops expanding it, as in retail.
// NF82 (q10990): moved off the row-local CBasicString / CCharString / pair / tree models onto stlport/fable_map.h
//   (FABLE_RETAIL_STLP_TREE_INSERT_UNIQUE_HINT_BODY, explicit instantiation, the landed 0x00801EC0 / 0x00A49F60 recipe) and
//   fable_string.h (FABLE_RETAIL_CBASICSTRING_LESS_BODY): the _M_insert calls now bind the tree's private STLport _M_insert (AAE,
//   the self row 0x007ACF70's spelling) instead of the stand-in public QAE one.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_INSERT_UNIQUE_HINT_BODY
#define FABLE_RETAIL_STLP_TREE_DEC_OUTOFLINE
#define FABLE_RETAIL_STLP_TREE_INC_OUTOFLINE
#include "fable_string.h"
#include "stlport/fable_map.h"
namespace std {
template _Rb_tree<CCharString, pair<CCharString const, bool >, _Select1st<pair<CCharString const, bool > >, less<CCharString >, allocator<pair<CCharString const, bool > > >::iterator _Rb_tree<CCharString, pair<CCharString const, bool >, _Select1st<pair<CCharString const, bool > >, less<CCharString >, allocator<pair<CCharString const, bool > > >::insert_unique(iterator, const pair<CCharString const, bool >&);
}
