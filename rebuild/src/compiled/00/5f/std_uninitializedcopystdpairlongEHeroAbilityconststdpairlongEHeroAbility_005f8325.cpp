// std::__uninitialized_copy(std::pair<long,EHeroAbility> const* first, std::pair<long,EHeroAbility> const* last, std::pair<long,EHeroAbility>* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PBU?$pair@JW4EHeroAbility@@@std@@PAU12@@std@@YIPAU?$pair@JW4EHeroAbility@@@0@PBU10@0PAU10@ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x005F8325, 37 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 005f8bd8 source (stdvectorstdpairlongEHeroAbility_Mallocateandcopy_005f8bd8.cpp) names std::pair<long,EHeroAbility>; caller 005f9074 source (STLvector_operator_005f9074.cpp) names std::pair<long,EHeroAbility>; binder ??$__uninitialized_copy@PBU?$pair@JW4EHeroAbility@@@std@@PAU12@@std@@YIPAU?$pair@JW4EHeroAbility@@@0@PBU10@0PAU10@ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over std::pair<long,EHeroAbility>.
#pragma optimize("s", on)
#include "stlport/fable_containers.h"
enum EHeroAbility { };
template std::pair<long,EHeroAbility>* std::__uninitialized_copy<std::pair<long,EHeroAbility> const*,std::pair<long,EHeroAbility>* >(std::pair<long,EHeroAbility> const*, std::pair<long,EHeroAbility> const*, std::pair<long,EHeroAbility>*, std::__false_type const&);
