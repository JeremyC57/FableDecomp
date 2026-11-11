// std::fill(ETutorialCategory* first, ETutorialCategory* last, ETutorialCategory const& x) -- STLport 4.6 _algobase.h: assign x to every element of [first, last)
// Leaf: ??$fill@PAW4ETutorialCategory@@W41@@std@@YIXPAW4ETutorialCategory@@0ABW41@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x0040EA50, 31 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 00411f10 source (stdvectorETutorialCategory_Mfillinsert_00411f10.cpp) names ETutorialCategory; binder ??$fill@PAW4ETutorialCategory@@W41@@std@@YIXPAW4ETutorialCategory@@0ABW41@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over ETutorialCategory.
#include "stlport/fable_containers.h"
enum ETutorialCategory { };
template void std::fill<ETutorialCategory*,ETutorialCategory >(ETutorialCategory*, ETutorialCategory*, ETutorialCategory const&);
