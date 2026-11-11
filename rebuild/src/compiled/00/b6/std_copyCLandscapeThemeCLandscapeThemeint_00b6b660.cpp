// std::__copy(CLandscapeTheme* first, CLandscapeTheme* last, CLandscapeTheme* result, std::random_access_iterator_tag const&, int*) -- STLport 4.6 _algobase.h: random-access counted forward copy `for (n = last - first; n > 0; --n) *result++ = *first++`, return the advanced result
// Leaf: ??$__copy@PAUCLandscapeTheme@@PAU1@H@std@@YIPAUCLandscapeTheme@@PAU1@00ABUrandom_access_iterator_tag@0@PAH@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B6B660, 72 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 0xc`.
// Identity: caller 00b6cb20 std::vector<CLandscapeTheme, std::allocator<CLandscapeTheme>>::resize; binder ??$__copy@PAUCLandscapeTheme@@PAU1@H@std@@YIPAUCLandscapeTheme@@PAU1@00ABUrandom_access_iterator_tag@0@PAH@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CLandscapeTheme.
#include "stlport/fable_containers.h"
struct CLandscapeTheme {
    unsigned char _body[0x38];
};
template CLandscapeTheme* std::__copy<CLandscapeTheme*,CLandscapeTheme*,int >(CLandscapeTheme*, CLandscapeTheme*, CLandscapeTheme*, std::random_access_iterator_tag const&, int*);
