// Behaviour test for 00c37930: ~CSoundLoop -- drops a finished voice, stops a still-held voice, then unregisters from the owner's loop list
#include <cstdio>

static long g_stopped = -2, g_removed = 0, g_finishedAt = 0;

struct CSoundSystem {
    void StopVoice(long handle) { g_stopped = handle; }
};
static CSoundSystem g_system;

struct CSoundLoop;
struct CLoopList {
    CSoundLoop* m_last;
    void Remove(CSoundLoop* loop) { m_last = loop; ++g_removed; }
};
struct CSoundLoopOwner {
    CLoopList m_loops;
};

struct CSoundLoop {
    CSoundLoopOwner* m_pOwner;
    long m_voice;
    long m_soundID;
    float m_volume;
    long m_refCount;
    CSoundSystem* GetSoundSystem() { return &g_system; }
    void DropFinishedVoice() { if (m_voice == g_finishedAt) m_voice = -1; }
    ~CSoundLoop()
    {
        DropFinishedVoice();
        long voice = m_voice;
        if (voice != -1) {
            GetSoundSystem()->StopVoice(voice);
            m_voice = -1;
        }
        m_pOwner->m_loops.Remove(this);
    }
};

static int fail() { std::printf("00c37930_TEST FAIL\n"); return 1; }

int main()
{
    CSoundLoopOwner owner = { { 0 } };
    g_finishedAt = 5;
    {
        CSoundLoop* loop = new CSoundLoop;
        loop->m_pOwner = &owner; loop->m_voice = 3;
        CSoundLoop* raw = loop;
        delete loop;                                        // voice 3 still playing: stopped
        if (g_stopped != 3 || g_removed != 1 || owner.m_loops.m_last != raw) return fail();
    }
    {
        g_stopped = -2;
        CSoundLoop* loop = new CSoundLoop;
        loop->m_pOwner = &owner; loop->m_voice = 5;         // finished: no stop
        delete loop;
        if (g_stopped != -2 || g_removed != 2) return fail();
    }
    std::printf("00c37930_TEST PASS\n");
    return 0;
}