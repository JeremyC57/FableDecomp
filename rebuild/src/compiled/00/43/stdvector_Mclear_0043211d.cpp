// std::vector<std::pair<unsigned long, CDefClassBase::CSubDefInfo>, std::allocator<std::pair<unsigned long, CDefClassBase::CSubDefInfo> > >::_M_clear(void) -- STLport 4.6 _M_clear (stlport/fable_map.h)
// Leaf: ?_M_clear@?$vector@U?$pair@KVCSubDefInfo@CDefClassBase@@@std@@V?$allocator@U?$pair@KVCSubDefInfo@CDefClassBase@@@std@@@2@@std@@IAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0043211D, oracle length ? bytes.
// Identity: (1) the landed caller 0x00431F90 (std::vector<std::pair<unsigned long, CDefClassBase::CSubDefInfo>, std::allocator<std::pair<unsigned long, CDefClassBase::CSubDefInfo>>>::operator=) binds this exact spelling (rebuild/oracles/reloc_bindings.tsv); (2) Ego_r.pdb / FableWin.pdb carry the vector<U?$pair@KVCSubDefInfo@CDefClassBase@@@std@@> instantiation (?$vector@U?$pair@KVCSubDefInfo@CDefClassBase@@@std@@V?$allocator@).
// Model: the shared stlport/fable_map.h (STLport 4.6 _tree.h / _tree.c; FABLE_RETAIL_STLP_MAP); the explicit instantiation emits the member as
//   its own COMDAT. Replaces the generated stand-in (was _unattributed, a descriptive placeholder class).
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_MAP
#include "stlport/fable_map.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"
template void std::vector<std::pair<unsigned long, CDefClassBase::CSubDefInfo>, std::allocator<std::pair<unsigned long, CDefClassBase::CSubDefInfo> > >::_M_clear(void);
