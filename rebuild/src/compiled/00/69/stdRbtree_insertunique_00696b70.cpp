// std::_Rb_tree<CCharString, pair<CCharString const, CCountedPointer<NCreatureAction::CActionEventBase> >, _Select1st<pair<CCharString const, CCountedPointer<NCreatureAction::CActionEventBase> > >, less<CCharString >, allocator<pair<CCharString const, CCountedPointer<NCreatureAction::CActionEventBase> > > >::insert_unique -- (map<CCharString, CCountedPointer<NCreatureAction::CActionEventBase>>) STLport unique insert: descend comparing CCharString keys, then insert unless the predecessor's key equals __v's; returns (iterator, inserted)
// Leaf: ?insert_unique@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCActionEventBase@NCreatureAction@@@@@std@@U?$_Select1st@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCActionEventBase@NCreatureAction@@@@@std@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCActionEventBase@NCreatureAction@@@@@std@@@3@@std@@QAE?AU?$pair@U?$_Rb_tree_iterator@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCActionEventBase@NCreatureAction@@@@@std@@U?$_Nonconst_traits@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCActionEventBase@NCreatureAction@@@@@std@@@2@@std@@_N@2@ABU?$pair@$$CBVCCharString@@V?$CCountedPointer@VCActionEventBase@NCreatureAction@@@@@2@@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00696B70, 254 bytes, thiscall, pair<iterator, bool> through the hidden pointer, `ret 8`.
// Identity (proven): its _M_insert 0x006969D0 is landed as map<CCharString, CCountedPointer<NCreatureAction::CActionEventBase> >'s; its callers are RegisterActionEventTypes 0x006932B0 (seven inserts of {name, counted new CActionEvent*} into the global map 0x013BA8BC).
// Callees: CBasicString<char>::operator< 0x00429950 (CCharString::operator< inline), _Rb_tree::_M_insert 0x006969D0,
//   _Rb_tree_iterator::operator-- 0x00695C80.
// Shape (family M197, representative 0x00696B70): STLport 4.6 _tree.c `insert_unique(const value_type&)` verbatim over the
//   landed CCharString-key tree model (the hinted insert_unique 0x00756790): CCharString::operator< inline (equal blocks false,
//   null this true, null other false, else the CBasicString compare as a same-TU noinline body); the iterator's operator--
//   noinline. Speed TU.
// NF85: moved off the row-local CBasicString / CCharString / pair / tree models onto stlport/fable_map.h (the NF80 0x009EFB80 recipe: STLport insert_unique(const V&) with the iterator -- kept out of line, explicit
//   instantiation) and fable_string.h: the _M_insert callee is now bound under the tree's private STLport AAE spelling (its NF82 self
//   row), was the public QAE stand-in.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_DEC_OUTOFLINE
#include "fable_string.h"
#include "stlport/fable_map.h"
#include "engine/CCountedPointer.h"
namespace NCreatureAction { class CActionEventBase; }
namespace std {
template pair<_Rb_tree<CCharString, pair<CCharString const, CCountedPointer<NCreatureAction::CActionEventBase> >, _Select1st<pair<CCharString const, CCountedPointer<NCreatureAction::CActionEventBase> > >, less<CCharString >, allocator<pair<CCharString const, CCountedPointer<NCreatureAction::CActionEventBase> > > >::iterator, bool> _Rb_tree<CCharString, pair<CCharString const, CCountedPointer<NCreatureAction::CActionEventBase> >, _Select1st<pair<CCharString const, CCountedPointer<NCreatureAction::CActionEventBase> > >, less<CCharString >, allocator<pair<CCharString const, CCountedPointer<NCreatureAction::CActionEventBase> > > >::insert_unique(const pair<CCharString const, CCountedPointer<NCreatureAction::CActionEventBase> >&);
}
