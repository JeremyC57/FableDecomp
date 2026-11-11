// std::__uninitialized_copy(NEnvironmentInternal::CEnvironmentThemeSetDay* first, NEnvironmentInternal::CEnvironmentThemeSetDay* last, NEnvironmentInternal::CEnvironmentThemeSetDay* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PAVCEnvironmentThemeSetDay@NEnvironmentInternal@@PAV12@@std@@YIPAVCEnvironmentThemeSetDay@NEnvironmentInternal@@PAV12@00ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x0088A410, 46 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 0088a700 std::vector<NEnvironmentInternal::CEnvironmentThemeSetDay, std::allocator<NEnvironmentInternal::CEnvironmentThemeSetDay>>::_M_insert_overflow; callee 008899c0 std::vector::vector; binder ??$__uninitialized_copy@PAVCEnvironmentThemeSetDay@NEnvironmentInternal@@PAV12@@std@@YIPAVCEnvironmentThemeSetDay@NEnvironmentInternal@@PAV12@00ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over NEnvironmentInternal::CEnvironmentThemeSetDay.
#include "stlport/fable_containers.h"
namespace NEnvironmentInternal {
class CEnvironmentThemeSetDay {
public:
    unsigned char _body[0xc];
    CEnvironmentThemeSetDay(const CEnvironmentThemeSetDay& other);           // 0x008899C0
};
}
template NEnvironmentInternal::CEnvironmentThemeSetDay* std::__uninitialized_copy<NEnvironmentInternal::CEnvironmentThemeSetDay*,NEnvironmentInternal::CEnvironmentThemeSetDay* >(NEnvironmentInternal::CEnvironmentThemeSetDay*, NEnvironmentInternal::CEnvironmentThemeSetDay*, NEnvironmentInternal::CEnvironmentThemeSetDay*, std::__false_type const&);
