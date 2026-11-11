// std::fill(CMeshDataBankEntry** first, CMeshDataBankEntry** last, CMeshDataBankEntry* const& x) -- STLport 4.6 _algobase.h: assign x to every element of [first, last)
// Leaf: ??$fill@PAPAVCMeshDataBankEntry@@PAV1@@std@@YIXPAPAVCMeshDataBankEntry@@0ABQAV1@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A291A0, 31 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 00a2c0f0 source (stdvector_Mfillinsert_00a2c0f0.cpp) names CMeshDataBankEntry; binder ??$fill@PAPAVCMeshDataBankEntry@@PAV1@@std@@YIXPAPAVCMeshDataBankEntry@@0ABQAV1@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CMeshDataBankEntry*.
#include "stlport/fable_containers.h"
class CMeshDataBankEntry;
template void std::fill<CMeshDataBankEntry**,CMeshDataBankEntry* >(CMeshDataBankEntry**, CMeshDataBankEntry**, CMeshDataBankEntry* const&);
