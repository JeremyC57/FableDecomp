// std::__uninitialized_copy(CWideString* first, CWideString* last, CWideString* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PAVCWideString@@PAV1@@std@@YIPAVCWideString@@PAV1@00ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x0040DD60, 46 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 00412330 std::vector<CWideString,std::allocator<CWideString> >::_M_insert_overflow; caller 004126e0 std::vector<CWideString,std::allocator<CWideString> >::_M_fill_insert; caller 004126e0 std::vector<CWideString,std::allocator<CWideString> >::_M_fill_insert; binder ??$__uninitialized_copy@PAVCWideString@@PAV1@@std@@YIPAVCWideString@@PAV1@00ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CWideString.
#include "stlport/fable_containers.h"
class CWideString {
public:
    CWideString(const CWideString& other);                       // 0x0099B720 ??0CWideString@@QAE@ABV0@@Z
    ~CWideString();                                              // 0x0099B510 ??1CWideString@@QAE@XZ
    class CWideStringData* PStringData;                          // +0x00 (Ego_r.pdb, 4 bytes)
};

template CWideString* std::__uninitialized_copy<CWideString*,CWideString* >(CWideString*, CWideString*, CWideString*, std::__false_type const&);
