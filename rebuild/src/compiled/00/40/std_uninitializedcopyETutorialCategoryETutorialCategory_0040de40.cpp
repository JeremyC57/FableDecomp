// std::__uninitialized_copy(ETutorialCategory* first, ETutorialCategory* last, ETutorialCategory* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PAW4ETutorialCategory@@PAW41@@std@@YIPAW4ETutorialCategory@@PAW41@00ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x0040DE40, 38 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 00411f10 source (stdvectorETutorialCategory_Mfillinsert_00411f10.cpp) names ETutorialCategory; binder ??$__uninitialized_copy@PAW4ETutorialCategory@@PAW41@@std@@YIPAW4ETutorialCategory@@PAW41@00ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over ETutorialCategory.
#include "stlport/fable_containers.h"
enum ETutorialCategory { };
template ETutorialCategory* std::__uninitialized_copy<ETutorialCategory*,ETutorialCategory* >(ETutorialCategory*, ETutorialCategory*, ETutorialCategory*, std::__false_type const&);
