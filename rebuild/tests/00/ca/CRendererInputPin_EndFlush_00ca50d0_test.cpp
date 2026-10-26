// Behaviour test for 0x00ca50d0: CRendererInputPin::EndFlush (renbase.cpp) -- both renderer locks held; the renderer ends
// flushing first; only on success does CBaseInputPin::EndFlush run and give the result; a failure is returned as is.
#include <cstdio>
static int g_seq[4]; static int g_n;
struct CCritSec { int held; void Lock() { ++held; } void Unlock() { --held; } };
struct CAutoLock { CCritSec* p; CAutoLock(CCritSec* l) : p(l) { p->Lock(); } ~CAutoLock() { p->Unlock(); } };
struct Renderer {
    CCritSec m_InterfaceLock, m_RendererLock; long result;
    long EndFlush() { g_seq[g_n++] = (m_InterfaceLock.held == 1 && m_RendererLock.held == 1) ? 1 : -1; return result; }
};
struct Pin {
    Renderer* m_pRenderer; bool m_bFlushing;
    long BaseEndFlush() { m_bFlushing = false; g_seq[g_n++] = (m_pRenderer->m_RendererLock.held == 1) ? 2 : -2; return 7; }
    long EndFlush() {
        CAutoLock cRendererLock(&m_pRenderer->m_InterfaceLock);
        CAutoLock cSampleLock(&m_pRenderer->m_RendererLock);
        long hr = m_pRenderer->EndFlush();
        if (hr >= 0) hr = BaseEndFlush();
        return hr;
    }
};
int main()
{
    Renderer r = {{0}, {0}, 0}; Pin p = {&r, true};
    bool ok = p.EndFlush() == 7 && g_n == 2 && g_seq[0] == 1 && g_seq[1] == 2 && !p.m_bFlushing && r.m_InterfaceLock.held == 0 && r.m_RendererLock.held == 0;
    g_n = 0; r.result = (long)0x80004005; p.m_bFlushing = true;
    ok = ok && p.EndFlush() == (long)0x80004005 && g_n == 1 && p.m_bFlushing && r.m_InterfaceLock.held == 0 && r.m_RendererLock.held == 0;
    if (!ok) { std::printf("00ca50d0_TEST FAIL\n"); return 1; }
    std::printf("00ca50d0_TEST PASS\n");
    return 0;
}
