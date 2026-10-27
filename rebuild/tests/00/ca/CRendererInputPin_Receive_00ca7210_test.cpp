// Behaviour test for 0x00CA7210 CRendererInputPin::Receive (standalone copy of the logic; renderer and locks stubbed)
#include <cstdio>
typedef int HRESULT;
static int g_locked;
struct Lock { Lock() { ++g_locked; } ~Lock() { --g_locked; } };
struct Renderer {
    HRESULT m_receiveResult; int m_bAbort; int m_streaming; int m_eosDelivered;
    int m_events; HRESULT m_lastEventParam; int m_eosNotified;
    HRESULT Receive() { return m_receiveResult; }
    void NotifyEvent(int code, HRESULT p1, int p2) { if (code == 3 && p2 == 0) { ++m_events; m_lastEventParam = p1; } }
    void NotifyEndOfStream() { ++m_eosNotified; }
};
struct Pin {
    Renderer* m_pRenderer; int m_stopped; int m_flushing; int m_bRunTimeError;
    HRESULT Receive()
    {
        HRESULT hr = m_pRenderer->Receive();
        if (hr < 0) {
            Lock cRendererLock;
            if (!m_stopped && !m_flushing && !m_pRenderer->m_bAbort && !m_bRunTimeError) {
                m_pRenderer->NotifyEvent(3, hr, 0);
                {
                    Lock alRendererLock;
                    if (m_pRenderer->m_streaming && !m_pRenderer->m_eosDelivered) m_pRenderer->NotifyEndOfStream();
                }
                m_bRunTimeError = 1;
            }
        }
        return hr;
    }
};
int main()
{
    Renderer r = { 0, 0, 1, 0, 0, 0, 0 };
    Pin p = { &r, 0, 0, 0 };
    if (p.Receive() != 0 || r.m_events != 0) { std::printf("00ca7210_TEST FAIL\n"); return 1; }
    r.m_receiveResult = (HRESULT)0x80004005L; p.m_flushing = 1;
    if (p.Receive() != (HRESULT)0x80004005L || r.m_events != 0 || p.m_bRunTimeError) { std::printf("00ca7210_TEST FAIL\n"); return 1; }
    p.m_flushing = 0;
    if (p.Receive() != (HRESULT)0x80004005L || r.m_events != 1 || r.m_lastEventParam != (HRESULT)0x80004005L || r.m_eosNotified != 1 || !p.m_bRunTimeError) { std::printf("00ca7210_TEST FAIL\n"); return 1; }
    if (p.Receive() != (HRESULT)0x80004005L || r.m_events != 1 || r.m_eosNotified != 1 || g_locked != 0) { std::printf("00ca7210_TEST FAIL\n"); return 1; }
    std::printf("00ca7210_TEST PASS\n");
    return 0;
}