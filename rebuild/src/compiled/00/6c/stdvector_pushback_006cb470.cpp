// std::vector::push_back(value) -- Append an element, growing the buffer when full.
// Leaf: ?push_back@?$vector@VCEngineTextureMorph@@V?$allocator@VCEngineTextureMorph@@@std@@@std@@QAEXABVCEngineTextureMorph@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006CB470; no E8/E9 caller in the exe (nftools/xref.py).
// Port (NF96 part 2, q11454): the old row bound _M_insert_overflow 0x006CB390 as the public
//   ?_M_insert_overflow@...@QAEX... through local CRGBColour / CCompositeBlendDesc / std::pair / std::vector / CEngineTextureMorph
//   models; now the STLport push_back of stlport/fable_containers.h on engine/CEngineTextureMorph.h (the define set of the TU-mate
//   AddEngineMorph 0x006CA8C0), which binds the protected STLport spelling ...@IAEX... (the 0x006CB390 Leaf). The self leaf is unchanged
//   (Ego_r.pdb / FableWin.pdb name only Lionhead's ?push_back@?$CArray@VCEngineTextureMorph@@@@QAEXABVCEngineTextureMorph@@@Z for this
//   element type: the row's own name is not settled here, NOTES).
#define FABLE_RETAIL_CARRAY_PUSHBACK
#define FABLE_STLP_OUTOFLINE_UNINIT_COPY
#define FABLE_STLP_OUTOFLINE_UNINIT_FILL_N
#define FABLE_RETAIL_CENGINETEXTUREMORPH_CTORS
#define FABLE_RETAIL_CENGINETEXTUREMORPHSET
#include "engine/CEngineTextureMorph.h"

template void std::vector<CEngineTextureMorph>::push_back(const CEngineTextureMorph&);
