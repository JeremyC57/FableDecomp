// CSoundManagerLH::vfunc_8() -- true when the sound system is up and the sound player's slot 10 returns zero
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A3E380, 30 bytes. Vtable 0x0129D2C4 (CSoundManagerLH, rtti-vtables.tsv) slot 8.
// Callees: the system's slot 6 (+0x18, GetPlayer), the player's slot 10 (+0x28).
// Shape: `== 0` on the long result is `neg ; sbb ; inc`; no system is the separate `xor al,al` exit.
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
    bool vfunc_8();
};

bool CSoundManagerLH::vfunc_8()
{
    if (m_pSoundSystem)
        return m_pSoundSystem->GetPlayer()->GetActiveCount() == 0;
    return false;
}
