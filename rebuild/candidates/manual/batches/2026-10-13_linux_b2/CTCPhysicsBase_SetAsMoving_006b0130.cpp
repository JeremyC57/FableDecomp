// CTCPhysicsBase::SetAsMoving(bool)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006B0130, oracle length 32 bytes.
// Grade: candidate (authored from the retail bytes; byte parity NOT yet proven -- run
//        tools/decomp_pipeline/verify_and_land.py on this batch's payload to grade it).
//
// Retail bytes:  push esi ; mov esi,ecx ; mov ecx,[esi+4] ; call CThing::SetAsFrameUpdated ; mov al,[esi+3Ch] ; xor al,[esp+8] ; mov cl,[esi+3Ch] ; and al,1 ; xor cl,al ; mov [esi+3Ch],cl ; pop esi ; ret 4
// engine/CTCPhysicsBase.h: +0x04 parent CThing (retail-only member `sub`), +0x3C bitfield group
// (PDB: OldMoving..., bool:1). Marks the owning thing as updated this frame (0x004C7C40,
// manifest: CThing::SetAsFrameUpdated) and writes bit 0 of the flag byte, which is the live
// "moving" bit; the xor/and/xor sequence is VC7.1's single-bit bitfield store.
#include "engine/CTCPhysicsBase.h"

struct CThingCalls {
    void SetAsFrameUpdated();       // 0x004C7C40
};

struct CPhysicsMovingFlags {
    unsigned char Moving    : 1;    // bit 0 (written here)
    unsigned char OldMoving : 1;    // bit 1
};

struct CTCPhysicsBase_Methods : CTCPhysicsBase {
    void SetAsMoving(bool moving);
};

void CTCPhysicsBase_Methods::SetAsMoving(bool moving)
{
    reinterpret_cast<CThingCalls*>(this->sub)->SetAsFrameUpdated();
    reinterpret_cast<CPhysicsMovingFlags*>(&this->OldMoving[0])->Moving = moving;
}
