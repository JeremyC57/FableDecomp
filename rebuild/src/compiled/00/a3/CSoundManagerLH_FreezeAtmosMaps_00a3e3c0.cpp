// CSoundManagerLH::FreezeAtmosMaps() -- freeze or unfreeze the atmos maps
// Name (agentSND, 2026-11-10): vtable slot 50 (+0xC8); spelling from the Fable Anniversary 360 PDB publics (CSoundManagerUN / CASoundManager at the same vtable position) with the TLC slot evidence in SLOTS.tsv; was vfunc_50.
// Leaf: FreezeAtmosMaps
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A3E3C0, 24 bytes, `ret 4`. Vtable 0x0129D2C4 (CSoundManagerLH, rtti-vtables.tsv) slot 50.
// Callees: the system's slot 3 (+0x0c, GetOutput, as in the landed vfunc_2 0x00A3E3E0), the output's slot 2 (+0x08) as a sibling-call `jmp`.
// Layout: the sound system at +0x40 (as the landed CSoundManagerLH::vfunc_2 0x00A3E3E0); the audio layer is not in Ego_r.pdb, so
// method and member names are descriptive.
struct CSoundPlayer {
    virtual void Play(long sound);                     // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual void SetEnabled(bool enabled);             // slot 9 (+0x24)
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
    void FreezeAtmosMaps(long value);
};

void CSoundManagerLH::FreezeAtmosMaps(long value)
{
    if (m_pSoundSystem)
        m_pSoundSystem->GetOutput()->SetValue(value);
}