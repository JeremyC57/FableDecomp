#include <cstdio>

static int g_eventState = -1;
static int SetEventStub(void*) { g_eventState = 1; return 1; }
static int ResetEventStub(void*) { g_eventState = 0; return 1; }
static int g_calls = 0;
struct IMediaStream {
    virtual ~IMediaStream() { }
    virtual long Resume() { ++g_calls; return 15; }
    virtual long Suspend() { ++g_calls; return 16; }
};
struct CVideoPlayer {
    IMediaStream* m_pStream; void* m_hEvent;
    long Call()
    {
        ResetEventStub(m_hEvent);
        if (!m_pStream)
            return 0x80040209;
        return m_pStream->Suspend();
    }
};

int main()
{
    int ev; IMediaStream s; CVideoPlayer p; p.m_pStream = 0; p.m_hEvent = &ev;
    if (p.Call() != (long)0x80040209 || g_eventState != 0 || g_calls != 0) { std::printf("00ca9b40_TEST FAIL\n"); return 1; }
    g_eventState = -1; p.m_pStream = &s;
    if (p.Call() != 16 || g_eventState != 0 || g_calls != 1) { std::printf("00ca9b40_TEST FAIL\n"); return 1; }
    std::printf("00ca9b40_TEST PASS\n");
    return 0;
}