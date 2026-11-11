// std::__destroy_aux(CLensFlareElementDef* first, CLensFlareElementDef* last, std::__false_type const&) -- STLport 4.6 _construct.h: run the destructor of every element of [first, last) (the non-trivial-destructor path)
// Leaf: ??$__destroy_aux@PAVCLensFlareElementDef@@@std@@YIXPAVCLensFlareElementDef@@0ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00450A74, 28 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 0046f565 std::vector<CLensFlareElementDef, std::allocator<CLensFlareElementDef>>::operator=; binder ??$__destroy_aux@PAVCLensFlareElementDef@@@std@@YIXPAVCLensFlareElementDef@@0ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CLensFlareElementDef.
#pragma optimize("s",on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "stlport/fable_containers.h"
#include "engine/CLensFlareElementDef.h"
template void std::__destroy_aux<CLensFlareElementDef* >(CLensFlareElementDef*, CLensFlareElementDef*, std::__false_type const&);
