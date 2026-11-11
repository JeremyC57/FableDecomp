// std::__uninitialized_copy(std::pair<long,CDefPointer<CHeroAbilityDef const> > const* first, std::pair<long,CDefPointer<CHeroAbilityDef const> > const* last, std::pair<long,CDefPointer<CHeroAbilityDef const> >* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PBU?$pair@JV?$CDefPointer@$$CBVCHeroAbilityDef@@@@@std@@PAU12@@std@@YIPAU?$pair@JV?$CDefPointer@$$CBVCHeroAbilityDef@@@@@0@PBU10@0PAU10@ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x005F9132, 43 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 005fa1ba std::vector<pair<long,CDefPointer<const CHeroAbilityDef> > >::operator=; callee 005f8578 std::pair<long,CDefPointer<const CHeroAbilityDef> >::pair; binder ??$__uninitialized_copy@PBU?$pair@JV?$CDefPointer@$$CBVCHeroAbilityDef@@@@@std@@PAU12@@std@@YIPAU?$pair@JV?$CDefPointer@$$CBVCHeroAbilityDef@@@@@0@PBU10@0PAU10@ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over std::pair<long,CDefPointer<CHeroAbilityDef const>>.
#pragma optimize("s",on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "stlport/fable_containers.h"
#include "engine/CDefPointer.h"
#include "engine/CHeroAbilityDef.h"
template std::pair<long,CDefPointer<CHeroAbilityDef const> >* std::__uninitialized_copy<std::pair<long,CDefPointer<CHeroAbilityDef const> > const*,std::pair<long,CDefPointer<CHeroAbilityDef const> >* >(std::pair<long,CDefPointer<CHeroAbilityDef const> > const*, std::pair<long,CDefPointer<CHeroAbilityDef const> > const*, std::pair<long,CDefPointer<CHeroAbilityDef const> >*, std::__false_type const&);
