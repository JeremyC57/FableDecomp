// std::vector<CEngineTextureMorph>::_M_clear() -- STLport: destroy [start, finish) (each record's Blends vector frees
// its buffer), then release the block
// Leaf: ?_M_clear@?$vector@VCEngineTextureMorph@@V?$allocator@VCEngineTextureMorph@@@std@@@std@@IAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006CB340, 58 bytes, thiscall `ret`: the end pointer is cached, every 20-byte record's
//   Blends._M_start (+0x08) is freed when non-null (free @ 0x00BFEA14), then `if (start) free(start)`.
// Identity (FINDINGS chunk 387; was the descriptive batch row CRetailVector_006cb340::Destroy_CRetailVector_006cb340):
//   its callers are the landed std::vector<CEngineTextureMorph>::_M_insert_overflow 0x006CB390 (which already declares
//   0x006CB340 as this vector's _M_clear) and 0x00728830; the 20-byte stride with an owned block at +0x08 is
//   CEngineTextureMorph (Ego_r.pdb / struct_layouts: OriginalTextureIndex +0, MorphID +4, vector<CCompositeBlendDesc>
//   Blends +8; 20 bytes in retail with the 12-byte STLport vector), whose implicit destructor is the inline
//   ~vector<CCompositeBlendDesc> (CCompositeBlendDesc is trivially destructible: no element loop, only the deallocate).
// Shape: STLport 4.x `_M_clear() { _Destroy(_M_start, _M_finish); _M_end_of_storage.deallocate(_M_start,
//   _M_end_of_storage._M_data - _M_start); }`, the whole _Destroy -> __destroy_aux(.., __false_type) -> ~T chain inline.
// Name fix (NF96 part 2, q11455): the old row defined ?_M_clear@?$vector@VCEngineTextureMorph@@@std@@... on a one-parameter local
//   std::vector model (no allocator argument) with local CRGBColour / CCompositeBlendDesc / CEngineTextureMorph; now the protected STLport
//   member on engine/CEngineTextureMorph.h + stlport/fable_containers.h (the define set of the TU-mate AddEngineMorph 0x006CA8C0, whose source
//   grades RELOCATION_MATCH here: hand4f_b5); the landed 0x00728830 binds this spelling.
#define FABLE_RETAIL_CARRAY_PUSHBACK
#define FABLE_STLP_OUTOFLINE_UNINIT_COPY
#define FABLE_STLP_OUTOFLINE_UNINIT_FILL_N
#define FABLE_RETAIL_CENGINETEXTUREMORPH_CTORS
#define FABLE_RETAIL_CENGINETEXTUREMORPHSET
#include "engine/CEngineTextureMorph.h"

template void std::vector<CEngineTextureMorph>::_M_clear();
