// std::vector<std::pair<unsigned long, CDefString>, std::allocator<std::pair<unsigned long, CDefString> > >::_M_clear(void) -- STLport 4.6 _M_clear (stlport/fable_map.h)
// Leaf: ?_M_clear@?$vector@U?$pair@KVCDefString@@@std@@V?$allocator@U?$pair@KVCDefString@@@std@@@2@@std@@IAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0045D2AA, oracle length ? bytes.
// Identity: (1) the landed caller 0x0045D183 (std::vector<std::pair<unsigned long, CDefString>, std::allocator<std::pair<unsigned long, CDefString>>>::operator=) binds this exact spelling (rebuild/oracles/reloc_bindings.tsv); (2) Ego_r.pdb / FableWin.pdb carry the vector<U?$pair@KVCDefString@@@std@@> instantiation (?$vector@U?$pair@KVCDefString@@@std@@V?$allocator@).
// Model: the shared stlport/fable_map.h (STLport 4.6 _tree.h / _tree.c; FABLE_RETAIL_STLP_MAP); the explicit instantiation emits the member as
//   its own COMDAT. Replaces the generated stand-in (was _unattributed, a descriptive placeholder class).
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#include "stlport/fable_map.h"
#include "engine/CDefString.h"
template void std::vector<std::pair<unsigned long, CDefString>, std::allocator<std::pair<unsigned long, CDefString> > >::_M_clear(void);
