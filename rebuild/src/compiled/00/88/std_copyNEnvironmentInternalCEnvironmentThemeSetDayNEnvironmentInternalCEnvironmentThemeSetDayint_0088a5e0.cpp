// std::__copy(NEnvironmentInternal::CEnvironmentThemeSetDay* first, NEnvironmentInternal::CEnvironmentThemeSetDay* last, NEnvironmentInternal::CEnvironmentThemeSetDay* result, std::random_access_iterator_tag const&, int*) -- STLport 4.6 _algobase.h: random-access counted forward copy `for (n = last - first; n > 0; --n) *result++ = *first++`, return the advanced result
// Leaf: ??$__copy@PAVCEnvironmentThemeSetDay@NEnvironmentInternal@@PAV12@H@std@@YIPAVCEnvironmentThemeSetDay@NEnvironmentInternal@@PAV12@00ABUrandom_access_iterator_tag@0@PAH@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x0088A5E0, 66 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 0xc`.
// Identity: caller 0088a9c0 std::vector<NEnvironmentInternal::CEnvironmentThemeSetDay, std::allocator<NEnvironmentInternal::CEnvironmentThemeSetDay>>::resize; binder ??$__copy@PAVCEnvironmentThemeSetDay@NEnvironmentInternal@@PAV12@H@std@@YIPAVCEnvironmentThemeSetDay@NEnvironmentInternal@@PAV12@00ABUrandom_access_iterator_tag@0@PAH@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over NEnvironmentInternal::CEnvironmentThemeSetDay.
#include "stlport/fable_containers.h"
namespace NEnvironmentInternal {
class CEnvironmentThemeSetDay {
public:
    unsigned char _body[0xc];
    CEnvironmentThemeSetDay& operator=(const CEnvironmentThemeSetDay& other);   // 0x00889F00
};
}
template NEnvironmentInternal::CEnvironmentThemeSetDay* std::__copy<NEnvironmentInternal::CEnvironmentThemeSetDay*,NEnvironmentInternal::CEnvironmentThemeSetDay*,int >(NEnvironmentInternal::CEnvironmentThemeSetDay*, NEnvironmentInternal::CEnvironmentThemeSetDay*, NEnvironmentInternal::CEnvironmentThemeSetDay*, std::random_access_iterator_tag const&, int*);
