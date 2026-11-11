// std::__copy_backward(std::pair<unsigned long,CCountedPointer<CFaction> >* first, std::pair<unsigned long,CCountedPointer<CFaction> >* last, std::pair<unsigned long,CCountedPointer<CFaction> >* result, std::random_access_iterator_tag const&, int*) -- STLport 4.6 _algobase.h: random-access counted backward copy `for (n = last - first; n > 0; --n) *--result = *--last`, return the moved-back result
// Leaf: ??$__copy_backward@PAU?$pair@KV?$CCountedPointer@VCFaction@@@@@std@@PAU12@H@std@@YIPAU?$pair@KV?$CCountedPointer@VCFaction@@@@@0@PAU10@00ABUrandom_access_iterator_tag@0@PAH@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x006B9100, 149 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 0xc`.
// Identity: caller 006ba7b0 source (std_linearinsert_006ba7b0.cpp) names std::pair<unsigned long,CCountedPointer<CFaction>>; binder ??$__copy_backward@PAU?$pair@KV?$CCountedPointer@VCFaction@@@@@std@@PAU12@H@std@@YIPAU?$pair@KV?$CCountedPointer@VCFaction@@@@@0@PAU10@00ABUrandom_access_iterator_tag@0@PAH@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over std::pair<unsigned long,CCountedPointer<CFaction>>.
#define FABLE_RETAIL_STLP_RANDOM_ACCESS_COPY_BACKWARD
#include "stlport/fable_containers.h"
#include "engine/CCountedPointer.h"
class CFaction;
template std::pair<unsigned long,CCountedPointer<CFaction> >* std::__copy_backward<std::pair<unsigned long,CCountedPointer<CFaction> >*,std::pair<unsigned long,CCountedPointer<CFaction> >*,int >(std::pair<unsigned long,CCountedPointer<CFaction> >*, std::pair<unsigned long,CCountedPointer<CFaction> >*, std::pair<unsigned long,CCountedPointer<CFaction> >*, std::random_access_iterator_tag const&, int*);
