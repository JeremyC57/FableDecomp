// behaviour test for 0x00C27890 CCodaPlayback::GetPlayProgress (copy of the model with stub callees)
#include <cstdio>
#include <cmath>
class PKCCriticalSection { public: unsigned char m_section[0x18]; };
static int g_lockDepth = 0;
class PKCProtectionInstance {
public:
    PKCProtectionInstance(PKCCriticalSection* section) : m_pSection(section), m_tick(0) { ++g_lockDepth; }
    ~PKCProtectionInstance() { --g_lockDepth; }
    PKCCriticalSection* m_pSection;
    unsigned long m_tick;
};
static PKCCriticalSection g_soundLock;
struct CODASource { long playing; unsigned long position; };
static long CODA_IsSourcePlaying(CODASource* source) { return source->playing; }
static unsigned long CODA_GetSourcePosition(CODASource* source) { return source->position; }
static unsigned long g_now = 0;
static unsigned long GetTickCount(void) { return g_now; }
struct CPanel { float m_masterVolume; unsigned long m_endTimeout; };
struct SPlayProgress { float m_fraction; long m_loopsLeft; };
struct CCodaPlayback {
    virtual void SetSourceProperty20(float) {}
    virtual void SetVolume(CPanel*, float) {}
    virtual void GetPlayProgress(CPanel* panel, SPlayProgress* progress);
    CODASource*   m_pSource;
    long          m_loopsLeft;
    unsigned long m_loopPoint;
    long          m_bArmed;
    unsigned long m_length;
    bool          m_bLooping;
    bool          m_b3D;
    bool          m_bStarted;
    bool          m_bEndTimerRunning;
    unsigned long m_endTimerStart;
};
void CCodaPlayback::GetPlayProgress(CPanel* panel, SPlayProgress* progress)
{
    PKCProtectionInstance lock(&g_soundLock);
    if (CODA_IsSourcePlaying(m_pSource)) {
        progress->m_loopsLeft = m_loopsLeft;
        unsigned long position = CODA_GetSourcePosition(m_pSource);
        if (position == 0 && m_bStarted && m_loopsLeft == 0) {
            unsigned long now = GetTickCount();
            if (!m_bEndTimerRunning) {
                m_bEndTimerRunning = true;
                m_endTimerStart = now;
            } else if (now - m_endTimerStart > panel->m_endTimeout) {
                progress->m_loopsLeft = 0;
                progress->m_fraction = 1.0f;
                return;
            }
            position = m_length - 1;
        } else if (position == m_length)
            --position;
        progress->m_fraction = (float)position / (float)m_length;
    } else {
        progress->m_loopsLeft = 0;
        progress->m_fraction = 1.0f;
    }
}
static bool near(float a, float b) { return std::fabs(a - b) < 1e-5f; }
int main()
{
    CODASource src = { 1, 25 };
    CPanel panel = { 1.0f, 500 };
    CCodaPlayback p;
    p.m_pSource = &src; p.m_loopsLeft = 3; p.m_loopPoint = 0; p.m_bArmed = 0; p.m_length = 100;
    p.m_bLooping = false; p.m_b3D = false; p.m_bStarted = false; p.m_bEndTimerRunning = false; p.m_endTimerStart = 0;
    SPlayProgress r = { -1.0f, -1 };
    p.GetPlayProgress(&panel, &r);                       // playing: 25 / 100, loops copied
    if (!near(r.m_fraction, 0.25f) || r.m_loopsLeft != 3 || g_lockDepth != 0) { std::printf("00c27890_TEST FAIL\n"); return 1; }
    src.position = 100;                                  // at the end: one short of the length
    p.GetPlayProgress(&panel, &r);
    if (!near(r.m_fraction, 0.99f)) { std::printf("00c27890_TEST FAIL\n"); return 1; }
    src.playing = 0;                                     // stopped: done
    p.GetPlayProgress(&panel, &r);
    if (r.m_fraction != 1.0f || r.m_loopsLeft != 0) { std::printf("00c27890_TEST FAIL\n"); return 1; }
    // a started one-shot back at position 0: the end timer starts, then expires after the panel timeout
    src.playing = 1; src.position = 0; p.m_bStarted = true; p.m_loopsLeft = 0; g_now = 1000;
    p.GetPlayProgress(&panel, &r);
    if (!p.m_bEndTimerRunning || p.m_endTimerStart != 1000 || !near(r.m_fraction, 0.99f) || r.m_loopsLeft != 0)
    { std::printf("00c27890_TEST FAIL\n"); return 1; }
    g_now = 1500;                                        // exactly the timeout: still in progress
    p.GetPlayProgress(&panel, &r);
    if (!near(r.m_fraction, 0.99f)) { std::printf("00c27890_TEST FAIL\n"); return 1; }
    g_now = 1501;                                        // past it: done
    r.m_loopsLeft = 7;
    p.GetPlayProgress(&panel, &r);
    if (r.m_fraction != 1.0f || r.m_loopsLeft != 0 || g_lockDepth != 0) { std::printf("00c27890_TEST FAIL\n"); return 1; }
    std::printf("00c27890_TEST PASS\n");
    return 0;
}