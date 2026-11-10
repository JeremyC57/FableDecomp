// CSoundManagerLH::SetEnableAtmos() -- switch the atmos layer: pass the negated flag to the sound system output object
// Name (agentSND, 2026-11-10): vtable slot 2 (+0x8); spelling from the Fable Anniversary 360 PDB publics (CSoundManagerUN / CASoundManager at the same vtable position) with the TLC slot evidence in SLOTS.tsv; was vfunc_2.
// Leaf: SetEnableAtmos
// Retail: Fable.exe 0x00A3E3E0, 54 bytes. Vtable 0x0129D2C4 (CSoundManagerLH, rtti-vtables.tsv) slot 2:
// the manifest label "CThingPhysical::GetNoNavCollisionLineLists" is a propagated name (wrong class).
// Callees: slot 3 (+0x0c) of the member object at +0x40 (called twice), slot 1 (+4) of its result.
// Shape: the member is re-read for the second slot-3 call (no local); the final virtual call is a
// sibling-call `jmp` with `!flag` stored over the argument slot (a bool argument: `sete cl`, whole ecx stored).
struct CSoundOutput {
    virtual void v000();
    virtual void SetEnabled(bool enabled);               // slot 1 (+4)
};

struct CSoundSystem {
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual CSoundOutput* GetOutput();                   // slot 3 (+0x0c)
};

struct CSoundManagerLH {                                 // vtable 0x0129D2C4
    virtual void v000();
    virtual void v001();
    virtual void SetEnableAtmos(bool disabled);
    unsigned char _pad04[0x3c];
    CSoundSystem* m_pSoundSystem;                        // +0x40
};

void CSoundManagerLH::SetEnableAtmos(bool disabled)
{
    if (m_pSoundSystem && m_pSoundSystem->GetOutput())
        m_pSoundSystem->GetOutput()->SetEnabled(!disabled);
}