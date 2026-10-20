// Behaviour test for 00c37840: the model below is a copy of src/00c37840.cpp (compiled separately), driven by test doubles.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __stdcall
#define __cdecl
#define __fastcall
#define __forceinline inline
#define __int64 long long
#endif
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
    virtual void v00() {} virtual void v01() {} virtual void v02() {} virtual void v03() {} virtual void v04() {}
    virtual void v05() {} virtual void v06() {} virtual void v07() {} virtual void v08() {} virtual void v09() {}
    virtual void v10() {} virtual void v11() {} virtual void v12() {} virtual void v13() {} virtual void v14() {}
    virtual void v15() {} virtual void v16() {} virtual void v17() {} virtual void v18() {} virtual void v19() {}
    virtual void v20() {} virtual void v21() {}
    virtual void StopVoice(long handle);                // slot 22 (+0x58)
    virtual void v23() {} virtual void v24() {}
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

// ---- test doubles and driver ----
static int g_stops = 0, g_volumes = 0, g_plays = 0, g_drops = 0, g_filter = 0, g_bad = 0, g_lastStopped = 0;
static float g_lastVolume = 0.0f, g_playVolume = 0.0f;
static long g_playSound = 0;
static bool g_finished = false;
CMessageListOwner_00c203a0::CMessageListOwner_00c203a0() : m_state(0), m_index(0), m_count(0) { m_messages.head = 0; }
CNavigationLayer::CNavigationLayer() : m_pFirst(0), m_pSecond(0), m_pThird(0), m_flag(0) {}
void CSoundPlayParams_00c37840::SetFilter_00c20440(long filter) { g_filter = (int)filter; m_filter.m_state |= 0x20; }
void CSoundSystem_00c37840::StopVoice(long handle) { ++g_stops; g_lastStopped = (int)handle; }
void CSoundSystem_00c37840::SetVoiceVolume(long handle, float volume) { ++g_volumes; g_lastVolume = volume; if (handle != 7) g_bad = 1; }
long CSoundSystem_00c37840::Play_00c13140(long soundID, long flags, const CSoundPlayParams_00c37840* p)
{
    ++g_plays; g_playSound = soundID; g_playVolume = p->m_volume;
    if (flags != 0 || p->m_loopCount != -1 || p->m_priority != 0 || p->m_pitch != 1.0f || p->m_delay != 0) g_bad = 1;
    if (p->m_flags != 0 || p->m_group != 0 || p->m_owner != 0 || p->m_user != 0 || (p->m_filter.m_state & 0x20) == 0) g_bad = 1;
    if (p->m_links.m_pFirst != 0 || p->m_links.m_flag != 0) g_bad = 1;
    return 7;
}
static CSoundSystem_00c37840 g_system;
void CSoundLoop_00c37840::DropFinishedVoice_00c377f0() { ++g_drops; if (g_finished) m_voice = -1; }
CSoundSystem_00c37840* CSoundLoop_00c37840::GetSoundSystem_00c377e0() { return &g_system; }

int main()
{
    bool ok = true;
    CSoundLoop_00c37840 loop;
    loop.m_pOwner = 0; loop.m_voice = -1; loop.m_soundID = 1234; loop.m_volume = 0.5f;

    loop.Update();                                    // silent + audible volume: start the voice
    ok = ok && g_plays == 1 && loop.m_voice == 7 && g_playSound == 1234 && g_playVolume == 0.5f && g_filter == -1;
    loop.m_volume = 0.25f;
    loop.Update();                                    // playing: new volume pushed
    ok = ok && g_volumes == 1 && g_lastVolume == 0.25f && g_plays == 1;
    loop.m_volume = 0.0f;
    loop.Update();                                    // zero volume: stop it
    ok = ok && g_stops == 1 && g_lastStopped == 7 && loop.m_voice == -1;
    loop.Update();                                    // already stopped: nothing
    ok = ok && g_stops == 1 && g_plays == 1 && g_volumes == 1;
    loop.m_volume = 1.0f; loop.m_voice = 7; g_finished = true;
    loop.Update();                                    // the voice finished: restarted
    ok = ok && g_plays == 2 && loop.m_voice == 7 && g_drops == 5 && !g_bad;

    if (!ok) { std::printf("00c37840_TEST FAIL\n"); return 1; }
    std::printf("00c37840_TEST PASS\n");
    return 0;
}