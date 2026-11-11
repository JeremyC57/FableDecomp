// std::__copy_backward(NEnvironmentInternal::CBlendedEnvironmentTheme* first, NEnvironmentInternal::CBlendedEnvironmentTheme* last, NEnvironmentInternal::CBlendedEnvironmentTheme* result, std::random_access_iterator_tag const&, int*) -- STLport 4.6 _algobase.h: random-access counted backward copy `for (n = last - first; n > 0; --n) *--result = *--last`, return the moved-back result
// Leaf: ??$__copy_backward@PAVCBlendedEnvironmentTheme@NEnvironmentInternal@@PAV12@H@std@@YIPAVCBlendedEnvironmentTheme@NEnvironmentInternal@@PAV12@00ABUrandom_access_iterator_tag@0@PAH@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x008898C0, 73 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 0xc`.
// Identity: caller 0088a180 std::vector<NEnvironmentInternal::CBlendedEnvironmentTheme, std::allocator<NEnvironmentInternal::CBlendedEnvironmentTheme>>::_M_fill_insert; callee 006b5df0 NEnvironmentInternal::CBlendedEnvironmentTheme::operator=; binder ??$__copy_backward@PAVCBlendedEnvironmentTheme@NEnvironmentInternal@@PAV12@H@std@@YIPAVCBlendedEnvironmentTheme@NEnvironmentInternal@@PAV12@00ABUrandom_access_iterator_tag@0@PAH@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over NEnvironmentInternal::CBlendedEnvironmentTheme.
#define FABLE_RETAIL_STLP_RANDOM_ACCESS_COPY_BACKWARD
#include "stlport/fable_containers.h"
namespace NEnvironmentInternal {
class CBlendedEnvironmentTheme {
public:
    unsigned char _body[0x1F4];
    CBlendedEnvironmentTheme& operator=(const CBlendedEnvironmentTheme& other);   // 0x006B5DF0
};
}
template NEnvironmentInternal::CBlendedEnvironmentTheme* std::__copy_backward<NEnvironmentInternal::CBlendedEnvironmentTheme*,NEnvironmentInternal::CBlendedEnvironmentTheme*,int >(NEnvironmentInternal::CBlendedEnvironmentTheme*, NEnvironmentInternal::CBlendedEnvironmentTheme*, NEnvironmentInternal::CBlendedEnvironmentTheme*, std::random_access_iterator_tag const&, int*);
