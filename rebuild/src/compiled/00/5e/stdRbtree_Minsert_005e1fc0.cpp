// std::_Rb_tree<CCharString, std::pair<CCharString const, CIVCountedPointer<CAnimComponentBase> >, ...>::_M_insert(_Rb_tree_node_base* x, _Rb_tree_node_base* y, const value_type& v, _Rb_tree_node_base* w) -- map<CCharString, CIVCountedPointer<CAnimComponentBase> > node insert: create a node for v, link it left or right of y, fix root/leftmost/rightmost, rebalance, count, return the iterator
// Leaf: ?_M_insert@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@V?$CIVCountedPointer@VCAnimComponentBase@@@@@std@@U?$_Select1st@U?$pair@$$CBVCCharString@@V?$CIVCountedPointer@VCAnimComponentBase@@@@@std@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@V?$CIVCountedPointer@VCAnimComponentBase@@@@@std@@@3@@std@@AAE?AU?$_Rb_tree_iterator@U?$pair@$$CBVCCharString@@V?$CIVCountedPointer@VCAnimComponentBase@@@@@std@@U?$_Nonconst_traits@U?$pair@$$CBVCCharString@@V?$CIVCountedPointer@VCAnimComponentBase@@@@@std@@@2@@2@PAU_Rb_tree_node_base@2@0ABU?$pair@$$CBVCCharString@@V?$CIVCountedPointer@VCAnimComponentBase@@@@@2@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005E1FC0, 256 bytes (0xFC + int3 padding), thiscall `ret 14h` (the hidden
//   iterator result pointer first, then x, y, &v, w).
// Identity: the tree at global 0x013B8C1C that CAnimationEntry::StatRegisterClass (0x005DD670) fills through
//   insert_unique 0x005E2580 (its only two call sites, 0x005E25F6 / 0x005E264F): every registration builds a
//   12-byte counted component object (vtable +0, count +4 = 1) and inserts (CCharString name, counted pointer) --
//   Ego_r.pdb map<CCharString, CIVCountedPointer<CAnimComponentBase> > (node 0x18: base 0x10 + the 8-byte pair).
//   The catalog label "std_set_InsertStringKey" is descriptive.
// Callees: CBasicString<char>::operator< 0x00429950 (CCharString::operator< inlined), malloc 0x00BFEA0E (inline
//   allocate), CCharString copy constructor 0x0099EC30, _Rb_global<bool>::_Rebalance 0x0042971D (__cdecl).
// Shape: STLport 4.6 `_M_insert` verbatim (the landed 0x009AB120 model); here the pair copy constructor is the
//   implicit one, so _M_create_node inlines the null-checked placement copy: the out-of-line key copy, then the
//   intrusive pointer copy (`if (P) ++P->RefCount`, count at +4) re-reading &v from its argument slot.
// NF82 (q10991 / q11007): moved off the row-local CBasicStringChar / CCharStringData / CCharString / CIVCountedPointer / pair / tree
//   models onto stlport/fable_map.h (FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY, explicit instantiation, the landed 0x004ABC70
//   recipe), engine/CIVCountedPointer.h (retail CIVCountedPointeeBase + the counted copy constructor opt-ins, as 0x005DD670 on the
//   same tree) and fable_string.h (FABLE_RETAIL_CBASICSTRING_LESS_BODY):
//   the row's own symbol is the private STLport _M_insert NF80's 0x005E2580 binds, and the string compare binds Ego_r.pdb
//   ??M?$CBasicString@D@@QBE_NABV0@@Z (was the stand-in ??MCBasicStringChar@@QBE_NABV0@@Z).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#include "fable_string.h"
#include "stlport/fable_map.h"
#include "engine/CIVCountedPointer.h"
class CAnimComponentBase;
namespace std {
template _Rb_tree<CCharString, pair<CCharString const, CIVCountedPointer<CAnimComponentBase> >, _Select1st<pair<CCharString const, CIVCountedPointer<CAnimComponentBase> > >, less<CCharString >, allocator<pair<CCharString const, CIVCountedPointer<CAnimComponentBase> > > >::iterator _Rb_tree<CCharString, pair<CCharString const, CIVCountedPointer<CAnimComponentBase> >, _Select1st<pair<CCharString const, CIVCountedPointer<CAnimComponentBase> > >, less<CCharString >, allocator<pair<CCharString const, CIVCountedPointer<CAnimComponentBase> > > >::_M_insert(_Rb_tree_node_base*, _Rb_tree_node_base*, const pair<CCharString const, CIVCountedPointer<CAnimComponentBase> >&, _Rb_tree_node_base*);
}
