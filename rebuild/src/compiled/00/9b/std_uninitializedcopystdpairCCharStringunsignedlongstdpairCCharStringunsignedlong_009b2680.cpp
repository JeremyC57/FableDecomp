// std::__uninitialized_copy(std::pair<CCharString,unsigned long>* first, std::pair<CCharString,unsigned long>* last, std::pair<CCharString,unsigned long>* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PAU?$pair@VCCharString@@K@std@@PAU12@@std@@YIPAU?$pair@VCCharString@@K@0@PAU10@00ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x009B2680, 52 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 009b71b0 std::vector<std::pair<CCharString, unsigned long>, std::allocator<std::pair<CCharString, unsigned long>>>::_M_insert_overflow; caller 009b71b0 std::vector<std::pair<CCharString, unsigned long>, std::allocator<std::pair<CCharString, unsigned long>>>::_M_insert_overflow; caller 00ae7340 std::vector<std::pair<CCharString, unsigned long>, std::allocator<std::pair<CCharString, unsigned long>>>::_M_fill_insert; binder ??$__uninitialized_copy@PAU?$pair@VCCharString@@K@std@@PAU12@@std@@YIPAU?$pair@VCCharString@@K@0@PAU10@00ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over std::pair<CCharString,unsigned long>.
#include "stlport/fable_containers.h"
class CCharString {
public:
    CCharString(const CCharString& other);                       // 0x0099EC30 ??0CCharString@@QAE@ABV0@@Z
    ~CCharString();                                              // 0x0099EAE0 ??1CCharString@@QAE@XZ
    const CCharString& operator=(const CCharString& other);      // 0x0099EFB0 ??4CCharString@@QAEABV0@ABV0@@Z
    class CCharStringData* PStringData;                          // +0x00 (Ego_r.pdb, 4 bytes)
};

template std::pair<CCharString,unsigned long>* std::__uninitialized_copy<std::pair<CCharString,unsigned long>*,std::pair<CCharString,unsigned long>* >(std::pair<CCharString,unsigned long>*, std::pair<CCharString,unsigned long>*, std::pair<CCharString,unsigned long>*, std::__false_type const&);
