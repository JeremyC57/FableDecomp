// CSoundManagerLH::vfunc_7(bool disabled) -- when the sound system is up, passes the NEGATED flag to the sound player's slot 9
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A3E350, 41 bytes, `ret 4`. Vtable 0x0129D2C4 (CSoundManagerLH, rtti-vtables.tsv) slot 7.
// Callees: the system's slot 6 (+0x18, GetPlayer), the player's slot 9 (+0x24) as a sibling-call `jmp` with `!disabled` stored over
// the argument slot. The player slot takes an int (BOOL): `xor ecx,ecx ; test bl,bl ; sete cl` (the bool argument parks in ebx so ecx
// can be zeroed first); a bool parameter gives the vfunc_2 0x00A3E3E0 shape (`sete cl` without the xor).
// Layout: the sound system at +0x40 (as the landed CSoundManagerLH::vfunc_2 0x00A3E3E0); the audio layer is not in Ego_r.pdb, so
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
    void vfunc_7(bool disabled);
};

void CSoundManagerLH::vfunc_7(bool disabled)
{
    if (m_pSoundSystem)
        m_pSoundSystem->GetPlayer()->SetEnabled(!disabled);
}