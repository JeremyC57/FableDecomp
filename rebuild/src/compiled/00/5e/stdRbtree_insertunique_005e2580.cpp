// std::_Rb_tree<CCharString, std::pair<CCharString const, CIVCountedPointer<CAnimComponentBase> >, ...>::insert_unique(const value_type& v) -- STLport 4.6 unhinted unique insert into map<CCharString, CIVCountedPointer<CAnimComponentBase> > (the animation components by token name)
// Retail: Fable.exe (TLC, VC7.1) @ 0x005E2580, 254 bytes, thiscall, pair<iterator,bool> through the hidden pointer.
// Identity: the landed caller CAnimationEntry::StatRegisterClass 0x005DD670 (Ego_r.pdb / FableWin.pdb) binds 0x005E2580 under this
//   STLport spelling (reloc_bindings) when it registers each component by its token; Ego_r.pdb's TPI has
//   map<CCharString,CIVCountedPointer<CAnimComponentBase> >. Callees: CBasicString<char>::operator< 0x00429950, the map's _M_insert
//   0x005E1FC0 and iterator operator-- 0x005DFCE0. Body: the 254-byte insert_unique shape of the landed 0x009EFB80.
// NF80 (q10923): moved off the stand-in namespace FableTree_005e2580 (local CBasicString / CComparePointer / tree / pair models, the
//   string modelled as the key pointer) onto stlport/fable_map.h (explicit instantiation, the 0x009EFB80 recipe) and fable_string.h
//   (FABLE_RETAIL_CBASICSTRING_LESS_BODY).
// Flags: /Gr
// Leaf: ?insert_unique@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@V?$CIVCountedPointer@VCAnimComponentBase@@@@@std@@U?$_Select1st@U?$pair@$$CBVCCharString@@V?$CIVCountedPointer@VCAnimComponentBase@@@@@std@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@V?$CIVCountedPointer@VCAnimComponentBase@@@@@std@@@3@@std@@QAE?AU?$pair@U?$_Rb_tree_iterator@U?$pair@$$CBVCCharString@@V?$CIVCountedPointer@VCAnimComponentBase@@@@@std@@U?$_Nonconst_traits@U?$pair@$$CBVCCharString@@V?$CIVCountedPointer@VCAnimComponentBase@@@@@std@@@2@@std@@_N@2@ABU?$pair@$$CBVCCharString@@V?$CIVCountedPointer@VCAnimComponentBase@@@@@2@@Z
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_DEC_OUTOFLINE
#include "stlport/fable_map.h"
#include "fable_string.h"
#include "engine/CIVCountedPointer.h"
class CAnimComponentBase;
namespace std {
template pair<_Rb_tree<CCharString, pair<CCharString const, CIVCountedPointer<CAnimComponentBase> >, _Select1st<pair<CCharString const, CIVCountedPointer<CAnimComponentBase> > >, less<CCharString >, allocator<pair<CCharString const, CIVCountedPointer<CAnimComponentBase> > > >::iterator, bool> _Rb_tree<CCharString, pair<CCharString const, CIVCountedPointer<CAnimComponentBase> >, _Select1st<pair<CCharString const, CIVCountedPointer<CAnimComponentBase> > >, less<CCharString >, allocator<pair<CCharString const, CIVCountedPointer<CAnimComponentBase> > > >::insert_unique(pair<CCharString const, CIVCountedPointer<CAnimComponentBase> > const&);
}
