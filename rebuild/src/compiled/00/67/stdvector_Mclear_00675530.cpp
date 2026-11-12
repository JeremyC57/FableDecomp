// std::vector<CRandomAppearanceMorph::CBodyPartMesh, std::allocator<CRandomAppearanceMorph::CBodyPartMesh> >::_M_clear(void) -- STLport 4.6 _M_clear (stlport/fable_map.h)
// Leaf: ?_M_clear@?$vector@VCBodyPartMesh@CRandomAppearanceMorph@@V?$allocator@VCBodyPartMesh@CRandomAppearanceMorph@@@std@@@std@@IAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00675530, oracle length 58 bytes.
// Identity: (1) the landed caller 0x006755E0 (std::vector<CRandomAppearanceMorph::CBodyPartMesh, std::allocator<CRandomAppearanceMorph::CBodyPartMesh>>::operator=) binds this exact spelling (rebuild/oracles/reloc_bindings.tsv); (2) Ego_r.pdb / FableWin.pdb carry the vector<VCBodyPartMesh@CRandomAppearanceMorph@@> instantiation (?$vector@VCBodyPartMesh@CRandomAppearanceMorph@@V?$allocator@).
// Model: the shared stlport/fable_map.h (STLport 4.6 _tree.h / _tree.c; FABLE_RETAIL_STLP_MAP); the explicit instantiation emits the member as
//   its own COMDAT. Replaces the generated stand-in (was _unattributed, a descriptive placeholder class).
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#include "stlport/fable_map.h"

#include "engine/CRandomAppearanceMorph.h"
template void std::vector<CRandomAppearanceMorph::CBodyPartMesh, std::allocator<CRandomAppearanceMorph::CBodyPartMesh> >::_M_clear(void);
