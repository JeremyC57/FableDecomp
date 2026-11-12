// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<C3DQuadricBasedMeshOptimiser>) the counted pointer's per-type deleter: `delete (T*)data`, null-guarded, through the object's virtual destructor (vtable slot 0, flag 1)
// Leaf: ?DeleteData@?$CCountedPointer@VC3DQuadricBasedMeshOptimiser@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A28510, 11 bytes: `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]; ret` -- a static __fastcall member (data in ecx);
//   only its address is used: it is stored as the count block's DeleteFunc (`mov [eax+4], 0x00A28510`).
// Names (agentFAM10, 2026-11-12; was _gapscan::sub_00a28510, the behavioural extern "C" DeleteIfSet): FableWin.pdb ?DeleteData@?$CCountedPointer@VC3DQuadricBasedMeshOptimiser@@@@SIXPAX@Z; stored by Reset 0x00A29F50 (reloc_bindings stand-in ?Reset@CMeshOptimiserPointer@@QAEXPAVC3DQuadricBasedMeshOptimiser@@@Z), called at 0x00A2722D with a fresh C3DQuadricBasedMeshOptimiser (ctor 0x00AA41C0, RTTI vtable); Ego_r.pdb CMeshDataBank::MeshOptimiser is CCountedPointer<C3DQuadricBasedMeshOptimiser>.
// Shape: engine/CCountedPointer.h DeleteData (`delete static_cast<T*>(data)`) on a polymorphic T; the explicit instantiation emits the COMDAT.
class C3DQuadricBasedMeshOptimiser {
public:
    virtual ~C3DQuadricBasedMeshOptimiser();                                   // slot 0 (scalar deleting destructor)
};

#include "engine/CCountedPointer.h"

template void FABLE_FASTCALL CCountedPointer<C3DQuadricBasedMeshOptimiser>::DeleteData(void*);
