// std::__uninitialized_copy(std::pair<unsigned __int64,CANavQuadTreeNode*>* first, std::pair<unsigned __int64,CANavQuadTreeNode*>* last, std::pair<unsigned __int64,CANavQuadTreeNode*>* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PAU?$pair@_KPAVCANavQuadTreeNode@@@std@@PAU12@@std@@YIPAU?$pair@_KPAVCANavQuadTreeNode@@@0@PAU10@00ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A7E630, 50 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 00a82480 source (stdvector_Minsertoverflow_00a82480.cpp) names std::pair<unsigned __int64,CANavQuadTreeNode*>; binder ??$__uninitialized_copy@PAU?$pair@_KPAVCANavQuadTreeNode@@@std@@PAU12@@std@@YIPAU?$pair@_KPAVCANavQuadTreeNode@@@0@PAU10@00ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over std::pair<unsigned __int64,CANavQuadTreeNode*>.
#include "stlport/fable_containers.h"
class CANavQuadTreeNode;
template std::pair<unsigned __int64,CANavQuadTreeNode*>* std::__uninitialized_copy<std::pair<unsigned __int64,CANavQuadTreeNode*>*,std::pair<unsigned __int64,CANavQuadTreeNode*>* >(std::pair<unsigned __int64,CANavQuadTreeNode*>*, std::pair<unsigned __int64,CANavQuadTreeNode*>*, std::pair<unsigned __int64,CANavQuadTreeNode*>*, std::__false_type const&);
