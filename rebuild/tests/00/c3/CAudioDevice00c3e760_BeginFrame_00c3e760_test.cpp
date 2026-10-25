// Behaviour test for 00c3e760: CAudioDevice::BeginFrame -- the outputs' readiness decides whether the frame runs (mixer locked, scratch block cleared, listener committed); without a primary output the active flag toggles
#include <cstdio>
#include <string.h>

static int g_locks = 0, g_commits = 0;

struct COutput {
    bool m_ready;
    int m_asked;
    bool IsReady() { ++m_asked; return m_ready; }
};
struct CMixer { void Lock() { ++g_locks; } };
struct CListener { void Commit() { ++g_commits; } };
struct CScratchBlock {
    unsigned char m_dirty;
    long* m_pSamples;
    void Clear()
    {
        m_dirty = 0;
        memset(m_pSamples, 0, 0x80 * sizeof(long));
    }
};

static bool s_audioActive = false;

struct CAudioDevice {
    CMixer* m_pMixer;
    CScratchBlock* m_pScratch;
    COutput* m_pPrimary;
    COutput* m_pSecondary;
    CListener* m_pListener;
    bool BeginFrame()
    {
        if (m_pPrimary) {
            s_audioActive = m_pPrimary->IsReady();
            if (m_pSecondary)
                s_audioActive = s_audioActive && m_pSecondary->IsReady();
            if (s_audioActive) {
                m_pMixer->Lock();
                if (m_pScratch)
                    m_pScratch->Clear();
            }
        } else
            s_audioActive = !s_audioActive;
        if (s_audioActive && m_pListener)
            m_pListener->Commit();
        return s_audioActive;
    }
};

static int fail() { std::printf("00c3e760_TEST FAIL\n"); return 1; }

int main()
{
    long samples[0x80];
    for (int i = 0; i < 0x80; ++i) samples[i] = i + 1;
    CScratchBlock scratch = { 1, samples };
    CMixer mixer; CListener listener;
    COutput primary = { true, 0 }, secondary = { false, 0 };
    CAudioDevice d = { &mixer, &scratch, &primary, &secondary, &listener };
    if (d.BeginFrame() || g_locks != 0 || secondary.m_asked != 1) return fail();      // secondary not ready
    secondary.m_ready = true;
    if (!d.BeginFrame() || g_locks != 1 || g_commits != 1 || scratch.m_dirty != 0 || samples[0x7f] != 0) return fail();
    primary.m_ready = false;
    if (d.BeginFrame() || secondary.m_asked != 2 || g_locks != 1) return fail();      // && short-circuits
    d.m_pPrimary = 0;                                                               // no primary: toggle
    if (!d.BeginFrame() || g_commits != 2) return fail();
    if (d.BeginFrame() || g_commits != 2) return fail();
    std::printf("00c3e760_TEST PASS\n");
    return 0;
}