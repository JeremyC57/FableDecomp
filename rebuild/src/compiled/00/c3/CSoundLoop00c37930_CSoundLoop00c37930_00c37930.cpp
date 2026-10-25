// CSoundLoop_00c37930::~CSoundLoop_00c37930() -- destroy a looping sound record: drop a voice that has played out, stop a voice still held (sound-system slot 22) and clear the handle, then unregister the record from its owner's loop list (the list object at owner +0x38, slot 8)
// Leaf: ??1CSoundLoop_00c37930@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C37930, 53 bytes (+ int3 padding), __fastcall, plain `ret`.
// Library: libpk audio (no PDB/RTTI); descriptive names. `this` is the landed 0x00C37840 CSoundLoop record
//   {owner +0, voice handle +4 (-1 = none), sound id +8, volume +0xc, reference count +0x10}. Callers: the record's
//   scalar deleting destructor 0x00C2AAB0 (`call ; test [esp+8],1 ; delete`) and its Release 0x00C2A600
//   (`if (--m_refCount == 0) { ~T(); operator delete; }`), so this is the out-of-line destructor. The manifest's
//   "Fastcall_PlayerModeDeactivate" is propagated.
// Callees: DropFinishedVoice 0x00C377F0 (`this` still in ecx), the sound-system accessor 0x00C377E0, the sound system's
//   StopVoice (slot 22, +0x58), the owner's loop list Remove (slot 8, +0x20, on the member object at owner +0x38).
struct CSoundSystem_00c37930 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21();
    virtual void StopVoice(long handle);                // slot 22 (+0x58)
};

struct CSoundLoop_00c37930;

struct CLoopList_00c37930 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07();
    virtual void Remove(CSoundLoop_00c37930* loop);     // slot 8 (+0x20)
};

struct CSoundLoopOwner_00c37930 {
    unsigned char _pad_0x00[0x38];
    CLoopList_00c37930 m_loops;                         // +0x38
};

struct CSoundLoop_00c37930 {
    CSoundLoopOwner_00c37930* m_pOwner;                 // +0x00
    long  m_voice;                                      // +0x04 (-1: not playing)
    long  m_soundID;                                    // +0x08
    float m_volume;                                     // +0x0c
    long  m_refCount;                                   // +0x10
    ~CSoundLoop_00c37930();
    void DropFinishedVoice();                           // 0x00C377F0
    CSoundSystem_00c37930* GetSoundSystem();            // 0x00C377E0
};

CSoundLoop_00c37930::~CSoundLoop_00c37930()
{
    DropFinishedVoice();
    long voice = m_voice;
    if (voice != -1) {
        GetSoundSystem()->StopVoice(voice);
        m_voice = -1;
    }
    m_pOwner->m_loops.Remove(this);
}