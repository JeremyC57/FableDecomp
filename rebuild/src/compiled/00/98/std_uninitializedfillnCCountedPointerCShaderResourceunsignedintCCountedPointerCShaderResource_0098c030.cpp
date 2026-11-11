// std::__uninitialized_fill_n(CCountedPointer<CShaderResource>* first, unsigned int n, CCountedPointer<CShaderResource> const& x, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first, return the end
// Leaf: ??$__uninitialized_fill_n@PAV?$CCountedPointer@VCShaderResource@@@@IV1@@std@@YIPAV?$CCountedPointer@VCShaderResource@@@@PAV1@IABV1@ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x0098C030, 46 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 0098dc30 source (stdvector_Minsertoverflow_0098dc30.cpp) names CCountedPointer<CShaderResource>; binder ??$__uninitialized_fill_n@PAV?$CCountedPointer@VCShaderResource@@@@IV1@@std@@YIPAV?$CCountedPointer@VCShaderResource@@@@PAV1@IABV1@ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CCountedPointer<CShaderResource>.
#include "stlport/fable_containers.h"
#include "engine/CCountedPointer.h"
#include "engine/CShaderResource.h"
template CCountedPointer<CShaderResource>* std::__uninitialized_fill_n<CCountedPointer<CShaderResource>*,unsigned int,CCountedPointer<CShaderResource> >(CCountedPointer<CShaderResource>*, unsigned int, CCountedPointer<CShaderResource> const&, std::__false_type const&);
