// std::__uninitialized_copy(std::pair<long,COpinionDeed*>* first, std::pair<long,COpinionDeed*>* last, std::pair<long,COpinionDeed*>* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PAU?$pair@JPAVCOpinionDeed@@@std@@PAU12@@std@@YIPAU?$pair@JPAVCOpinionDeed@@@0@PAU10@00ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x006C5220, 44 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 006c8610 source (stdvector_Mfillinsert_006c8610.cpp) names std::pair<long,COpinionDeed*>; binder ??$__uninitialized_copy@PAU?$pair@JPAVCOpinionDeed@@@std@@PAU12@@std@@YIPAU?$pair@JPAVCOpinionDeed@@@0@PAU10@00ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over std::pair<long,COpinionDeed*>.
#include "stlport/fable_containers.h"
class COpinionDeed;
template std::pair<long,COpinionDeed*>* std::__uninitialized_copy<std::pair<long,COpinionDeed*>*,std::pair<long,COpinionDeed*>* >(std::pair<long,COpinionDeed*>*, std::pair<long,COpinionDeed*>*, std::pair<long,COpinionDeed*>*, std::__false_type const&);
