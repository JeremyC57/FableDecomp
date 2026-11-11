// std::__destroy_aux(CCountedPointer<CShaderResource>* first, CCountedPointer<CShaderResource>* last, std::__false_type const&) -- STLport 4.6 _construct.h: run the destructor of every element of [first, last) (the non-trivial-destructor path)
// Leaf: ??$__destroy_aux@PAV?$CCountedPointer@VCShaderResource@@@@@std@@YIXPAV?$CCountedPointer@VCShaderResource@@@@0ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x0098CEE0, 76 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 0098abf0 source (CShaderRenderManager_CShaderRenderManager_0098abf0.cpp) names CCountedPointer<CShaderResource>; caller 0098dc30 source (stdvector_Minsertoverflow_0098dc30.cpp) names CCountedPointer<CShaderResource>; binder ??$__destroy_aux@PAV?$CCountedPointer@VCShaderResource@@@@@std@@YIXPAV?$CCountedPointer@VCShaderResource@@@@0ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CCountedPointer<CShaderResource>.
#include "stlport/fable_containers.h"
#include "engine/CCountedPointer.h"
#include "engine/CShaderResource.h"
template void std::__destroy_aux<CCountedPointer<CShaderResource>* >(CCountedPointer<CShaderResource>*, CCountedPointer<CShaderResource>*, std::__false_type const&);
