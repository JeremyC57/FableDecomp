// std::vector<EVillageTaskType, std::allocator<EVillageTaskType> >::_M_clear(void) -- STLport 4.6 _M_clear (stlport/fable_map.h)
// Leaf: ?_M_clear@?$vector@W4EVillageTaskType@@V?$allocator@W4EVillageTaskType@@@std@@@std@@IAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E212D, oracle length ? bytes.
// Identity: (1) the landed caller 0x004E201F (std::vector<EVillageTaskType, std::allocator<EVillageTaskType>>::operator=) binds this exact spelling (rebuild/oracles/reloc_bindings.tsv); (2) Ego_r.pdb / FableWin.pdb carry the vector<W4EVillageTaskType@@> instantiation (?$vector@W4EVillageTaskType@@V?$allocator@).
// Model: the shared stlport/fable_map.h (STLport 4.6 _tree.h / _tree.c; FABLE_RETAIL_STLP_MAP); the explicit instantiation emits the member as
//   its own COMDAT. Replaces the generated stand-in (was _unattributed, a descriptive placeholder class).
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_MAP
#include "stlport/fable_map.h"
enum EVillageTaskType { };
template void std::vector<EVillageTaskType, std::allocator<EVillageTaskType> >::_M_clear(void);
