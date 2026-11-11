// std::__copy_backward(CIDrawEnvironmentParams::CThemeChannel* first, CIDrawEnvironmentParams::CThemeChannel* last, CIDrawEnvironmentParams::CThemeChannel* result, std::random_access_iterator_tag const&, int*) -- STLport 4.6 _algobase.h: random-access counted backward copy `for (n = last - first; n > 0; --n) *--result = *--last`, return the moved-back result
// Leaf: ??$__copy_backward@PAUCThemeChannel@CIDrawEnvironmentParams@@PAU12@H@std@@YIPAUCThemeChannel@CIDrawEnvironmentParams@@PAU12@00ABUrandom_access_iterator_tag@0@PAH@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x006B6250, 88 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 0xc`.
// Identity: caller 006b6b30 source (stdvector_Mfillinsert_006b6b30.cpp) names CIDrawEnvironmentParams::CThemeChannel; binder ??$__copy_backward@PAUCThemeChannel@CIDrawEnvironmentParams@@PAU12@H@std@@YIPAUCThemeChannel@CIDrawEnvironmentParams@@PAU12@00ABUrandom_access_iterator_tag@0@PAH@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CIDrawEnvironmentParams::CThemeChannel.
#define FABLE_RETAIL_STLP_RANDOM_ACCESS_COPY_BACKWARD
#include "stlport/fable_containers.h"
struct CIDrawEnvironmentParams {
public:
struct CThemeChannel {
    unsigned char _body[0x1F0];
    CThemeChannel& operator=(const CThemeChannel& other);   // 0x006B6180
};
};
template CIDrawEnvironmentParams::CThemeChannel* std::__copy_backward<CIDrawEnvironmentParams::CThemeChannel*,CIDrawEnvironmentParams::CThemeChannel*,int >(CIDrawEnvironmentParams::CThemeChannel*, CIDrawEnvironmentParams::CThemeChannel*, CIDrawEnvironmentParams::CThemeChannel*, std::random_access_iterator_tag const&, int*);
