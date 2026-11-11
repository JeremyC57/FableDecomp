// std::__uninitialized_copy(CIDrawEnvironmentParams::CThemeChannel* first, CIDrawEnvironmentParams::CThemeChannel* last, CIDrawEnvironmentParams::CThemeChannel* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PAUCThemeChannel@CIDrawEnvironmentParams@@PAU12@@std@@YIPAUCThemeChannel@CIDrawEnvironmentParams@@PAU12@00ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x006B6500, 52 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: callee 006b6010 CIDrawEnvironmentParams::CThemeChannel::CThemeChannel; binder ??$__uninitialized_copy@PAUCThemeChannel@CIDrawEnvironmentParams@@PAU12@@std@@YIPAUCThemeChannel@CIDrawEnvironmentParams@@PAU12@00ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CIDrawEnvironmentParams::CThemeChannel.
#include "stlport/fable_containers.h"
struct CIDrawEnvironmentParams {
public:
struct CThemeChannel {
    unsigned char _body[0x1f0];
    CThemeChannel(const CThemeChannel& other);           // 0x006B6010
};
};
template CIDrawEnvironmentParams::CThemeChannel* std::__uninitialized_copy<CIDrawEnvironmentParams::CThemeChannel*,CIDrawEnvironmentParams::CThemeChannel* >(CIDrawEnvironmentParams::CThemeChannel*, CIDrawEnvironmentParams::CThemeChannel*, CIDrawEnvironmentParams::CThemeChannel*, std::__false_type const&);
