// std::fill(std::pair<long,COpinionDeed*>* first, std::pair<long,COpinionDeed*>* last, std::pair<long,COpinionDeed*> const& x) -- STLport 4.6 _algobase.h: assign x to every element of [first, last)
// Leaf: ??$fill@PAU?$pair@JPAVCOpinionDeed@@@std@@U12@@std@@YIXPAU?$pair@JPAVCOpinionDeed@@@0@0ABU10@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x006C4B90, 37 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 006c8610 source (stdvector_Mfillinsert_006c8610.cpp) names std::pair<long,COpinionDeed*>; binder ??$fill@PAU?$pair@JPAVCOpinionDeed@@@std@@U12@@std@@YIXPAU?$pair@JPAVCOpinionDeed@@@0@0ABU10@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over std::pair<long,COpinionDeed*>.
#include "stlport/fable_containers.h"
class COpinionDeed;
template void std::fill<std::pair<long,COpinionDeed*>*,std::pair<long,COpinionDeed*> >(std::pair<long,COpinionDeed*>*, std::pair<long,COpinionDeed*>*, std::pair<long,COpinionDeed*> const&);
