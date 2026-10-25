// Behaviour test for 00c377f0: CSoundLoop::DropFinishedVoice -- forgets the voice once the sound system reports it finished (status 0, position >= 1)
#include <cstdio>

struct CSoundSystem {
    long m_status;
    float m_position;
    long m_queried;
    float GetVoicePosition(long handle, long* status) { m_queried = handle; *status = m_status; return m_position; }
};
static CSoundSystem g_system;

struct CSoundLoop {
    void* m_pOwner;
    long m_voice;
    long m_soundID;
    float m_volume;
    long m_refCount;
    CSoundSystem* GetSoundSystem() { return &g_system; }
    void DropFinishedVoice()
    {
        long voice = m_voice;
        if (voice != -1) {
            long status;
            float position = GetSoundSystem()->GetVoicePosition(voice, &status);
            if (status == 0 && position >= 1.0f)
                m_voice = -1;
        }
    }
};

static int fail() { std::printf("00c377f0_TEST FAIL\n"); return 1; }

int main()
{
    CSoundLoop loop = { 0, 7, 1, 1.0f, 1 };
    g_system.m_queried = 0;
    g_system.m_status = 0; g_system.m_position = 0.5f;     // still playing
    loop.DropFinishedVoice();
    if (loop.m_voice != 7 || g_system.m_queried != 7) return fail();
    g_system.m_status = 1; g_system.m_position = 2.0f;     // status error: keep
    loop.DropFinishedVoice();
    if (loop.m_voice != 7) return fail();
    g_system.m_status = 0; g_system.m_position = 1.0f;     // finished
    loop.DropFinishedVoice();
    if (loop.m_voice != -1) return fail();
    g_system.m_queried = 99;                               // no voice: no query
    loop.DropFinishedVoice();
    if (g_system.m_queried != 99) return fail();
    std::printf("00c377f0_TEST PASS\n");
    return 0;
}