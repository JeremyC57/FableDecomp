// std::_Rb_tree<CCharString, std::pair<const CCharString, std::pair<long, long>>, ...>::insert_unique(iterator position, const value_type& v) -- STLport 4.6 hinted unique insert over CCharString keys: the begin / end / interior hint cases, falling back on the unhinted insert_unique when the hint does not bracket v
// Leaf: ?insert_unique@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@U?$pair@JJ@std@@@std@@U?$_Select1st@U?$pair@$$CBVCCharString@@U?$pair@JJ@std@@@std@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@U?$pair@JJ@std@@@std@@@3@@std@@QAE?AU?$_Rb_tree_iterator@U?$pair@$$CBVCCharString@@U?$pair@JJ@std@@@std@@U?$_Nonconst_traits@U?$pair@$$CBVCCharString@@U?$pair@JJ@std@@@std@@@2@@2@U32@ABU?$pair@$$CBVCCharString@@U?$pair@JJ@std@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00756790, 668 bytes, thiscall, iterator result through the hidden pointer, `ret 0Ch`.
// Identity (proven): bound (reloc_bindings) as ?insert_unique@?$map@VCCharString@@U?$pair@JJ@std@@...@QAE?AU?$_Rb_tree_iterator@...@U32@ABU?$pair@$$CBVCCharString@@U?$pair@JJ@std@@@2@@Z (a map stand-in for the tree member); its callers are the landed map<CCharString, pair<long, long> >::operator[] 0x00756BE0 (CTCHeroRememberCrimes::RegionNamesToCrimeDurationAndFine, Ego_r.pdb) and the map's insert 0x00756AB0.
// Callees (this instantiation): insert_unique(v) 0x00756250, _M_insert 0x00756150, iterator operator++ 0x00755BB0 / operator--
//   0x00755BF0 (out of line); shared: CBasicString<char>::operator< 0x00429950 and less<CCharString>::operator() 0x0042AAB3.
// Shape (family L056): the STLport 4.6 _tree.c hinted insert_unique (the landed 0x00516AF0 shape); CCharString::operator< inline
//   over the one CBasicString<char>::operator< definition (fable_string.h), less<CCharString>::operator() called out of line where
//   VC7.1 stops expanding it, as in retail.
// NF82 (q10989): moved off the row-local CBasicString / CCharString / pair / tree models onto stlport/fable_map.h
//   (FABLE_RETAIL_STLP_TREE_INSERT_UNIQUE_HINT_BODY, explicit instantiation, the landed 0x00801EC0 / 0x00A49F60 recipe) and
//   fable_string.h (FABLE_RETAIL_CBASICSTRING_LESS_BODY): the seven _M_insert calls now bind the tree's private STLport _M_insert
//   (AAE, the spelling of the self row 0x00756150) instead of the stand-in public QAE one.
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
template _Rb_tree<CCharString, pair<CCharString const, pair<long, long> >, _Select1st<pair<CCharString const, pair<long, long> > >, less<CCharString >, allocator<pair<CCharString const, pair<long, long> > > >::iterator _Rb_tree<CCharString, pair<CCharString const, pair<long, long> >, _Select1st<pair<CCharString const, pair<long, long> > >, less<CCharString >, allocator<pair<CCharString const, pair<long, long> > > >::insert_unique(iterator, const pair<CCharString const, pair<long, long> >&);
}
