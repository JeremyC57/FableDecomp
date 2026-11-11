// std::fill(KnickKnack* first, KnickKnack* last, KnickKnack const& x) -- STLport 4.6 _algobase.h: assign x to every element of [first, last)
// Leaf: ??$fill@PAUKnickKnack@@U1@@std@@YIXPAUKnickKnack@@0ABU1@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x007D96F0, 55 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 007d9d70 std::vector<KnickKnack, std::allocator<KnickKnack>>::_M_fill_insert; caller 007d9d70 std::vector<KnickKnack, std::allocator<KnickKnack>>::_M_fill_insert; binder ??$fill@PAUKnickKnack@@U1@@std@@YIXPAUKnickKnack@@0ABU1@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over KnickKnack.
#include "stlport/fable_containers.h"
struct KnickKnack { unsigned char _body[0x10];
};
template void std::fill<KnickKnack*,KnickKnack >(KnickKnack*, KnickKnack*, KnickKnack const&);
