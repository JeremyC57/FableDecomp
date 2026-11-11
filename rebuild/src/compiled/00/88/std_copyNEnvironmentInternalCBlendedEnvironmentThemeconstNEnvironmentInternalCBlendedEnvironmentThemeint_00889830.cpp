// std::__copy(NEnvironmentInternal::CBlendedEnvironmentTheme const* first, NEnvironmentInternal::CBlendedEnvironmentTheme const* last, NEnvironmentInternal::CBlendedEnvironmentTheme* result, std::random_access_iterator_tag const&, int*) -- STLport 4.6 _algobase.h: random-access counted forward copy `for (n = last - first; n > 0; --n) *result++ = *first++`, return the advanced result
// Leaf: ??$__copy@PBVCBlendedEnvironmentTheme@NEnvironmentInternal@@PAV12@H@std@@YIPAVCBlendedEnvironmentTheme@NEnvironmentInternal@@PBV12@0PAV12@ABUrandom_access_iterator_tag@0@PAH@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00889830, 73 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 0xc`.
// Identity: callee 006b5df0 NEnvironmentInternal::CBlendedEnvironmentTheme::operator=; binder ??$__copy@PBVCBlendedEnvironmentTheme@NEnvironmentInternal@@PAV12@H@std@@YIPAVCBlendedEnvironmentTheme@NEnvironmentInternal@@PBV12@0PAV12@ABUrandom_access_iterator_tag@0@PAH@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over NEnvironmentInternal::CBlendedEnvironmentTheme.
#include "stlport/fable_containers.h"
namespace NEnvironmentInternal {
class CBlendedEnvironmentTheme {
public:                // 500 bytes (0x1f4); its own assignment copies the body
    unsigned char _body[0x1f4];
    CBlendedEnvironmentTheme& operator=(const CBlendedEnvironmentTheme& other);   // 0x006B5DF0
};
}
template NEnvironmentInternal::CBlendedEnvironmentTheme* std::__copy<NEnvironmentInternal::CBlendedEnvironmentTheme const*,NEnvironmentInternal::CBlendedEnvironmentTheme*,int >(NEnvironmentInternal::CBlendedEnvironmentTheme const*, NEnvironmentInternal::CBlendedEnvironmentTheme const*, NEnvironmentInternal::CBlendedEnvironmentTheme*, std::random_access_iterator_tag const&, int*);
