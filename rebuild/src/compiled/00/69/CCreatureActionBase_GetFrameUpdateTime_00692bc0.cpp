// CCreatureActionBase::GetFrameUpdateTime() const -- the creature's animation speed multiplier times one frame (1 / frame rate)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00692BC0, length 36 bytes. Vtable slot 62 of the CCreatureActionBase family
// (rtti-vtables.tsv lists it through CCreatureAction_StartSneaking, 0x01258DFC).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8), CThingCreatureBase::GetAnimationSpeedMultiplier
// @ 0x006655A0. Globals: the long frame rate @ 0x01375550 (`fild`), the float 1.0f @ 0x0122DED8 (`fdivr`: 1.0f / rate).
// Shape: the frame time is a named float local (`fstp [esp]` into the one-dword frame before the call -- a named float
// local for a value that must survive a call, GOTCHAS), then the call result is multiplied by it (`fmul [esp]`).
struct CThingCreatureBase_Calls {         // engine/CThingCreatureBase.h exists but is not used: only this call is modelled
    float GetAnimationSpeedMultiplier() const;   // 0x006655A0
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThingCreatureBase_Calls* m_pItem;      // +0x04
    CThingCreatureBase_Calls* GetPItem() const;   // 0x00A01B50
};

extern long g_frameRate;                    // 0x01375550: frames per second (integer)

// engine/CCreatureActionBase.h exists but is not used: it is the PDB build's flat pack(1) layout (0xb4 bytes, three
// containers 4 bytes wider than retail's 0xa8, no vtable or methods); the retail `this` is modelled locally.
struct CCreatureActionBase {
    virtual void v00();
    unsigned char _pad04[4];                // +0x04
    CBaseIntelligentPointer PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    float GetFrameUpdateTime() const;
};

float CCreatureActionBase::GetFrameUpdateTime() const
{
    float frameTime = 1.0f / (float)g_frameRate;
    return PCreature.GetPItem()->GetAnimationSpeedMultiplier() * frameTime;
}