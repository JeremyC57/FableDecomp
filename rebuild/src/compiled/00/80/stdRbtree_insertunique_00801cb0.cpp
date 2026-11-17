// std::_Rb_tree<EIdleStateGroup, pair<const EIdleStateGroup, CAIStateGroupBase*>, ...>::insert_unique(iterator position, const value_type& v) -- STLport 4.6 hinted unique insert of map<EIdleStateGroup, CAIStateGroupBase*>: begin / end / interior hint cases, falling back on the unhinted insert_unique when the hint does not bracket v
// Leaf: ?insert_unique@?$_Rb_tree@W4EIdleStateGroup@@U?$pair@$$CBW4EIdleStateGroup@@PAVCAIStateGroupBase@@@std@@U?$_Select1st@U?$pair@$$CBW4EIdleStateGroup@@PAVCAIStateGroupBase@@@std@@@3@U?$less@W4EIdleStateGroup@@@3@V?$allocator@U?$pair@$$CBW4EIdleStateGroup@@PAVCAIStateGroupBase@@@std@@@3@@std@@QAE?AU?$_Rb_tree_iterator@U?$pair@$$CBW4EIdleStateGroup@@PAVCAIStateGroupBase@@@std@@U?$_Nonconst_traits@U?$pair@$$CBW4EIdleStateGroup@@PAVCAIStateGroupBase@@@std@@@2@@2@U32@ABU?$pair@$$CBW4EIdleStateGroup@@PAVCAIStateGroupBase@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00801CB0, 518 bytes, thiscall `ret 0Ch` (hidden iterator result, position, v); /O2 TU.
// Callees: insert_unique(const value_type&) 0x008017F0, _M_insert 0x00801730, the instance's iterator operator++ 0x00801200 and
//   operator-- 0x008012F0 (thiscall COMDATs, catalogued as std::_Rb_tree_iterator<pair<EIdleStateGroup const, CAIStateGroupBase*>,
//   _Nonconst_traits<..> >::operator++ / operator--).
// Identity (NF58, round 7): map<EIdleStateGroup, CAIStateGroupBase*> (Ego_r.pdb CTCIdleScheduler::IdleStates); its callees
//   0x008017F0 / 0x00801730 are that instance's insert_unique / _M_insert (0x008017F0 is landed under that STLport name). Was the
//   stand-in RecutTree_00801cb0::_Rb_tree (local namespace-std model); now the explicit instantiation of stlport/fable_map.h
//   FABLE_RETAIL_STLP_TREE_INSERT_UNIQUE_HINT_BODY, with the iterator steps out of line (FABLE_RETAIL_STLP_TREE_DEC_OUTOFLINE, and the
//   NF58 overlay opt-in FABLE_RETAIL_STLP_TREE_INC_OUTOFLINE). STLport 4.6.2 source, descriptive-by-library.
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_INSERT_UNIQUE_HINT_BODY
#define FABLE_RETAIL_STLP_TREE_DEC_OUTOFLINE
#define FABLE_RETAIL_STLP_TREE_INC_OUTOFLINE
#include "stlport/fable_map.h"
enum EIdleStateGroup;
class CAIStateGroupBase;
namespace std {
template _Rb_tree<EIdleStateGroup, pair<EIdleStateGroup const, CAIStateGroupBase*>, _Select1st<pair<EIdleStateGroup const, CAIStateGroupBase*> >, less<EIdleStateGroup >, allocator<pair<EIdleStateGroup const, CAIStateGroupBase*> > >::iterator _Rb_tree<EIdleStateGroup, pair<EIdleStateGroup const, CAIStateGroupBase*>, _Select1st<pair<EIdleStateGroup const, CAIStateGroupBase*> >, less<EIdleStateGroup >, allocator<pair<EIdleStateGroup const, CAIStateGroupBase*> > >::insert_unique(iterator, const pair<EIdleStateGroup const, CAIStateGroupBase*>&);
}
