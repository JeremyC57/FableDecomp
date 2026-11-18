// std::_Rb_tree<CCharString, std::pair<CCharString const, bool>, ...>::insert_unique(const value_type& v) -- STLport 4.6 unhinted unique insert into map<CCharString, bool>
// Retail: Fable.exe (TLC, VC7.1) @ 0x007AD060, 254 bytes, thiscall, pair<iterator,bool> through the hidden pointer.
// Identity: the landed caller 0x008ACA90 binds 0x007AD060 under this STLport spelling (reloc_bindings), and its callees are the same
//   map's _M_insert 0x007ACF70 and iterator operator-- 0x007AC920 (both catalogued under map<CCharString, bool>; 0x008ACA90 binds them
//   so); the _M_find of the same map is the landed 0x007ACC90. Body: the 254-byte insert_unique shape of the landed 0x009EFB80.
// NF80 (q10923): moved off the stand-in namespace FableTree_007ad060 (local CBasicString / CComparePointer / tree / pair models, the
//   string modelled as the key pointer) onto stlport/fable_map.h (explicit instantiation, the 0x009EFB80 recipe) and fable_string.h
//   (FABLE_RETAIL_CBASICSTRING_LESS_BODY).
// Flags: /Gr
// Leaf: ?insert_unique@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@_N@std@@U?$_Select1st@U?$pair@$$CBVCCharString@@_N@std@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@_N@std@@@3@@std@@QAE?AU?$pair@U?$_Rb_tree_iterator@U?$pair@$$CBVCCharString@@_N@std@@U?$_Nonconst_traits@U?$pair@$$CBVCCharString@@_N@std@@@2@@std@@_N@2@ABU?$pair@$$CBVCCharString@@_N@2@@Z
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_DEC_OUTOFLINE
#include "stlport/fable_map.h"
#include "fable_string.h"
namespace std {
template pair<_Rb_tree<CCharString, pair<CCharString const, bool >, _Select1st<pair<CCharString const, bool > >, less<CCharString >, allocator<pair<CCharString const, bool > > >::iterator, bool> _Rb_tree<CCharString, pair<CCharString const, bool >, _Select1st<pair<CCharString const, bool > >, less<CCharString >, allocator<pair<CCharString const, bool > > >::insert_unique(pair<CCharString const, bool > const&);
}
