// CSoundManagerLH::SetOutputConfigurationToHeadphones() -- tell the sound player whether the output is headphones
// Name (agentSND, 2026-11-10): vtable slot 7 (+0x1C); spelling from the Fable Anniversary 360 PDB publics (CSoundManagerUN / CASoundManager at the same vtable position) with the TLC slot evidence in SLOTS.tsv; was vfunc_7.
// Leaf: SetOutputConfigurationToHeadphones
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A3E350, 41 bytes, `ret 4`. Vtable 0x0129D2C4 (CSoundManagerLH, rtti-vtables.tsv) slot 7.
// Callees: the system's slot 6 (+0x18, GetPlayer), the player's slot 9 (+0x24) as a sibling-call `jmp` with `!disabled` stored over
// the argument slot. The player slot takes an int (BOOL): `xor ecx,ecx ; test bl,bl ; sete cl` (the bool argument parks in ebx so ecx
// can be zeroed first); a bool parameter gives the SetEnableAtmos 0x00A3E3E0 shape (`sete cl` without the xor).
// Layout: the sound system at +0x40 (as the landed CSoundManagerLH::SetEnableAtmos 0x00A3E3E0); the audio layer is not in Ego_r.pdb, so
// method and member names are descriptive.
struct CSoundPlayer {
    virtual void Play(long sound);                     // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual void SetEnabled(int enabled);              // slot 9 (+0x24): an int flag (BOOL)
    virtual long GetActiveCount();                     // slot 10 (+0x28)
};

struct CSoundOutput {
    virtual void v00(); virtual void v01();
    virtual void SetValue(long value);                 // slot 2 (+0x08)
};

struct CSoundSystem {
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual CSoundOutput* GetOutput();                 // slot 3 (+0x0c)
    virtual void v04(); virtual void v05();
    virtual CSoundPlayer* GetPlayer();                 // slot 6 (+0x18)
};

struct CSoundManagerLH {                               // vtable 0x0129D2C4
    virtual void v000();
    unsigned char _pad04[0x3c];
    CSoundSystem* m_pSoundSystem;                      // +0x40
    void SetOutputConfigurationToHeadphones(bool disabled);
};

void CSoundManagerLH::SetOutputConfigurationToHeadphones(bool disabled)
{
    if (m_pSoundSystem)
        m_pSoundSystem->GetPlayer()->SetEnabled(!disabled);
}