// std::uninitialized_fill_n(std::pair<long,COpinionDeed*>* first, unsigned int n, std::pair<long,COpinionDeed*> const& x) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first (the __false_type helper inlined), return the end
// Leaf: ??$uninitialized_fill_n@PAU?$pair@JPAVCOpinionDeed@@@std@@IU12@@std@@YIPAU?$pair@JPAVCOpinionDeed@@@0@PAU10@IABU10@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x006C5280, 40 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 006c8610 source (stdvector_Mfillinsert_006c8610.cpp) names std::pair<long,COpinionDeed*>; binder ??$uninitialized_fill_n@PAU?$pair@JPAVCOpinionDeed@@@std@@IU12@@std@@YIPAU?$pair@JPAVCOpinionDeed@@@0@PAU10@IABU10@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over std::pair<long,COpinionDeed*>.
#include "stlport/fable_containers.h"
class COpinionDeed;
template std::pair<long,COpinionDeed*>* std::uninitialized_fill_n<std::pair<long,COpinionDeed*>*,unsigned int,std::pair<long,COpinionDeed*> >(std::pair<long,COpinionDeed*>*, unsigned int, std::pair<long,COpinionDeed*> const&);
