// CSoundLoop_00c37840::Update() -- per-frame upkeep of a looping sound: drop a finished voice, stop the voice when the volume is zero, start it (with default play parameters and the current volume) when silent, else push the new volume to the playing voice
// Leaf: Update@CSoundLoop_00c37840
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C37840, length 232 bytes, plain `ret`.
// Identity: library code in the 0x00C3xxxx module; the catalog label "CPlayer::CPlayer::OnModeActivityUpdate" is
// propagated (no CPlayer here: `this` is a 16-byte {owner, voice handle, sound id, volume} record). Single caller
// 0x00C2A464. Class and member names are descriptive.
// Callees: the finished-voice check 0x00C377F0 and the sound-system accessor 0x00C377E0 (`[this]->[0]->[4]->[38h]`)
// of the same record; the sound system's virtual slots 22 (+0x58, stop a voice) and 25 (+0x64, set a voice's
// volume) and its non-virtual play request 0x00C13140 (sound id, 0, &params; landed as
// "CTCHeroExperience_00c13140::GetBlockParryReward"); the play-parameter block's member constructors 0x00C203A0 (landed
// as CMessageListOwner_00c203a0) and 0x00C20390 (landed as CNavigationLayer), and its filter setter
// 0x00C20440 (landed as the free "bind2nd_greater"). Float 0.0f @ 0x0122DEDC.
// Shape: the voice handle is read into edi before the volume compare (`fld [0.0f] ; fld [esi+0Ch] ; fucompp ; test
// ah,44h ; jp`); the 0x50-byte play parameters are a local whose inline constructor runs the two member constructors
// with the scalar initialisers in declaration order around them; the caller's own assignments follow.
struct CMessageEventList {
    void* head;
};

struct CMessageListOwner_00c203a0 {                     // 12 bytes
    long  m_state;                                      // +0x00 (flag word, |= 0x20 by the filter setter)
    short m_index;                                      // +0x04
    short m_count;                                      // +0x06
    CMessageEventList m_messages;                       // +0x08
    CMessageListOwner_00c203a0();                       // 0x00C203A0
};

struct CNavigationLayer {                               // 16 bytes
    void* m_pFirst;                                     // +0x00
    void* m_pSecond;                                    // +0x04
    void* m_pThird;                                     // +0x08
    unsigned char m_flag;                               // +0x0c
    CNavigationLayer();                                 // 0x00C20390
};

struct CSoundPlayParams_00c37840 {                      // 0x50 bytes
    CMessageListOwner_00c203a0 m_filter;                // +0x00
    unsigned char _unset00c[0x10];                      // +0x0c .. +0x1c (left unset here)
    long  m_loopCount;                                  // +0x1c
    long  m_delay;                                      // +0x20
    float m_volume;                                     // +0x24
    float m_pitch;                                      // +0x28
    long  m_priority;                                   // +0x2c
    CNavigationLayer m_links;                           // +0x30
    long  m_flags;                                      // +0x40
    short m_group;                                      // +0x44
    long  m_owner;                                      // +0x48
    long  m_user;                                       // +0x4c
    CSoundPlayParams_00c37840()
        : m_loopCount(0), m_delay(0), m_volume(1.0f), m_pitch(1.0f), m_priority(0),
          m_flags(0), m_group(0), m_owner(0), m_user(0) {}
    void SetFilter_00c20440(long filter);               // 0x00C20440 (thiscall: ecx + one stack argument)
};

struct CSoundSystem_00c37840 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21();
    virtual void StopVoice(long handle);                // slot 22 (+0x58)
    virtual void v23(); virtual void v24();
    virtual void SetVoiceVolume(long handle, float volume);         // slot 25 (+0x64)
    long Play_00c13140(long soundID, long flags, const CSoundPlayParams_00c37840* params);   // 0x00C13140
};

struct CSoundLoop_00c37840 {
    void* m_pOwner;                                     // +0x00
    long  m_voice;                                      // +0x04 (-1: not playing)
    long  m_soundID;                                    // +0x08
    float m_volume;                                     // +0x0c
    void DropFinishedVoice_00c377f0();                  // 0x00C377F0
    CSoundSystem_00c37840* GetSoundSystem_00c377e0();   // 0x00C377E0
    void Update();
};

void CSoundLoop_00c37840::Update()
{
    DropFinishedVoice_00c377f0();
    long voice = m_voice;
    if (m_volume == 0.0f) {
        if (voice != -1) {
            GetSoundSystem_00c377e0()->StopVoice(voice);
            m_voice = -1;
        }
        return;
    }
    if (voice == -1) {
        CSoundPlayParams_00c37840 params;
        params.m_loopCount = -1;
        params.m_priority = 0;
        params.SetFilter_00c20440(-1);
        params.m_volume = m_volume;
        m_voice = GetSoundSystem_00c377e0()->Play_00c13140(m_soundID, 0, &params);
    } else {
        GetSoundSystem_00c377e0()->SetVoiceVolume(voice, m_volume);
    }
}