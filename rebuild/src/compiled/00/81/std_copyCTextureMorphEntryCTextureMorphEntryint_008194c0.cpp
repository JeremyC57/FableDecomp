// std::__copy(CTextureMorphEntry* first, CTextureMorphEntry* last, CTextureMorphEntry* result, std::random_access_iterator_tag const&, int*) -- STLport 4.6 _algobase.h: random-access counted forward copy `for (n = last - first; n > 0; --n) *result++ = *first++`, return the advanced result
// Leaf: ??$__copy@PAVCTextureMorphEntry@@PAV1@H@std@@YIPAVCTextureMorphEntry@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x008194C0, 86 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 0xc`.
// Identity: caller 0081daa0 std::vector<CTextureMorphEntry, std::allocator<CTextureMorphEntry>>::resize; binder ??$__copy@PAVCTextureMorphEntry@@PAV1@H@std@@YIPAVCTextureMorphEntry@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CTextureMorphEntry.
#include "stlport/fable_containers.h"
class CTextureMorphEntry {
public:
    unsigned char _body[0x14];
};
template CTextureMorphEntry* std::__copy<CTextureMorphEntry*,CTextureMorphEntry*,int >(CTextureMorphEntry*, CTextureMorphEntry*, CTextureMorphEntry*, std::random_access_iterator_tag const&, int*);
