// behaviour test for 0x00C276B0 CCodaPlayback::Update: started flag, loop-pass counting, stop after the last pass
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
    void Update();
};

void CCodaPlayback::Update()
{
    CScopedLock lock(g_soundLock);
    unsigned long position = CODA_GetSourcePosition(m_pSource);
    if (position)
        m_bStarted = true;
    if (!m_bLooping)
        return;
    if (m_bArmed) {
        if (position > m_loopPoint) {
            if (m_loopsLeft != 0) {
                if (m_loopsLeft > 0)
                    --m_loopsLeft;
            } else
                CODA_StopSource(m_pSource, false);
            m_bArmed = 0;
        }
    } else if (position < m_loopPoint)
        m_bArmed = 1;
}

int main()
{
    CODASource s; s.playPosition = 0; s.stops = 0;
    CCodaPlayback p; p.m_pSource = &s; p.m_loopsLeft = 1; p.m_loopPoint = 100; p.m_bArmed = 0;
    p.m_bLooping = true; p.m_bStarted = false;
    p.Update();                                    // position 0: not started, arms (0 < 100)
    bool ok = !p.m_bStarted && p.m_bArmed == 1;
    s.playPosition = 50; p.Update();               // still below the loop point
    ok = ok && p.m_bStarted && p.m_bArmed == 1 && p.m_loopsLeft == 1;
    s.playPosition = 150; p.Update();              // first pass done: count down, disarm
    ok = ok && p.m_bArmed == 0 && p.m_loopsLeft == 0 && s.stops == 0;
    s.playPosition = 160; p.Update();              // disarmed and above: nothing
    ok = ok && p.m_bArmed == 0 && s.stops == 0;
    s.playPosition = 10; p.Update();               // wrapped: re-arm
    ok = ok && p.m_bArmed == 1;
    s.playPosition = 120; p.Update();              // last pass done: stop
    ok = ok && s.stops == 1 && p.m_bArmed == 0 && p.m_loopsLeft == 0;
    p.m_loopsLeft = -1; p.m_bArmed = 1; s.playPosition = 130; p.Update();   // forever: no count, no stop
    ok = ok && p.m_loopsLeft == -1 && p.m_bArmed == 0 && s.stops == 1;
    p.m_bLooping = false; p.m_bArmed = 0; s.playPosition = 5; p.Update();   // not looping: never arms
    ok = ok && p.m_bArmed == 0 && g_locks == 8 && g_unlocks == 8;
    if (!ok) { std::printf("00c276b0_TEST FAIL\n"); return 1; }
    std::printf("00c276b0_TEST PASS\n");
    return 0;
}