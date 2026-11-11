// std::__copy(std::pair<std::pair<CCharString,CCharString>,CSoundBankLH*>* first, std::pair<std::pair<CCharString,CCharString>,CSoundBankLH*>* last, std::pair<std::pair<CCharString,CCharString>,CSoundBankLH*>* result, std::random_access_iterator_tag const&, int*) -- STLport 4.6 _algobase.h: random-access counted forward copy `for (n = last - first; n > 0; --n) *result++ = *first++`, return the advanced result
// Leaf: ??$__copy@PAU?$pair@U?$pair@VCCharString@@V1@@std@@PAVCSoundBankLH@@@std@@PAU12@H@std@@YIPAU?$pair@U?$pair@VCCharString@@V1@@std@@PAVCSoundBankLH@@@0@PAU10@00ABUrandom_access_iterator_tag@0@PAH@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A40040, 84 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 0xc`.
// Identity: caller 00a40c10 source (STLvector_erase_00a40c10.cpp) names std::pair<std::pair<CCharString,CCharString>,CSoundBankLH*>; binder ??$__copy@PAU?$pair@U?$pair@VCCharString@@V1@@std@@PAVCSoundBankLH@@@std@@PAU12@H@std@@YIPAU?$pair@U?$pair@VCCharString@@V1@@std@@PAVCSoundBankLH@@@0@PAU10@00ABUrandom_access_iterator_tag@0@PAH@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over std::pair<std::pair<CCharString,CCharString>,CSoundBankLH*>.
#include "stlport/fable_containers.h"
class CSoundBankLH;
class CCharString {
public:
    CCharString(const CCharString& other);                       // 0x0099EC30 ??0CCharString@@QAE@ABV0@@Z
    ~CCharString();                                              // 0x0099EAE0 ??1CCharString@@QAE@XZ
    const CCharString& operator=(const CCharString& other);      // 0x0099EFB0 ??4CCharString@@QAEABV0@ABV0@@Z
    class CCharStringData* PStringData;                          // +0x00 (Ego_r.pdb, 4 bytes)
};
template std::pair<std::pair<CCharString,CCharString>,CSoundBankLH*>* std::__copy<std::pair<std::pair<CCharString,CCharString>,CSoundBankLH*>*,std::pair<std::pair<CCharString,CCharString>,CSoundBankLH*>*,int >(std::pair<std::pair<CCharString,CCharString>,CSoundBankLH*>*, std::pair<std::pair<CCharString,CCharString>,CSoundBankLH*>*, std::pair<std::pair<CCharString,CCharString>,CSoundBankLH*>*, std::random_access_iterator_tag const&, int*);
