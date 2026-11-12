// std::_Rb_tree<void*, std::pair<void* const, CBulletTimeManager::CBulletTimeInfo>, std::_Select1st<std::pair<void* const, CBulletTimeManager::CBulletTimeInfo> >, std::less<void*>, std::allocator<std::pair<void* const, CBulletTimeManager::CBulletTimeInfo> > >::_M_erase(std::_Rb_tree_node<std::pair<void* const, CBulletTimeManager::CBulletTimeInfo> >*) -- STLport 4.6 _M_erase (stlport/fable_map.h)
// Leaf: ?_M_erase@?$_Rb_tree@PAXU?$pair@QAXVCBulletTimeInfo@CBulletTimeManager@@@std@@U?$_Select1st@U?$pair@QAXVCBulletTimeInfo@CBulletTimeManager@@@std@@@2@U?$less@PAX@2@V?$allocator@U?$pair@QAXVCBulletTimeInfo@CBulletTimeManager@@@std@@@2@@std@@AAEXPAU?$_Rb_tree_node@U?$pair@QAXVCBulletTimeInfo@CBulletTimeManager@@@std@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004AA650, oracle length 51 bytes.
// Identity: (1) the landed caller 0x004ADDF0 (std::map<void *, CBulletTimeManager::CBulletTimeInfo, std::less<void *>, std::allocator<std::pair<void *const, CBulletTimeManager::CBulletTimeInfo>>>::`scalar deleting destructor') binds this exact spelling (rebuild/oracles/reloc_bindings.tsv); (2) Ego_r.pdb / FableWin.pdb carry the map<PAXVCBulletTimeInfo@CBulletTimeManager@@> instantiation (_Tmap_traits@PAXVCBulletTimeInfo@CBulletTimeManager@@U?$less@).
// Model: the shared stlport/fable_map.h (STLport 4.6 _tree.h / _tree.c; FABLE_RETAIL_STLP_MAP); the explicit instantiation emits the member as
//   its own COMDAT. Replaces the generated stand-in (was _unattributed, a descriptive placeholder class).
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_SET_EMPTY
#define FABLE_RETAIL_STLP_MAP_EMPTY
#define FABLE_RETAIL_CBULLETTIMEMANAGER
#define FABLE_RETAIL_CBULLETTIMEMANAGER_INTERPOLATE
#include "stlport/fable_map.h"
#include "engine/CBulletTimeManager.h"
template void std::_Rb_tree<void*, std::pair<void* const, CBulletTimeManager::CBulletTimeInfo>, std::_Select1st<std::pair<void* const, CBulletTimeManager::CBulletTimeInfo> >, std::less<void*>, std::allocator<std::pair<void* const, CBulletTimeManager::CBulletTimeInfo> > >::_M_erase(std::_Rb_tree_node<std::pair<void* const, CBulletTimeManager::CBulletTimeInfo> >*);
