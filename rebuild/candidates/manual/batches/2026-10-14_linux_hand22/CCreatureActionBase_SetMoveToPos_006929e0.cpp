// CCreatureActionBase::SetMoveToPos(const C3DVector& pos) -- record the next movement position and
// the world frame it belongs to
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006929E0, length 54 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (on PCreature at +0x08),
// a no-argument member @ 0x004C7AB0 on the creature whose result is not used (it reads a global's
// +0x24; the manifest "CViewBase::GetDisplayEngine" label is propagated), and the world-frame
// getter @ 0x0049D870 (returns the global @ 0x013B89BC; its "GFGetPreMainMemoryUsed" label is
// propagated) stored into NextMovementPosWorldFrame at +0x64. The position is then copied into
// NextMovementPos at +0x68 as three dwords, the member address formed in place (`add esi,68h`).
// engine/CCreatureActionBase.h (PDB) agrees: PCreature +0x08, NextMovementPosWorldFrame +0x64,
// NextMovementPos +0x68; a local model is used because the method takes a stack argument.
struct C3DVector {
    float X;
    float Y;
    float Z;
};

struct CThingCreatureBase_Calls {
    void* Call_004c7ab0();                     // 0x004C7AB0 (result unused)
};

struct CBaseIntelligentPointer_Calls {         // CIntelligentPointer<CThingCreatureBase>, 8 bytes
    unsigned char _body[8];
    CThingCreatureBase_Calls* GetPItem();      // 0x00A01B50
};

long GetWorldFrame_0049d870();                 // 0x0049D870 (returns the global @ 0x013B89BC)

struct CCreatureActionBase {
    virtual void v00();
    unsigned char _pad04[4];                   // +0x04
    CBaseIntelligentPointer_Calls PCreature;   // +0x08
    unsigned char _pad10[0x54];                // +0x10..+0x64
    long      NextMovementPosWorldFrame;       // +0x64
    C3DVector NextMovementPos;                 // +0x68
    void SetMoveToPos(const C3DVector& pos);
};

void CCreatureActionBase::SetMoveToPos(const C3DVector& pos)
{
    PCreature.GetPItem()->Call_004c7ab0();
    NextMovementPosWorldFrame = GetWorldFrame_0049d870();
    NextMovementPos = pos;
}
