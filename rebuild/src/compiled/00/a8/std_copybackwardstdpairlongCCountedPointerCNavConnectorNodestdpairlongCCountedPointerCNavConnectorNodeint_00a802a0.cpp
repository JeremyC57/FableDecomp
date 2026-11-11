// std::__copy_backward(std::pair<long,CCountedPointer<CNavConnectorNode> >* first, std::pair<long,CCountedPointer<CNavConnectorNode> >* last, std::pair<long,CCountedPointer<CNavConnectorNode> >* result, std::random_access_iterator_tag const&, int*) -- STLport 4.6 _algobase.h: random-access counted backward copy `for (n = last - first; n > 0; --n) *--result = *--last`, return the moved-back result
// Leaf: ??$__copy_backward@PAU?$pair@JV?$CCountedPointer@VCNavConnectorNode@@@@@std@@PAU12@H@std@@YIPAU?$pair@JV?$CCountedPointer@VCNavConnectorNode@@@@@0@PAU10@00ABUrandom_access_iterator_tag@0@PAH@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A802A0, 149 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 0xc`.
// Identity: caller 00a834b0 source (STL_linearinsert_00a834b0.cpp) names std::pair<long,CCountedPointer<CNavConnectorNode>>; binder ??$__copy_backward@PAU?$pair@JV?$CCountedPointer@VCNavConnectorNode@@@@@std@@PAU12@H@std@@YIPAU?$pair@JV?$CCountedPointer@VCNavConnectorNode@@@@@0@PAU10@00ABUrandom_access_iterator_tag@0@PAH@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over std::pair<long,CCountedPointer<CNavConnectorNode>>.
#define FABLE_RETAIL_STLP_RANDOM_ACCESS_COPY_BACKWARD
#include "stlport/fable_containers.h"
#include "engine/CCountedPointer.h"
class CNavConnectorNode;
template std::pair<long,CCountedPointer<CNavConnectorNode> >* std::__copy_backward<std::pair<long,CCountedPointer<CNavConnectorNode> >*,std::pair<long,CCountedPointer<CNavConnectorNode> >*,int >(std::pair<long,CCountedPointer<CNavConnectorNode> >*, std::pair<long,CCountedPointer<CNavConnectorNode> >*, std::pair<long,CCountedPointer<CNavConnectorNode> >*, std::random_access_iterator_tag const&, int*);
