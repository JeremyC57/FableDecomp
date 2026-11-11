// std::__copy_backward(CTextureMorphEntry* first, CTextureMorphEntry* last, CTextureMorphEntry* result, std::random_access_iterator_tag const&, int*) -- STLport 4.6 _algobase.h: random-access counted backward copy `for (n = last - first; n > 0; --n) *--result = *--last`, return the moved-back result
// Leaf: ??$__copy_backward@PAVCTextureMorphEntry@@PAV1@H@std@@YIPAVCTextureMorphEntry@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x008197B0, 89 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 0xc`.
// Identity: caller 0081d0b0 source (stdvector_Mfillinsert_0081d0b0.cpp) names CTextureMorphEntry; binder ??$__copy_backward@PAVCTextureMorphEntry@@PAV1@H@std@@YIPAVCTextureMorphEntry@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CTextureMorphEntry.
#define FABLE_RETAIL_STLP_RANDOM_ACCESS_COPY_BACKWARD
#include "stlport/fable_containers.h"
class CTextureMorphEntry {
public:
    unsigned char _body[0x14];
};
template CTextureMorphEntry* std::__copy_backward<CTextureMorphEntry*,CTextureMorphEntry*,int >(CTextureMorphEntry*, CTextureMorphEntry*, CTextureMorphEntry*, std::random_access_iterator_tag const&, int*);
