// std::__uninitialized_copy(NGameText::CDataBankTextEntry* first, NGameText::CDataBankTextEntry* last, NGameText::CDataBankTextEntry* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PAVCDataBankTextEntry@NGameText@@PAV12@@std@@YIPAVCDataBankTextEntry@NGameText@@PAV12@00ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A43630, 51 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 00a43b20 source (STLvector_Minsertoverflow_00a43b20.cpp) names NGameText::CDataBankTextEntry; binder ??$__uninitialized_copy@PAVCDataBankTextEntry@NGameText@@PAV12@@std@@YIPAVCDataBankTextEntry@NGameText@@PAV12@00ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over NGameText::CDataBankTextEntry.
#include "stlport/fable_containers.h"
struct CTextEntry {                             // NGameText::CTextEntry (intrusively counted)
    void* __vftable;                            // +0x00
    long  RefCount;                             // +0x04
};

struct CIVCountedPointer_CTextEntry {           // CIVCountedPointer<NGameText::CTextEntry>
    CTextEntry* P;
    CIVCountedPointer_CTextEntry(const CIVCountedPointer_CTextEntry& x) : P(x.P) { if (P) ++P->RefCount; }
};
namespace NGameText {
class CDataBankTextEntry {
public:                     // NGameText::CDataBankTextEntry, 8 bytes
    CIVCountedPointer_CTextEntry PText;         // +0x00
    long                         NarratorIndex; // +0x04
};
}
template NGameText::CDataBankTextEntry* std::__uninitialized_copy<NGameText::CDataBankTextEntry*,NGameText::CDataBankTextEntry* >(NGameText::CDataBankTextEntry*, NGameText::CDataBankTextEntry*, NGameText::CDataBankTextEntry*, std::__false_type const&);
