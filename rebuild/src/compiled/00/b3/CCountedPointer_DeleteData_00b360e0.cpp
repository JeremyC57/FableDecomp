// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<CEnginePrimitiveManagerLines>) the counted pointer's per-type deleter: `delete (T*)data`, null-guarded, through the object's virtual destructor (vtable slot 0, flag 1)
// Leaf: ?DeleteData@?$CCountedPointer@VCEnginePrimitiveManagerLines@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B360E0, 11 bytes: `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]; ret` -- a static __fastcall member (data in ecx);
//   only its address is used: it is stored as the count block's DeleteFunc (`mov [eax+4], 0x00B360E0`).
// Names (agentFAM10, 2026-11-12; was _gapscan::sub_00b360e0, the behavioural extern "C" DeleteIfSet): FableWin.pdb ?DeleteData@?$CCountedPointer@VCEnginePrimitiveManagerLines@@@@SIXPAX@Z; stored by Reset 0x00B379F0, called at 0x00B33F30 on `this+0x2C0` with a fresh CEnginePrimitiveManagerLines (ctor 0x00BC7E90, RTTI vtable); Ego_r.pdb CEnginePrimitiveRenderer::LineManager (9th counted manager member).
// Shape: engine/CCountedPointer.h DeleteData (`delete static_cast<T*>(data)`) on a polymorphic T; the explicit instantiation emits the COMDAT.
class CEnginePrimitiveManagerLines {
public:
    virtual ~CEnginePrimitiveManagerLines();                                   // slot 0 (scalar deleting destructor)
};

#include "engine/CCountedPointer.h"

template void FABLE_FASTCALL CCountedPointer<CEnginePrimitiveManagerLines>::DeleteData(void*);
