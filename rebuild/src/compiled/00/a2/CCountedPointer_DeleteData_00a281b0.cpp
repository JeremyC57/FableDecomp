// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<C3DAnimationManager>) the counted pointer's per-type deleter: `delete (T*)data`, null-guarded, through the object's virtual destructor (vtable slot 0, flag 1)
// Leaf: ?DeleteData@?$CCountedPointer@VC3DAnimationManager@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A281B0, 11 bytes: `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]; ret` -- a static __fastcall member (data in ecx);
//   only its address is used: it is stored as the count block's DeleteFunc (`mov [eax+4], 0x00A281B0`).
// Names (agentFAM10, 2026-11-12; was _gapscan::sub_00a281b0, the behavioural extern "C" DeleteIfSet): FableWin.pdb ?DeleteData@?$CCountedPointer@VC3DAnimationManager@@@@SIXPAX@Z; stored by Reset 0x00A29800 (reloc_bindings stand-in ?Reset@CAnimationManagerPointer@@QAEXPAVC3DAnimationManager@@@Z), called at 0x00A27308 with a fresh C3DAnimationManager (ctor 0x00AA0F60 stores its RTTI vtable); Ego_r.pdb CMeshDataBank::AnimationManager is CCountedPointer<C3DAnimationManager>.
// Shape: engine/CCountedPointer.h DeleteData (`delete static_cast<T*>(data)`) on a polymorphic T; the explicit instantiation emits the COMDAT.
class C3DAnimationManager {
public:
    virtual ~C3DAnimationManager();                                   // slot 0 (scalar deleting destructor)
};

#include "engine/CCountedPointer.h"

template void FABLE_FASTCALL CCountedPointer<C3DAnimationManager>::DeleteData(void*);
