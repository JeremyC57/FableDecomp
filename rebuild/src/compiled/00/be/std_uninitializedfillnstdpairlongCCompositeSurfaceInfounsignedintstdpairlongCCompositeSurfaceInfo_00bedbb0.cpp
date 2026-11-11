// std::__uninitialized_fill_n(std::pair<long,CCompositeSurfaceInfo>* first, unsigned int n, std::pair<long,CCompositeSurfaceInfo> const& x, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first, return the end
// Leaf: ??$__uninitialized_fill_n@PAU?$pair@JVCCompositeSurfaceInfo@@@std@@IU12@@std@@YIPAU?$pair@JVCCompositeSurfaceInfo@@@0@PAU10@IABU10@ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BEDBB0, 63 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 00beef90 source (STLvector_Minsertoverflow_00beef90.cpp) names std::pair<long,CCompositeSurfaceInfo>; binder ??$__uninitialized_fill_n@PAU?$pair@JVCCompositeSurfaceInfo@@@std@@IU12@@std@@YIPAU?$pair@JVCCompositeSurfaceInfo@@@0@PAU10@IABU10@ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over std::pair<long,CCompositeSurfaceInfo>.
#include "stlport/fable_containers.h"
class CCompositeSurfaceCache;

#include "engine/CCountedPointer.h"

class CCompositeSurfaceInfo { public:                    // vtable 0x012A8154
    virtual void v00();
    CCountedPointer<CCompositeSurfaceCache> Cache;                        // +4
};
template std::pair<long,CCompositeSurfaceInfo>* std::__uninitialized_fill_n<std::pair<long,CCompositeSurfaceInfo>*,unsigned int,std::pair<long,CCompositeSurfaceInfo> >(std::pair<long,CCompositeSurfaceInfo>*, unsigned int, std::pair<long,CCompositeSurfaceInfo> const&, std::__false_type const&);
