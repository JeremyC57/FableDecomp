// std::__copy_backward(CKeyCamera* first, CKeyCamera* last, CKeyCamera* result, std::random_access_iterator_tag const&, int*) -- STLport 4.6 _algobase.h: random-access counted backward copy `for (n = last - first; n > 0; --n) *--result = *--last`, return the moved-back result
// Leaf: ??$__copy_backward@PAVCKeyCamera@@PAV1@H@std@@YIPAVCKeyCamera@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x0075F0E0, 72 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 0xc`.
// Identity: caller 0075fae0 std::vector<CKeyCamera>::insert; callee 0075ef00 CKeyCamera::operator=; binder ??$__copy_backward@PAVCKeyCamera@@PAV1@H@std@@YIPAVCKeyCamera@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CKeyCamera.
#define FABLE_RETAIL_STLP_RANDOM_ACCESS_COPY_BACKWARD
#include "stlport/fable_containers.h"
class CKeyCamera {
public:
    unsigned char _body[0x38];
    CKeyCamera& operator=(const CKeyCamera& other);   // 0x0075EF00
};
template CKeyCamera* std::__copy_backward<CKeyCamera*,CKeyCamera*,int >(CKeyCamera*, CKeyCamera*, CKeyCamera*, std::random_access_iterator_tag const&, int*);
