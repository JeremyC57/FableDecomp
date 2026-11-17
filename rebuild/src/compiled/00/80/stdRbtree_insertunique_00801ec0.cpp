// std::_Rb_tree<EIdleStateGroup, pair<const EIdleStateGroup, long>, ...>::insert_unique(iterator position, const value_type& v) -- STLport 4.6 hinted unique insert of map<EIdleStateGroup, long>: begin / end / interior hint cases, falling back on the unhinted insert_unique when the hint does not bracket v
// Leaf: ?insert_unique@?$_Rb_tree@W4EIdleStateGroup@@U?$pair@$$CBW4EIdleStateGroup@@J@std@@U?$_Select1st@U?$pair@$$CBW4EIdleStateGroup@@J@std@@@3@U?$less@W4EIdleStateGroup@@@3@V?$allocator@U?$pair@$$CBW4EIdleStateGroup@@J@std@@@3@@std@@QAE?AU?$_Rb_tree_iterator@U?$pair@$$CBW4EIdleStateGroup@@J@std@@U?$_Nonconst_traits@U?$pair@$$CBW4EIdleStateGroup@@J@std@@@2@@2@U32@ABU?$pair@$$CBW4EIdleStateGroup@@J@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00801EC0, 518 bytes, thiscall `ret 0Ch` (hidden iterator result, position, v); /O2 TU.
// Callees: insert_unique(const value_type&) 0x004E63E0, _M_insert 0x004E6466, the instance's iterator operator++ 0x008010D0 and
//   operator-- 0x00801340 (thiscall COMDATs, the _Rb_global steps inlined in them).
// Identity (NF58, round 7): map<EIdleStateGroup, long> (Ego_r.pdb CTCIdleScheduler::IdleStatesWeight / CIdleSchedulerDef::Period);
//   its callees 0x004E63E0 / 0x004E6466 are that instance's insert_unique / _M_insert (GFSerialiseMapBinaryIn<long, EIdleStateGroup>
//   0x004E62CA binds the same insert_unique). Was the stand-in SignedKeyTree::_Rb_tree (local namespace-std model); now the explicit
//   instantiation of stlport/fable_map.h FABLE_RETAIL_STLP_TREE_INSERT_UNIQUE_HINT_BODY, with the iterator steps out of line
//   (FABLE_RETAIL_STLP_TREE_DEC_OUTOFLINE, and the NF58 overlay opt-in FABLE_RETAIL_STLP_TREE_INC_OUTOFLINE). STLport 4.6.2 source,
//   descriptive-by-library.
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_INSERT_UNIQUE_HINT_BODY
#define FABLE_RETAIL_STLP_TREE_DEC_OUTOFLINE
#define FABLE_RETAIL_STLP_TREE_INC_OUTOFLINE
#include "stlport/fable_map.h"
enum EIdleStateGroup;
namespace std {
template _Rb_tree<EIdleStateGroup, pair<EIdleStateGroup const, long>, _Select1st<pair<EIdleStateGroup const, long> >, less<EIdleStateGroup >, allocator<pair<EIdleStateGroup const, long> > >::iterator _Rb_tree<EIdleStateGroup, pair<EIdleStateGroup const, long>, _Select1st<pair<EIdleStateGroup const, long> >, less<EIdleStateGroup >, allocator<pair<EIdleStateGroup const, long> > >::insert_unique(iterator, const pair<EIdleStateGroup const, long>&);
}
