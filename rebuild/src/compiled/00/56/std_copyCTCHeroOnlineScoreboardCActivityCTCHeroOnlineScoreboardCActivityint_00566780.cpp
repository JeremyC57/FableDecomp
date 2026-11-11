// std::__copy(CTCHeroOnlineScoreboard::CActivity* first, CTCHeroOnlineScoreboard::CActivity* last, CTCHeroOnlineScoreboard::CActivity* result, std::random_access_iterator_tag const&, int*) -- STLport 4.6 _algobase.h: random-access counted forward copy `for (n = last - first; n > 0; --n) *result++ = *first++`, return the advanced result
// Leaf: ??$__copy@PAVCActivity@CTCHeroOnlineScoreboard@@PAV12@H@std@@YIPAVCActivity@CTCHeroOnlineScoreboard@@PAV12@00ABUrandom_access_iterator_tag@0@PAH@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00566780, 89 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 0xc`.
// Identity: caller 00569d90 std::vector<CTCHeroOnlineScoreboard::CActivity, std::allocator<CTCHeroOnlineScoreboard::CActivity>>::resize; binder ??$__copy@PAVCActivity@CTCHeroOnlineScoreboard@@PAV12@H@std@@YIPAVCActivity@CTCHeroOnlineScoreboard@@PAV12@00ABUrandom_access_iterator_tag@0@PAH@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CTCHeroOnlineScoreboard::CActivity.
#include "stlport/fable_containers.h"
class CCharString {
public:
    CCharString(const CCharString& other);                       // 0x0099EC30 ??0CCharString@@QAE@ABV0@@Z
    ~CCharString();                                              // 0x0099EAE0 ??1CCharString@@QAE@XZ
    const CCharString& operator=(const CCharString& other);      // 0x0099EFB0 ??4CCharString@@QAEABV0@ABV0@@Z
    class CCharStringData* PStringData;                          // +0x00 (Ego_r.pdb, 4 bytes)
};
class CTCHeroOnlineScoreboard {
public:
class CActivity {
public:                              // 16 bytes, flat
    CCharString m_0;                            // +0x00
    CCharString m_4;                            // +0x04
    CCharString m_8;                            // +0x08
    bool        m_flagC;                        // +0x0c
    bool        m_flagD;                        // +0x0d
};
};
template CTCHeroOnlineScoreboard::CActivity* std::__copy<CTCHeroOnlineScoreboard::CActivity*,CTCHeroOnlineScoreboard::CActivity*,int >(CTCHeroOnlineScoreboard::CActivity*, CTCHeroOnlineScoreboard::CActivity*, CTCHeroOnlineScoreboard::CActivity*, std::random_access_iterator_tag const&, int*);
