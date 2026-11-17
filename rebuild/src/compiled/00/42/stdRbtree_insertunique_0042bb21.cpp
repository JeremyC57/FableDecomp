// std::_Rb_tree<NUISystem::CManager::EUsedKeys, pair<NUISystem::CManager::EUsedKeys const, long>, _Select1st<pair<NUISystem::CManager::EUsedKeys const, long> >, less<NUISystem::CManager::EUsedKeys >, allocator<pair<NUISystem::CManager::EUsedKeys const, long> > >::insert_unique(const value_type& v) -- STLport 4.6 _tree.c unique insert: descend on key < node key, step back with _Rb_global<bool>::_M_decrement (0x004294E2) unless at begin(), then _M_insert (0x0042BA99) when the predecessor's key is less; pair<iterator, bool> through the hidden pointer
// Leaf: ?insert_unique@?$_Rb_tree@W4EUsedKeys@CManager@NUISystem@@U?$pair@$$CBW4EUsedKeys@CManager@NUISystem@@J@std@@U?$_Select1st@U?$pair@$$CBW4EUsedKeys@CManager@NUISystem@@J@std@@@5@U?$less@W4EUsedKeys@CManager@NUISystem@@@5@V?$allocator@U?$pair@$$CBW4EUsedKeys@CManager@NUISystem@@J@std@@@5@@std@@QAE?AU?$pair@U?$_Rb_tree_iterator@U?$pair@$$CBW4EUsedKeys@CManager@NUISystem@@J@std@@U?$_Nonconst_traits@U?$pair@$$CBW4EUsedKeys@CManager@NUISystem@@J@std@@@2@@std@@_N@2@ABU?$pair@$$CBW4EUsedKeys@CManager@NUISystem@@J@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0042BB21, 134 bytes, thiscall `ret 8`; frame-pointer size TU.
// Identity (NF58, round 7): the map<NUISystem::CManager::EUsedKeys, long> tree; Ego_r.pdb members of this type: CManager::Keys. Landed
//   callers of its insert_unique 0x0042BB21: 0x0042C1C1 _global::CMemoryAllocatorVariableSize_FindInsertPointLeft. Landed callers of
//   _M_insert besides 0x0042BB21: 0x0042C1C1 _global::CMemoryAllocatorVariableSize_FindInsertPointLeft.
//   Its callee _M_insert 0x0042BA99 is the same instance. STLport 4.6.2 source, descriptive-by-library. Was the stand-in
//   `CRetailTree_longkey_ins0042ba99::insert`; now an explicit instantiation over stlport/fable_map.h (FABLE_RETAIL_STLP_MAP body;
//   FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ: out-of-line _M_decrement), the landed 0x0042CEE3 pattern.
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ
#include "stlport/fable_map.h"
#include "engine/NUISystem_CManager.h"
namespace std {
template pair<_Rb_tree<NUISystem::CManager::EUsedKeys, pair<NUISystem::CManager::EUsedKeys const, long>, _Select1st<pair<NUISystem::CManager::EUsedKeys const, long> >, less<NUISystem::CManager::EUsedKeys >, allocator<pair<NUISystem::CManager::EUsedKeys const, long> > >::iterator, bool> _Rb_tree<NUISystem::CManager::EUsedKeys, pair<NUISystem::CManager::EUsedKeys const, long>, _Select1st<pair<NUISystem::CManager::EUsedKeys const, long> >, less<NUISystem::CManager::EUsedKeys >, allocator<pair<NUISystem::CManager::EUsedKeys const, long> > >::insert_unique(const pair<NUISystem::CManager::EUsedKeys const, long>&);
}
