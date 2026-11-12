// std::vector<CVectorMap<unsigned long, CRandomAppearanceMorph::CTextureMorph, CKeyPairCompareLess<unsigned long, CRandomAppearanceMorph::CTextureMorph> >, std::allocator<CVectorMap<unsigned long, CRandomAppearanceMorph::CTextureMorph, CKeyPairCompareLess<unsigned long, CRandomAppearanceMorph::CTextureMorph> > > >::_M_clear(void) -- STLport 4.6 _M_clear (stlport/fable_map.h)
// Leaf: ?_M_clear@?$vector@V?$CVectorMap@KVCTextureMorph@CRandomAppearanceMorph@@V?$CKeyPairCompareLess@KVCTextureMorph@CRandomAppearanceMorph@@@@@@V?$allocator@V?$CVectorMap@KVCTextureMorph@CRandomAppearanceMorph@@V?$CKeyPairCompareLess@KVCTextureMorph@CRandomAppearanceMorph@@@@@@@std@@@std@@IAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00675340, oracle length 57 bytes.
// Identity: (1) the landed caller 0x0086E1C0 (std::vector::_M_insert_overflow) binds this exact spelling (rebuild/oracles/reloc_bindings.tsv); (2) Ego_r.pdb / FableWin.pdb carry the vector<V?$CVectorMap@KVCTextureMorph@CRandomAppearanceMorph@@V?$CKeyPairCompareLess@KVCTextureMorph@CRandomAppearanceMorph@@@@@@> instantiation (?$vector@V?$CVectorMap@KVCTextureMorph@CRandomAppearanceMorph@@V?$CKeyPairCompareLess@KVCTextureMorph@CRandomAppearanceMorph@@@@@@V?$allocator@).
// Model: the shared stlport/fable_map.h (STLport 4.6 _tree.h / _tree.c; FABLE_RETAIL_STLP_MAP); the explicit instantiation emits the member as
//   its own COMDAT. Replaces the generated stand-in (was _unattributed, a descriptive placeholder class).
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#include "stlport/fable_map.h"

#include "engine/CRandomAppearanceMorph.h"
template void std::vector<CVectorMap<unsigned long, CRandomAppearanceMorph::CTextureMorph, CKeyPairCompareLess<unsigned long, CRandomAppearanceMorph::CTextureMorph> >, std::allocator<CVectorMap<unsigned long, CRandomAppearanceMorph::CTextureMorph, CKeyPairCompareLess<unsigned long, CRandomAppearanceMorph::CTextureMorph> > > >::_M_clear(void);
