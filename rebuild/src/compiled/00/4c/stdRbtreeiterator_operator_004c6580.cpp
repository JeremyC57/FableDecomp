// std::_Rb_tree_iterator::operator--() -- STLport 4.6 tree iterator pre-decrement (_Rb_global::_M_decrement inlined), value type pair<void* const, CBulletTimeManager::CBulletTimeInfo>
// Leaf: ??F?$_Rb_tree_iterator@U?$pair@QAXVCBulletTimeInfo@CBulletTimeManager@@@std@@U?$_Nonconst_traits@U?$pair@QAXVCBulletTimeInfo@CBulletTimeManager@@@std@@@2@@std@@QAEAAU01@XZ
// Identity: the respelled caller 0x004C6A30 (map<void*, CBulletTimeManager::CBulletTimeInfo>) (agentFAM12) calls this address as this operator-- of the proven value type
//   (agentFAM12 CALLEE_FIXES.tsv; the caller row's Leaf/types come from its own landed binding + the Ego_r/FableWin name sets);
//   the body is STLport 4.6 _Rb_tree_iterator::operator-- (the head-node test, rightmost of the left subtree, climb while a left child).
//   Was the invented stand-in CRetailTreeIterator::Decrement.
// Model: stlport/fable_map.h + the explicit instantiation of the member (its own COMDAT, as retail).
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_SET_EMPTY
#define FABLE_RETAIL_STLP_MAP_EMPTY
#define FABLE_RETAIL_CBULLETTIMEMANAGER
#define FABLE_RETAIL_CBULLETTIMEMANAGER_INTERPOLATE
#include "stlport/fable_map.h"
#include "engine/CBulletTimeManager.h"
template std::_Rb_tree_iterator<std::pair<void* const, CBulletTimeManager::CBulletTimeInfo>, std::_Nonconst_traits<std::pair<void* const, CBulletTimeManager::CBulletTimeInfo> > >& std::_Rb_tree_iterator<std::pair<void* const, CBulletTimeManager::CBulletTimeInfo>, std::_Nonconst_traits<std::pair<void* const, CBulletTimeManager::CBulletTimeInfo> > >::operator--();
