// std::vector<NEnvironmentInternal::CEnvironmentThemePriorityStackEntry, std::allocator<NEnvironmentInternal::CEnvironmentThemePriorityStackEntry> >::_M_clear(void) -- STLport 4.6 _M_clear (stlport/fable_map.h)
// Leaf: ?_M_clear@?$vector@VCEnvironmentThemePriorityStackEntry@NEnvironmentInternal@@V?$allocator@VCEnvironmentThemePriorityStackEntry@NEnvironmentInternal@@@std@@@std@@IAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00889A30, oracle length 58 bytes.
// Identity: (1) the landed caller 0x00889B20 (std::vector::_M_insert_overflow) binds this exact spelling (rebuild/oracles/reloc_bindings.tsv); (2) Ego_r.pdb / FableWin.pdb carry the vector<VCEnvironmentThemePriorityStackEntry@NEnvironmentInternal@@> instantiation (?$vector@VCEnvironmentThemePriorityStackEntry@NEnvironmentInternal@@V?$allocator@).
// Model: the shared stlport/fable_map.h (STLport 4.6 _tree.h / _tree.c; FABLE_RETAIL_STLP_MAP); the explicit instantiation emits the member as
//   its own COMDAT. Replaces the generated stand-in (was _unattributed, a descriptive placeholder class).
#define FABLE_RETAIL_STLP_MAP
#include "stlport/fable_map.h"
#include "engine/CEnvironmentThemePriorityStackEntry.h"
template void std::vector<NEnvironmentInternal::CEnvironmentThemePriorityStackEntry, std::allocator<NEnvironmentInternal::CEnvironmentThemePriorityStackEntry> >::_M_clear(void);
