// CTCTrapBase::BulletTimeFrameUpdate() -- bullet-time frame update of a trap: runs the normal FrameUpdate (virtual tail call)
// Leaf: ?BulletTimeFrameUpdate@CTCTrapBase@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007891F2, length 5 bytes: `mov eax,[ecx] ; jmp [eax+24h]`.
// Identity: CTCTrapBase vtable 0x0126B7FC slot 10 (CTCBase.h: slot 10 BulletTimeFrameUpdate, slot 9 FrameUpdate; the jump goes
// through slot 9 = +0x24, CTCTrapBase::FrameUpdate 0x007888B4); Ego_r.pdb and FableWin.pdb both carry
// ?BulletTimeFrameUpdate@CTCTrapBase@@UAEXXZ, and the PDB class lists BulletTimeFrameUpdate right after HasFrameUpdate (slot 8
// 0x007891EF) as the retail bodies are laid out.
// Shape: the virtual call through `this` (not the qualified CTCTrapBase::FrameUpdate) gives the jmp through the vtable; a
// size-optimised TU (the trap TU's small bodies 0x007891EB / 0x007891EF / 0x007891F7 are unaligned, packed back to back).
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCTRAPBASE
#include "engine/ETCInterfaceType.h"
#include "engine/CTCTrapBase.h"

void CTCTrapBase::BulletTimeFrameUpdate()
{
    FrameUpdate();
}