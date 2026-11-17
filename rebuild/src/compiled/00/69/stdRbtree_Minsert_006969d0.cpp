// std::_Rb_tree<CCharString, std::pair<CCharString const, CCountedPointer<NCreatureAction::CActionEventBase>>, ...>::_M_insert(_Rb_tree_node_base* x, _Rb_tree_node_base* y, const value_type& v, _Rb_tree_node_base* w) -- STLport 4.6 node insert: create a node for v, link it left or right of y, fix root / leftmost / rightmost, rebalance, count, return the iterator
// Leaf: ?_M_insert@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCActionEventBase@NCreatureAction@@@@@std@@U?$_Select1st@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCActionEventBase@NCreatureAction@@@@@std@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCActionEventBase@NCreatureAction@@@@@std@@@3@@std@@AAE?AU?$_Rb_tree_iterator@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCActionEventBase@NCreatureAction@@@@@std@@U?$_Nonconst_traits@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCActionEventBase@NCreatureAction@@@@@std@@@2@@2@PAU_Rb_tree_node_base@2@0ABU?$pair@$$CBVCCharString@@V?$CCountedPointer@VCActionEventBase@NCreatureAction@@@@@2@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006969D0 (thiscall `ret 14h`: the hidden iterator result, x, y, &v, w).
// Identity: its only caller, the landed insert_unique 0x00696B70, binds this address as the private _M_insert of map<CCharString, CCountedPointer<NCreatureAction::CActionEventBase> > (the value type named in this row's own model and in that binding).
// Callees: CBasicString<char>::operator< 0x00429950 (through the inline CCharString::operator<), malloc 0x00BFEA0E (inline allocate),
//   CCharString copy constructor 0x0099EC30 and the value's copy, _Rb_global<bool>::_Rebalance 0x0042971D.
// NF82 (q11007): moved off the row-local CBasicStringChar / CCharString / value / pair / tree models onto stlport/fable_map.h
//   (FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY, explicit instantiation, the 0x004ABC70 / 0x005E1FC0 recipe) and fable_string.h
//   (FABLE_RETAIL_CBASICSTRING_LESS_BODY): the string compare binds Ego_r.pdb ??M?$CBasicString@D@@QBE_NABV0@@Z (was the stand-in
//   ??MCBasicStringChar@@QBE_NABV0@@Z) and the own symbol is the private STLport _M_insert its caller binds.
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#include "fable_string.h"
#include "stlport/fable_map.h"
#include "engine/CCountedPointer.h"
namespace NCreatureAction { class CActionEventBase; }
namespace std {
template _Rb_tree<CCharString, pair<CCharString const, CCountedPointer<NCreatureAction::CActionEventBase> >, _Select1st<pair<CCharString const, CCountedPointer<NCreatureAction::CActionEventBase> > >, less<CCharString>, allocator<pair<CCharString const, CCountedPointer<NCreatureAction::CActionEventBase> > > >::iterator _Rb_tree<CCharString, pair<CCharString const, CCountedPointer<NCreatureAction::CActionEventBase> >, _Select1st<pair<CCharString const, CCountedPointer<NCreatureAction::CActionEventBase> > >, less<CCharString>, allocator<pair<CCharString const, CCountedPointer<NCreatureAction::CActionEventBase> > > >::_M_insert(_Rb_tree_node_base*, _Rb_tree_node_base*, const pair<CCharString const, CCountedPointer<NCreatureAction::CActionEventBase> >&, _Rb_tree_node_base*);
}
