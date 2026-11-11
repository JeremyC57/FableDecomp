// std::fill(NGameText::CDataBankEntry** first, NGameText::CDataBankEntry** last, NGameText::CDataBankEntry* const& x) -- STLport 4.6 _algobase.h: assign x to every element of [first, last)
// Leaf: ??$fill@PAPAVCDataBankEntry@NGameText@@PAV12@@std@@YIXPAPAVCDataBankEntry@NGameText@@0ABQAV12@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x009C9C60, 31 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 009caa30 std::vector<NGameText::CDataBankEntry *, std::allocator<NGameText::CDataBankEntry *>>::_M_fill_insert; caller 009caa30 std::vector<NGameText::CDataBankEntry *, std::allocator<NGameText::CDataBankEntry *>>::_M_fill_insert; binder ??$fill@PAPAVCDataBankEntry@NGameText@@PAV12@@std@@YIXPAPAVCDataBankEntry@NGameText@@0ABQAV12@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over NGameText::CDataBankEntry*.
#include "stlport/fable_containers.h"
namespace NGameText {
class CDataBankEntry;
}
template void std::fill<NGameText::CDataBankEntry**,NGameText::CDataBankEntry* >(NGameText::CDataBankEntry**, NGameText::CDataBankEntry**, NGameText::CDataBankEntry* const&);
