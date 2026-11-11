// std::__uninitialized_fill_n(CCountedPointer<CMeshDataBankEntry>* first, unsigned int n, CCountedPointer<CMeshDataBankEntry> const& x, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first, return the end
// Leaf: ??$__uninitialized_fill_n@PAV?$CCountedPointer@VCMeshDataBankEntry@@@@IV1@@std@@YIPAV?$CCountedPointer@VCMeshDataBankEntry@@@@PAV1@IABV1@ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A29270, 46 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 00a2d4d0 source (stdvector_Minsertoverflow_00a2d4d0.cpp) names CCountedPointer<CMeshDataBankEntry>; binder ??$__uninitialized_fill_n@PAV?$CCountedPointer@VCMeshDataBankEntry@@@@IV1@@std@@YIPAV?$CCountedPointer@VCMeshDataBankEntry@@@@PAV1@IABV1@ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CCountedPointer<CMeshDataBankEntry>.
#include "stlport/fable_containers.h"
#include "engine/CCountedPointer.h"
#include "engine/CMeshDataBankEntry.h"
template CCountedPointer<CMeshDataBankEntry>* std::__uninitialized_fill_n<CCountedPointer<CMeshDataBankEntry>*,unsigned int,CCountedPointer<CMeshDataBankEntry> >(CCountedPointer<CMeshDataBankEntry>*, unsigned int, CCountedPointer<CMeshDataBankEntry> const&, std::__false_type const&);
