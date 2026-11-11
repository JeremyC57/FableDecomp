// std::__uninitialized_fill_n(std::pair<long,CDefPointer<CHeroAbilityDef const> >* first, unsigned int n, std::pair<long,CDefPointer<CHeroAbilityDef const> > const& x, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first, return the end
// Leaf: ??$__uninitialized_fill_n@PAU?$pair@JV?$CDefPointer@$$CBVCHeroAbilityDef@@@@@std@@IU12@@std@@YIPAU?$pair@JV?$CDefPointer@$$CBVCHeroAbilityDef@@@@@0@PAU10@IABU10@ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x005F9379, 38 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 005f9bfe std::vector<pair<long,CDefPointer<const CHeroAbilityDef> > >::vector; callee 005f8578 std::pair<long,CDefPointer<const CHeroAbilityDef> >::pair; binder ??$__uninitialized_fill_n@PAU?$pair@JV?$CDefPointer@$$CBVCHeroAbilityDef@@@@@std@@IU12@@std@@YIPAU?$pair@JV?$CDefPointer@$$CBVCHeroAbilityDef@@@@@0@PAU10@IABU10@ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over std::pair<long,CDefPointer<CHeroAbilityDef const>>.
#pragma optimize("s",on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "stlport/fable_containers.h"
#include "engine/CDefPointer.h"
#include "engine/CHeroAbilityDef.h"
template std::pair<long,CDefPointer<CHeroAbilityDef const> >* std::__uninitialized_fill_n<std::pair<long,CDefPointer<CHeroAbilityDef const> >*,unsigned int,std::pair<long,CDefPointer<CHeroAbilityDef const> > >(std::pair<long,CDefPointer<CHeroAbilityDef const> >*, unsigned int, std::pair<long,CDefPointer<CHeroAbilityDef const> > const&, std::__false_type const&);
