#include <cstdio>

static void* g_signalled = 0;
static int SetEventStub(void* e) { g_signalled = e; return 1; }
static int g_runs = 0;
struct IMediaControl { virtual ~IMediaControl() { } virtual long Run() { ++g_runs; return 7; } };
struct CVideoPlayer {
    bool m_paused; IMediaControl* m_pControl; void* m_hEvent;
    long Resume()
    {
        SetEventStub(m_hEvent);
        m_paused = false;
        if (!m_pControl)
            return 0x8004020a;
        return m_pControl->Run();
    }
};

int main()
{
    int ev; IMediaControl c; CVideoPlayer p; p.m_paused = true; p.m_pControl = 0; p.m_hEvent = &ev;
    if (p.Resume() != (long)0x8004020a || p.m_paused || g_signalled != &ev || g_runs != 0) { std::printf("00ca9ae0_TEST FAIL\n"); return 1; }
    p.m_paused = true; p.m_pControl = &c;
    if (p.Resume() != 7 || p.m_paused || g_runs != 1) { std::printf("00ca9ae0_TEST FAIL\n"); return 1; }
    std::printf("00ca9ae0_TEST PASS\n");
    return 0;
}