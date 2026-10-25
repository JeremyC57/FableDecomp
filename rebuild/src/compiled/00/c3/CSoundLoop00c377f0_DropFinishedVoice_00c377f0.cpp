// CSoundLoop_00c377f0::DropFinishedVoice() -- forget a looping sound's voice once it has played out: when a voice is held, ask the sound system for its play position (slot 28, with a status word out); a zero status and a position at or past 1.0 clear the handle to -1
// Leaf: ?DropFinishedVoice@CSoundLoop_00c377f0@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C377F0, 69 bytes (+ int3 padding), __fastcall, plain `ret`.
// Library: libpk audio (no PDB/RTTI); descriptive names. `this` is the landed 0x00C37840 CSoundLoop record
//   {owner +0, voice handle +4 (-1 = none), sound id +8, volume +0xc, reference count +0x10}; Update 0x00C37840 and the
//   destructor 0x00C37930 call this first. The manifest's "Fastcall_PlayerActivityCheck" is propagated.
// Callees: the record's sound-system accessor 0x00C377E0 (`[this]->[0]->[4]->[38h]`), the sound system's virtual slot 28
//   (+0x70: float position of a voice, status word through the pointer). Float 1.0f @ 0x0122DED8.
// Shape: the handle is read once into edi; the status word is the `push ecx` frame word; the float result stays on the
//   x87 stack across the status test (`fstp st(0)` on the non-zero arm), `>= 1.0f` is `fcomp ; test ah,1 ; jne`.
struct CSoundSystem_00c377f0 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21();
    virtual void StopVoice(long handle);                            // slot 22 (+0x58)
    virtual void v23(); virtual void v24();
    virtual void SetVoiceVolume(long handle, float volume);         // slot 25 (+0x64)
    virtual void v26(); virtual void v27();
    virtual float GetVoicePosition(long handle, long* status);      // slot 28 (+0x70)
};

struct CSoundLoop_00c377f0 {
    void* m_pOwner;                                     // +0x00
    long  m_voice;                                      // +0x04 (-1: not playing)
    long  m_soundID;                                    // +0x08
    float m_volume;                                     // +0x0c
    long  m_refCount;                                   // +0x10
    CSoundSystem_00c377f0* GetSoundSystem();            // 0x00C377E0
    void DropFinishedVoice();
};

void CSoundLoop_00c377f0::DropFinishedVoice()
{
    long voice = m_voice;
    if (voice != -1) {
        long status;
        float position = GetSoundSystem()->GetVoicePosition(voice, &status);
        if (status == 0 && position >= 1.0f)
            m_voice = -1;
    }
}