// std::__uninitialized_copy(NFire::CFirePoint* first, NFire::CFirePoint* last, NFire::CFirePoint* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PAVCFirePoint@NFire@@PAV12@@std@@YIPAVCFirePoint@NFire@@PAV12@00ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x007816C0, 46 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: callee 007813d0 CFirePoint::CFirePoint; binder ??$__uninitialized_copy@PAVCFirePoint@NFire@@PAV12@@std@@YIPAVCFirePoint@NFire@@PAV12@00ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over NFire::CFirePoint.
#include "stlport/fable_containers.h"
namespace NFire {
class CFirePoint {
public:
    unsigned char _body[0x2c];
    CFirePoint(const CFirePoint& other);           // 0x007813D0
};
}
template NFire::CFirePoint* std::__uninitialized_copy<NFire::CFirePoint*,NFire::CFirePoint* >(NFire::CFirePoint*, NFire::CFirePoint*, NFire::CFirePoint*, std::__false_type const&);
