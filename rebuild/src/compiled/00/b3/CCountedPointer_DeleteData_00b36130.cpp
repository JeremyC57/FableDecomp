// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<CEngineClothRenderer>) the counted pointer's per-type deleter: `delete (T*)data`, null-guarded, through the object's virtual destructor (vtable slot 0, flag 1)
// Leaf: ?DeleteData@?$CCountedPointer@VCEngineClothRenderer@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B36130, 11 bytes: `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]; ret` -- a static __fastcall member (data in ecx);
//   only its address is used: it is stored as the count block's DeleteFunc (`mov [eax+4], 0x00B36130`).
// Names (agentFAM10, 2026-11-12; was _gapscan::sub_00b36130, the behavioural extern "C" DeleteIfSet): FableWin.pdb ?DeleteData@?$CCountedPointer@VCEngineClothRenderer@@@@SIXPAX@Z; stored by Reset 0x00B37DA0, called at 0x00B3463C with a fresh CEngineClothRenderer (ctor 0x00BB5040, RTTI vtable); Ego_r.pdb CEnginePrimitiveRenderer::ClothRenderer (between PrimitiveChangerManager +0x2E8 and ParticleLineGroupManager +0x2F8).
// Shape: engine/CCountedPointer.h DeleteData (`delete static_cast<T*>(data)`) on a polymorphic T; the explicit instantiation emits the COMDAT.
class CEngineClothRenderer {
public:
    virtual ~CEngineClothRenderer();                                   // slot 0 (scalar deleting destructor)
};

#include "engine/CCountedPointer.h"

template void FABLE_FASTCALL CCountedPointer<CEngineClothRenderer>::DeleteData(void*);
