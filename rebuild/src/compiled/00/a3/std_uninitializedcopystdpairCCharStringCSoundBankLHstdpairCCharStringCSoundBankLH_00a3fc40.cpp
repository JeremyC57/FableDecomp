// std::__uninitialized_copy(std::pair<CCharString,CSoundBankLH*>* first, std::pair<CCharString,CSoundBankLH*>* last, std::pair<CCharString,CSoundBankLH*>* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PAU?$pair@VCCharString@@PAVCSoundBankLH@@@std@@PAU12@@std@@YIPAU?$pair@VCCharString@@PAVCSoundBankLH@@@0@PAU10@00ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A3FC40, 52 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 00a41480 std::vector<std::pair<CCharString, CSoundBankLH *>, std::allocator<std::pair<CCharString, CSoundBankLH *>>>::_M_insert_overflow; caller 00a41480 std::vector<std::pair<CCharString, CSoundBankLH *>, std::allocator<std::pair<CCharString, CSoundBankLH *>>>::_M_insert_overflow; binder ??$__uninitialized_copy@PAU?$pair@VCCharString@@PAVCSoundBankLH@@@std@@PAU12@@std@@YIPAU?$pair@VCCharString@@PAVCSoundBankLH@@@0@PAU10@00ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over std::pair<CCharString,CSoundBankLH*>.
#include "stlport/fable_containers.h"
class CCharString {
public:
    CCharString(const CCharString& other);                       // 0x0099EC30 ??0CCharString@@QAE@ABV0@@Z
    ~CCharString();                                              // 0x0099EAE0 ??1CCharString@@QAE@XZ
    const CCharString& operator=(const CCharString& other);      // 0x0099EFB0 ??4CCharString@@QAEABV0@ABV0@@Z
    class CCharStringData* PStringData;                          // +0x00 (Ego_r.pdb, 4 bytes)
};

class CSoundBankLH;
template std::pair<CCharString,CSoundBankLH*>* std::__uninitialized_copy<std::pair<CCharString,CSoundBankLH*>*,std::pair<CCharString,CSoundBankLH*>* >(std::pair<CCharString,CSoundBankLH*>*, std::pair<CCharString,CSoundBankLH*>*, std::pair<CCharString,CSoundBankLH*>*, std::__false_type const&);
