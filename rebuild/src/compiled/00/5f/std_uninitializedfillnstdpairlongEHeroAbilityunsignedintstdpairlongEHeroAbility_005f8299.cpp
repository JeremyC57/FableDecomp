// std::__uninitialized_fill_n(std::pair<long,EHeroAbility>* first, unsigned int n, std::pair<long,EHeroAbility> const& x, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first, return the end
// Leaf: ??$__uninitialized_fill_n@PAU?$pair@JW4EHeroAbility@@@std@@IU12@@std@@YIPAU?$pair@JW4EHeroAbility@@@0@PAU10@IABU10@ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x005F8299, 35 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 005f957e std::vector<std::pair<long, EHeroAbility>, std::allocator<std::pair<long, EHeroAbility>>>::_M_insert_overflow; binder ??$__uninitialized_fill_n@PAU?$pair@JW4EHeroAbility@@@std@@IU12@@std@@YIPAU?$pair@JW4EHeroAbility@@@0@PAU10@IABU10@ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over std::pair<long,EHeroAbility>.
#pragma optimize("s", on)
#include "stlport/fable_containers.h"
enum EHeroAbility { };
template std::pair<long,EHeroAbility>* std::__uninitialized_fill_n<std::pair<long,EHeroAbility>*,unsigned int,std::pair<long,EHeroAbility> >(std::pair<long,EHeroAbility>*, unsigned int, std::pair<long,EHeroAbility> const&, std::__false_type const&);
