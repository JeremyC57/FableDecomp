// std::__destroy_aux(CCountedPointer<CMeshDataBankEntry>* first, CCountedPointer<CMeshDataBankEntry>* last, std::__false_type const&) -- STLport 4.6 _construct.h: run the destructor of every element of [first, last) (the non-trivial-destructor path)
// Leaf: ??$__destroy_aux@PAV?$CCountedPointer@VCMeshDataBankEntry@@@@@std@@YIXPAV?$CCountedPointer@VCMeshDataBankEntry@@@@0ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A2BD20, 76 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 00a2dbc0 std::vector<CCountedPointer<CMeshDataBankEntry>, std::allocator<CCountedPointer<CMeshDataBankEntry>>>::resize; binder ??$__destroy_aux@PAV?$CCountedPointer@VCMeshDataBankEntry@@@@@std@@YIXPAV?$CCountedPointer@VCMeshDataBankEntry@@@@0ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CCountedPointer<CMeshDataBankEntry>.
#include "stlport/fable_containers.h"
#include "engine/CCountedPointer.h"
#include "engine/CMeshDataBankEntry.h"
template void std::__destroy_aux<CCountedPointer<CMeshDataBankEntry>* >(CCountedPointer<CMeshDataBankEntry>*, CCountedPointer<CMeshDataBankEntry>*, std::__false_type const&);
