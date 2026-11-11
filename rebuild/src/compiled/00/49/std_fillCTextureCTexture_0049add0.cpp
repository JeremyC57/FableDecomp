// std::fill(CTexture* first, CTexture* last, CTexture const& x) -- STLport 4.6 _algobase.h: assign x to every element of [first, last)
// Leaf: ??$fill@PAVCTexture@@V1@@std@@YIXPAVCTexture@@0ABV1@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x0049ADD0, 37 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 0049b5a0 std::vector<CTexture>::_M_fill_insert; callee 009fa1c0 CTexture::operator=; binder ??$fill@PAVCTexture@@V1@@std@@YIXPAVCTexture@@0ABV1@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CTexture.
#define FABLE_RETAIL_CTEXTURE
#include "stlport/fable_containers.h"
#include "engine/CTexture.h"
template void std::fill<CTexture*,CTexture >(CTexture*, CTexture*, CTexture const&);
