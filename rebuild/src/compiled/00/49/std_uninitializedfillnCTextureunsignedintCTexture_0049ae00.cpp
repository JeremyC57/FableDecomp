// std::uninitialized_fill_n(CTexture* first, unsigned int n, CTexture const& x) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first (the __false_type helper inlined), return the end
// Leaf: ??$uninitialized_fill_n@PAVCTexture@@IV1@@std@@YIPAVCTexture@@PAV1@IABV1@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x0049AE00, 42 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 0049b5a0 std::vector<CTexture>::_M_fill_insert; callee 009f9d00 CTexture::CTexture; binder ??$uninitialized_fill_n@PAVCTexture@@IV1@@std@@YIPAVCTexture@@PAV1@IABV1@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CTexture.
#define FABLE_RETAIL_CTEXTURE
#include "stlport/fable_containers.h"
#include "engine/CTexture.h"
template CTexture* std::uninitialized_fill_n<CTexture*,unsigned int,CTexture >(CTexture*, unsigned int, CTexture const&);
