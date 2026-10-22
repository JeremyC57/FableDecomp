// CSoundManagerLH::vfunc_53(long sound) -- when the sound system is up, passes the argument to the sound player's slot 0
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A3E330, 23 bytes, `ret 4`. Vtable 0x0129D2C4 (CSoundManagerLH, rtti-vtables.tsv) slot 53.
// Callees: the system's slot 6 (+0x18, GetPlayer), the player's slot 0 as a sibling-call `jmp [edx]` (argument forwarded in place).
// Layout: the sound system at +0x40 (as the landed CSoundManagerLH::vfunc_2 0x00A3E3E0); the audio layer is not in Ego_r.pdb, so
// method and member names are descriptive.
// Shape: the member tested and re-used (`mov eax,[ecx+40h] ; test ; mov ecx,eax`); a named local keeps it in ecx.
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
    void vfunc_53(long sound);
};

void CSoundManagerLH::vfunc_53(long sound)
{
    if (m_pSoundSystem)
        m_pSoundSystem->GetPlayer()->Play(sound);
}