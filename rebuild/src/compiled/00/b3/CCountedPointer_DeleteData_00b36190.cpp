// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<CEnginePrimitiveManagerDecal>) the counted pointer's per-type deleter: `delete (T*)data`, null-guarded, through the object's virtual destructor (vtable slot 0, flag 1)
// Leaf: ?DeleteData@?$CCountedPointer@VCEnginePrimitiveManagerDecal@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B36190, 11 bytes: `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]; ret` -- a static __fastcall member (data in ecx);
//   only its address is used: it is stored as the count block's DeleteFunc (`mov [eax+4], 0x00B36190`).
// Names (agentFAM10, 2026-11-12; was _gapscan::sub_00b36190, the behavioural extern "C" DeleteIfSet): FableWin.pdb ?DeleteData@?$CCountedPointer@VCEnginePrimitiveManagerDecal@@@@SIXPAX@Z; stored by Reset 0x00B381C0, called at 0x00B340C8 on `this+0x320` with a fresh CEnginePrimitiveManagerDecal (ctor 0x00BBA480, RTTI vtable); Ego_r.pdb CEnginePrimitiveRenderer::DecalManager.
// Shape: engine/CCountedPointer.h DeleteData (`delete static_cast<T*>(data)`) on a polymorphic T; the explicit instantiation emits the COMDAT.
class CEnginePrimitiveManagerDecal {
public:
    virtual ~CEnginePrimitiveManagerDecal();                                   // slot 0 (scalar deleting destructor)
};

#include "engine/CCountedPointer.h"

template void FABLE_FASTCALL CCountedPointer<CEnginePrimitiveManagerDecal>::DeleteData(void*);
