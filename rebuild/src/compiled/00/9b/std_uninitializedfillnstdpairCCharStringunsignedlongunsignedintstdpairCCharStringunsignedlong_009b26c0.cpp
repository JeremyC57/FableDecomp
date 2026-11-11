// std::__uninitialized_fill_n(std::pair<CCharString,unsigned long>* first, unsigned int n, std::pair<CCharString,unsigned long> const& x, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first, return the end
// Leaf: ??$__uninitialized_fill_n@PAU?$pair@VCCharString@@K@std@@IU12@@std@@YIPAU?$pair@VCCharString@@K@0@PAU10@IABU10@ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x009B26C0, 48 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 009b71b0 std::vector<std::pair<CCharString, unsigned long>, std::allocator<std::pair<CCharString, unsigned long>>>::_M_insert_overflow; callee 0099ec30 CCharString::CCharString; binder ??$__uninitialized_fill_n@PAU?$pair@VCCharString@@K@std@@IU12@@std@@YIPAU?$pair@VCCharString@@K@0@PAU10@IABU10@ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over std::pair<CCharString,unsigned long>.
#include "stlport/fable_containers.h"
class CCharString {
public:
    CCharString(const CCharString& other);                       // 0x0099EC30 ??0CCharString@@QAE@ABV0@@Z
    ~CCharString();                                              // 0x0099EAE0 ??1CCharString@@QAE@XZ
    const CCharString& operator=(const CCharString& other);      // 0x0099EFB0 ??4CCharString@@QAEABV0@ABV0@@Z
    class CCharStringData* PStringData;                          // +0x00 (Ego_r.pdb, 4 bytes)
};

template std::pair<CCharString,unsigned long>* std::__uninitialized_fill_n<std::pair<CCharString,unsigned long>*,unsigned int,std::pair<CCharString,unsigned long> >(std::pair<CCharString,unsigned long>*, unsigned int, std::pair<CCharString,unsigned long> const&, std::__false_type const&);
