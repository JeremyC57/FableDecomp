// std::__uninitialized_copy(std::pair<C2DVector,unsigned char>* first, std::pair<C2DVector,unsigned char>* last, std::pair<C2DVector,unsigned char>* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PAU?$pair@VC2DVector@@E@std@@PAU12@@std@@YIPAU?$pair@VC2DVector@@E@0@PAU10@00ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x008002F0, 50 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 00800da0 std::vector<pair<C2DVector,unsigned char> >::_M_insert_overflow; binder ??$__uninitialized_copy@PAU?$pair@VC2DVector@@E@std@@PAU12@@std@@YIPAU?$pair@VC2DVector@@E@0@PAU10@00ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over std::pair<C2DVector,unsigned char>.
#pragma pack(push, 1)
#pragma pack(pop)
#define FABLE_RETAIL_C2DVECTOR
#include "stlport/fable_containers.h"
#include "engine/C2DVector.h"
template std::pair<C2DVector,unsigned char>* std::__uninitialized_copy<std::pair<C2DVector,unsigned char>*,std::pair<C2DVector,unsigned char>* >(std::pair<C2DVector,unsigned char>*, std::pair<C2DVector,unsigned char>*, std::pair<C2DVector,unsigned char>*, std::__false_type const&);
