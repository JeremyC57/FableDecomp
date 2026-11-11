// std::__copy(NUISystem::CIconText::CEmbeddedIcon* first, NUISystem::CIconText::CEmbeddedIcon* last, NUISystem::CIconText::CEmbeddedIcon* result, std::random_access_iterator_tag const&, int*) -- STLport 4.6 _algobase.h: random-access counted forward copy `for (n = last - first; n > 0; --n) *result++ = *first++`, return the advanced result
// Leaf: ??$__copy@PAUCEmbeddedIcon@CIconText@NUISystem@@PAU123@H@std@@YIPAUCEmbeddedIcon@CIconText@NUISystem@@PAU123@00ABUrandom_access_iterator_tag@0@PAH@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00543700, 72 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 0xc`.
// Identity: caller 00542730 source (NUISystemCIconText_ClearIcons_00542730.cpp) names NUISystem::CIconText::CEmbeddedIcon; binder ??$__copy@PAUCEmbeddedIcon@CIconText@NUISystem@@PAU123@H@std@@YIPAUCEmbeddedIcon@CIconText@NUISystem@@PAU123@00ABUrandom_access_iterator_tag@0@PAH@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over NUISystem::CIconText::CEmbeddedIcon.
#include "stlport/fable_containers.h"
namespace NUISystem {
class CIconText {
public:
struct CEmbeddedIcon {
    unsigned char _body[0x1c];
};
};
}
template NUISystem::CIconText::CEmbeddedIcon* std::__copy<NUISystem::CIconText::CEmbeddedIcon*,NUISystem::CIconText::CEmbeddedIcon*,int >(NUISystem::CIconText::CEmbeddedIcon*, NUISystem::CIconText::CEmbeddedIcon*, NUISystem::CIconText::CEmbeddedIcon*, std::random_access_iterator_tag const&, int*);
