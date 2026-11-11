// std::__copy(CIDrawEnvironmentParams::CThemeChannel const* first, CIDrawEnvironmentParams::CThemeChannel const* last, CIDrawEnvironmentParams::CThemeChannel* result, std::random_access_iterator_tag const&, int*) -- STLport 4.6 _algobase.h: random-access counted forward copy `for (n = last - first; n > 0; --n) *result++ = *first++`, return the advanced result
// Leaf: ??$__copy@PBUCThemeChannel@CIDrawEnvironmentParams@@PAU12@H@std@@YIPAUCThemeChannel@CIDrawEnvironmentParams@@PBU12@0PAU12@ABUrandom_access_iterator_tag@0@PAH@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x006B65C0, 77 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 0xc`.
// Identity: caller 006b6d90 source (STLvector_resize_006b6d90.cpp) names CIDrawEnvironmentParams::CThemeChannel; binder ??$__copy@PBUCThemeChannel@CIDrawEnvironmentParams@@PAU12@H@std@@YIPAUCThemeChannel@CIDrawEnvironmentParams@@PBU12@0PAU12@ABUrandom_access_iterator_tag@0@PAH@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CIDrawEnvironmentParams::CThemeChannel.
#include "stlport/fable_containers.h"
struct CIDrawEnvironmentParams {
public:
struct CThemeChannel {                           // 496 bytes (0x1f0); its own assignment copies the body
    unsigned char _body[0x1f0];
    CThemeChannel& operator=(const CThemeChannel& other);   // 0x006B6180
};
};
template CIDrawEnvironmentParams::CThemeChannel* std::__copy<CIDrawEnvironmentParams::CThemeChannel const*,CIDrawEnvironmentParams::CThemeChannel*,int >(CIDrawEnvironmentParams::CThemeChannel const*, CIDrawEnvironmentParams::CThemeChannel const*, CIDrawEnvironmentParams::CThemeChannel*, std::random_access_iterator_tag const&, int*);
