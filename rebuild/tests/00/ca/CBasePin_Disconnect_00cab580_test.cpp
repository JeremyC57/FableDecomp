// Behaviour test for 0x00CAB580 CBasePin::Disconnect (standalone copy of the logic with DisconnectInternal; lock stubbed)
#include <cstdio>
typedef int HRESULT;
static int g_locked;
struct Lock { Lock() { ++g_locked; } ~Lock() { --g_locked; } };
struct Peer { int refs; void Release() { --refs; } };
struct Pin {
    int m_state; Peer* m_Connected; HRESULT m_breakResult; int m_breaks;
    HRESULT BreakConnect() { ++m_breaks; return m_breakResult; }
    HRESULT DisconnectInternal()
    {
        if (m_Connected) {
            HRESULT hr = BreakConnect();
            if (hr < 0) return hr;
            m_Connected->Release();
            m_Connected = 0;
            return 0;
        }
        return 1;
    }
    HRESULT Disconnect()
    {
        Lock cObjectLock;
        if (m_state != 0) return (HRESULT)0x80040224L;
        return DisconnectInternal();
    }
};
int main()
{
    Peer peer = { 2 };
    Pin p = { 1, &peer, 0, 0 };
    if (p.Disconnect() != (HRESULT)0x80040224L || p.m_breaks != 0) { std::printf("00cab580_TEST FAIL\n"); return 1; }
    p.m_state = 0; p.m_breakResult = (HRESULT)0x80004005L;
    if (p.Disconnect() != (HRESULT)0x80004005L || p.m_Connected != &peer || peer.refs != 2) { std::printf("00cab580_TEST FAIL\n"); return 1; }
    p.m_breakResult = 0;
    if (p.Disconnect() != 0 || p.m_Connected != 0 || peer.refs != 1) { std::printf("00cab580_TEST FAIL\n"); return 1; }
    if (p.Disconnect() != 1 || g_locked != 0) { std::printf("00cab580_TEST FAIL\n"); return 1; }
    std::printf("00cab580_TEST PASS\n");
    return 0;
}