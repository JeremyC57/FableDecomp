// std::vector<CDefString, std::allocator<CDefString> >::_M_clear(void) -- STLport 4.6 _M_clear (stlport/fable_map.h)
// Leaf: ?_M_clear@?$vector@VCDefString@@V?$allocator@VCDefString@@@std@@@std@@IAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0046089A, oracle length ? bytes.
// Identity: (1) the landed caller 0x0046078C (std::vector<CDefString, std::allocator<CDefString>>::operator=) binds this exact spelling (rebuild/oracles/reloc_bindings.tsv); (2) Ego_r.pdb / FableWin.pdb carry the vector<VCDefString@@> instantiation (?$vector@VCDefString@@V?$allocator@).
// Model: the shared stlport/fable_map.h (STLport 4.6 _tree.h / _tree.c; FABLE_RETAIL_STLP_MAP); the explicit instantiation emits the member as
//   its own COMDAT. Replaces the generated stand-in (was _unattributed, a descriptive placeholder class).
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#include "stlport/fable_map.h"
#include "engine/CDefString.h"
template void std::vector<CDefString, std::allocator<CDefString> >::_M_clear(void);
