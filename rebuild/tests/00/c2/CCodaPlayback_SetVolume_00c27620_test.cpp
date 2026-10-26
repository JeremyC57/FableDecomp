// behaviour test for 0x00C27620 CCodaPlayback::SetVolume: non-3D volume scaled by the panel's master volume, under the lock
#include <cstdio>

static int g_locks = 0, g_unlocks = 0, g_depth = 0, g_lockedCalls = 0;
struct CScopedLock {
    void* m_pSection; unsigned long m_tick;
    CScopedLock(void* section) : m_pSection(section), m_tick(0) { ++g_locks; ++g_depth; }
    ~CScopedLock() { ++g_unlocks; --g_depth; }
};
unsigned char g_soundLock[0x18];

class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector SwapYZ() const { C3DVector v(X, Z, Y); return v; }
};

struct CODASource { float volume; C3DVector position; unsigned long playPosition; int stops; };
void CODA_SetSourceVolume(CODASource* s, float v) { if (g_depth) ++g_lockedCalls; s->volume = v; }
void CODA_SetSourcePosition(CODASource* s, const C3DVector* p) { if (g_depth) ++g_lockedCalls; s->position = *p; }
unsigned long CODA_GetSourcePosition(CODASource* s) { if (g_depth) ++g_lockedCalls; return s->playPosition; }
long CODA_StopSource(CODASource* s, bool) { if (g_depth) ++g_lockedCalls; ++s->stops; return 1; }

struct CPanel { float m_masterVolume; };

struct CCodaPlayback {
    CODASource*   m_pSource;
    int           m_loopsLeft;
    unsigned int  m_loopPoint;
    int           m_bArmed;
    bool          m_bLooping, m_b3D, m_bStarted;
    void SetVolume(CPanel* panel, float volume);
};

void CCodaPlayback::SetVolume(CPanel* panel, float volume)
{
    CScopedLock lock(g_soundLock);
    if (!m_b3D)
        volume *= panel->m_masterVolume;
    CODA_SetSourceVolume(m_pSource, volume);
}

int main()
{
    CODASource s = { 0.0f };
    CPanel panel = { 0.5f };
    CCodaPlayback p; p.m_pSource = &s; p.m_b3D = false;
    p.SetVolume(&panel, 0.8f);
    bool ok = s.volume > 0.3999f && s.volume < 0.4001f;
    p.m_b3D = true;
    p.SetVolume(&panel, 0.8f);
    ok = ok && s.volume > 0.7999f && s.volume < 0.8001f && g_locks == 2 && g_unlocks == 2 && g_lockedCalls == 2;
    if (!ok) { std::printf("00c27620_TEST FAIL\n"); return 1; }
    std::printf("00c27620_TEST PASS\n");
    return 0;
}