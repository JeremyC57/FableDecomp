// std::__uninitialized_copy(CTexture* first, CTexture* last, CTexture* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PAVCTexture@@PAV1@@std@@YIPAVCTexture@@PAV1@00ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x0049AD70, 46 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 0049b480 std::vector<CTexture>::_M_insert_overflow; caller 0049b5a0 std::vector<CTexture>::_M_fill_insert; caller 0049b5a0 std::vector<CTexture>::_M_fill_insert; binder ??$__uninitialized_copy@PAVCTexture@@PAV1@@std@@YIPAVCTexture@@PAV1@00ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CTexture.
#define FABLE_RETAIL_CTEXTURE
#include "stlport/fable_containers.h"
#include "engine/CTexture.h"
template CTexture* std::__uninitialized_copy<CTexture*,CTexture* >(CTexture*, CTexture*, CTexture*, std::__false_type const&);
