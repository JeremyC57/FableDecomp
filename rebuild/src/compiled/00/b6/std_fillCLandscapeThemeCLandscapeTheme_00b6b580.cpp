// std::fill(CLandscapeTheme* first, CLandscapeTheme* last, CLandscapeTheme const& x) -- STLport 4.6 _algobase.h: assign x to every element of [first, last)
// Leaf: ??$fill@PAUCLandscapeTheme@@U1@@std@@YIXPAUCLandscapeTheme@@0ABU1@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B6B580, 40 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 00b6c700 std::vector<CLandscapeTheme, std::allocator<CLandscapeTheme>>::_M_fill_insert; caller 00b6c700 std::vector<CLandscapeTheme, std::allocator<CLandscapeTheme>>::_M_fill_insert; binder ??$fill@PAUCLandscapeTheme@@U1@@std@@YIXPAUCLandscapeTheme@@0ABU1@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CLandscapeTheme.
#include "stlport/fable_containers.h"
struct CLandscapeTheme {
    unsigned char _body[0x38];
};
template void std::fill<CLandscapeTheme*,CLandscapeTheme >(CLandscapeTheme*, CLandscapeTheme*, CLandscapeTheme const&);
