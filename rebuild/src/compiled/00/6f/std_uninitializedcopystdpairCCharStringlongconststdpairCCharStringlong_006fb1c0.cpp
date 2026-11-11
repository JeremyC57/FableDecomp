// std::__uninitialized_copy(std::pair<CCharString,long> const* first, std::pair<CCharString,long> const* last, std::pair<CCharString,long>* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PBU?$pair@VCCharString@@J@std@@PAU12@@std@@YIPAU?$pair@VCCharString@@J@0@PBU10@0PAU10@ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x006FB1C0, 52 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 006fc130 FableVector_006fc130::vector<FableVector_006fc130::pair<FableVector_006fc130::CCharString, int>>::_M_insert_overflow; caller 006fc130 FableVector_006fc130::vector<FableVector_006fc130::pair<FableVector_006fc130::CCharString, int>>::_M_insert_overflow; caller 00a55500 std::vector<pair<CCharString, long> >::reserve; binder spelling ??$__uninitialized_copy@PBV?$pair@VCCharString@@J@std@@PAV12@@std@@YIPAV?$pair@VCCharString@@J@0@PBV10@0PAV10@ABU__false_type@0@@Z names no symbol; the instantiation emits ??$__uninitialized_copy@PBU?$pair@VCCharString@@J@std@@PAU12@@std@@YIPAU?$pair@VCCharString@@J@0@PBU10@0PAU10@ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over std::pair<CCharString,long>.
#include "stlport/fable_containers.h"
class CCharString {
public:
    CCharString(const CCharString& other);                       // 0x0099EC30 ??0CCharString@@QAE@ABV0@@Z
    ~CCharString();                                              // 0x0099EAE0 ??1CCharString@@QAE@XZ
    const CCharString& operator=(const CCharString& other);      // 0x0099EFB0 ??4CCharString@@QAEABV0@ABV0@@Z
    class CCharStringData* PStringData;                          // +0x00 (Ego_r.pdb, 4 bytes)
};

template std::pair<CCharString,long>* std::__uninitialized_copy<std::pair<CCharString,long> const*,std::pair<CCharString,long>* >(std::pair<CCharString,long> const*, std::pair<CCharString,long> const*, std::pair<CCharString,long>*, std::__false_type const&);
