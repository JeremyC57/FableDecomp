// std::_Rb_tree<CCharString, std::pair<CCharString const, std::pair<long, long> >, ...>::insert_unique(const value_type& v) -- STLport 4.6 unhinted unique insert into map<CCharString, pair<long, long> > (CTCHeroRememberCrimes::RegionNamesToCrimeDurationAndFine)
// Flags: /O2 /Oy /W3
// Leaf: ?insert_unique@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@U?$pair@JJ@std@@@std@@U?$_Select1st@U?$pair@$$CBVCCharString@@U?$pair@JJ@std@@@std@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@U?$pair@JJ@std@@@std@@@3@@std@@QAE?AU?$pair@U?$_Rb_tree_iterator@U?$pair@$$CBVCCharString@@U?$pair@JJ@std@@@std@@U?$_Nonconst_traits@U?$pair@$$CBVCCharString@@U?$pair@JJ@std@@@std@@@2@@std@@_N@2@ABU?$pair@$$CBVCCharString@@U?$pair@JJ@std@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00756250, 254 bytes, thiscall, pair<iterator,bool> through the hidden pointer.
// Identity: the landed hinted insert_unique 0x00756790 (same map) falls back on this unhinted insert_unique and already binds it under
//   this STLport spelling (reloc_bindings); the body is the 254-byte shape of the landed 0x009EFB80 (map<CCharString, CCountedPointer<
//   CConsoleInputBase> >::insert_unique, Ego_r.pdb CConsole::RegisteredConsoleInputs). Callees: CBasicString<char>::operator< 0x00429950,
//   _M_insert 0x00756150, iterator operator-- 0x00755BF0 (out of line).
// NF80 (q10923): moved off the stand-in namespace X_00756250 (local CBasicString / CCharString / pair / tree models) onto
//   stlport/fable_map.h (explicit instantiation, the 0x009EFB80 recipe) and fable_string.h (FABLE_RETAIL_CBASICSTRING_LESS_BODY).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_DEC_OUTOFLINE
#include "stlport/fable_map.h"
#include "fable_string.h"
namespace std {
template pair<_Rb_tree<CCharString, pair<CCharString const, pair<long, long> >, _Select1st<pair<CCharString const, pair<long, long> > >, less<CCharString >, allocator<pair<CCharString const, pair<long, long> > > >::iterator, bool> _Rb_tree<CCharString, pair<CCharString const, pair<long, long> >, _Select1st<pair<CCharString const, pair<long, long> > >, less<CCharString >, allocator<pair<CCharString const, pair<long, long> > > >::insert_unique(pair<CCharString const, pair<long, long> > const&);
}
