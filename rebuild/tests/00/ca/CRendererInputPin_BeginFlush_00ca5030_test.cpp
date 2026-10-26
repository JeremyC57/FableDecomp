// Behaviour test for 0x00ca5030: CRendererInputPin::BeginFlush (renbase.cpp) -- interface lock held throughout; the sample
// lock is held only while the pin is flagged as flushing and the renderer begins flushing; ResetEndOfStream runs after the
// sample lock is left and its result is returned.
#include <cstdio>
static int g_seq[8]; static int g_n;
struct CCritSec { int held; void Lock() { ++held; } void Unlock() { --held; } };
struct CAutoLock { CCritSec* p; CAutoLock(CCritSec* l) : p(l) { p->Lock(); } ~CAutoLock() { p->Unlock(); } };
struct Renderer {
    CCritSec m_InterfaceLock, m_RendererLock; bool flushing;
    long BeginFlush() { g_seq[g_n++] = (m_InterfaceLock.held == 1 && m_RendererLock.held == 1) ? 2 : -2; return 0; }
    long ResetEndOfStream() { g_seq[g_n++] = (m_InterfaceLock.held == 1 && m_RendererLock.held == 0) ? 3 : -3; return 0x1234; }
};
struct Pin {
    Renderer* m_pRenderer; bool m_bFlushing;
    long BaseBeginFlush() { m_bFlushing = true; g_seq[g_n++] = (m_pRenderer->m_RendererLock.held == 1) ? 1 : -1; return 0; }
    long BeginFlush() {
        CAutoLock cRendererLock(&m_pRenderer->m_InterfaceLock);
        { CAutoLock cSampleLock(&m_pRenderer->m_RendererLock); BaseBeginFlush(); m_pRenderer->BeginFlush(); }
        return m_pRenderer->ResetEndOfStream();
    }
};
int main()
{
    Renderer r = {{0}, {0}, false}; Pin p = {&r, false};
    long hr = p.BeginFlush();
    bool ok = hr == 0x1234 && g_n == 3 && g_seq[0] == 1 && g_seq[1] == 2 && g_seq[2] == 3 && p.m_bFlushing
              && r.m_InterfaceLock.held == 0 && r.m_RendererLock.held == 0;
    if (!ok) { std::printf("00ca5030_TEST FAIL\n"); return 1; }
    std::printf("00ca5030_TEST PASS\n");
    return 0;
}
