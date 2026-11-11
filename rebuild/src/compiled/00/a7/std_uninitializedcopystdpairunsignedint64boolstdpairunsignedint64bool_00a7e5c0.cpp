// std::__uninitialized_copy(std::pair<unsigned __int64,bool>* first, std::pair<unsigned __int64,bool>* last, std::pair<unsigned __int64,bool>* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PAU?$pair@_K_N@std@@PAU12@@std@@YIPAU?$pair@_K_N@0@PAU10@00ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A7E5C0, 50 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: builtin; caller 00a82380 source (stdvector_Minsertoverflow_00a82380.cpp) names std::pair<unsigned __int64,bool>; binder ??$__uninitialized_copy@PAU?$pair@_K_N@std@@PAU12@@std@@YIPAU?$pair@_K_N@0@PAU10@00ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over std::pair<unsigned __int64,bool>.
#include "stlport/fable_containers.h"
template std::pair<unsigned __int64,bool>* std::__uninitialized_copy<std::pair<unsigned __int64,bool>*,std::pair<unsigned __int64,bool>* >(std::pair<unsigned __int64,bool>*, std::pair<unsigned __int64,bool>*, std::pair<unsigned __int64,bool>*, std::__false_type const&);
