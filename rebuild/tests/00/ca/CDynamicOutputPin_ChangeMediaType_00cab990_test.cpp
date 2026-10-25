// Behaviour test for 00cab990: CDynamicOutputPin::ChangeMediaType -- not connected, dynamic accept (helper), reconnect fallback
#include <cstdio>
static int g_released = 0, g_accept = 0, g_helper = 0, g_reconnects = 0; static long g_helperHr = 0; static const void* g_pmt = 0;
struct IPinConnection { virtual long DynamicQueryAccept(const void* pmt) { g_pmt = pmt; return g_accept ? 0 : 1; } virtual void Release() { ++g_released; } };
static IPinConnection g_conn; static int g_hasConn = 1;
struct IPin { virtual long QueryInterface(void** pp) { *pp = g_hasConn ? &g_conn : 0; return g_hasConn ? 0 : (long)0x80004002L; } };
struct IGraphConfig { virtual long Reconnect(void* out, void* in, const void* pmt, void* filter, void* stop, unsigned long flags)
    { ++g_reconnects; return (in == 0 && filter == 0 && stop == (void*)3 && flags == 2 && pmt == (void*)0x44 && out != 0) ? 0x77 : -1; } };
template <class T> struct QzCComPtr { T* p; QzCComPtr() : p(0) { } ~QzCComPtr() { if (p) p->Release(); } };
class CDynamicOutputPin {
public:
    IPin* m_Connected; void* m_hStopEvent; IGraphConfig* m_pGraphConfig;
    long ChangeMediaTypeHelper(const void*) { ++g_helper; return g_helperHr; }
    long DynamicReconnect(const void* pmt)
    { if (m_pGraphConfig == 0 || 0 == m_hStopEvent) return (long)0x80004005L; return m_pGraphConfig->Reconnect(this, 0, pmt, 0, m_hStopEvent, 2); }
    long ChangeMediaType(const void* pmt)
    {
        if (!m_Connected) return (long)0x80040209L;
        QzCComPtr<IPinConnection> pConnection;
        m_Connected->QueryInterface((void**)&pConnection.p);
        if (pConnection.p != 0) {
            if (0 == pConnection.p->DynamicQueryAccept(pmt)) {
                long hr = ChangeMediaTypeHelper(pmt);
                if (hr < 0) return hr;
                return 0;
            }
        }
        return DynamicReconnect(pmt);
    }
};
int main()
{
    IPin pin; IGraphConfig gc; CDynamicOutputPin p; p.m_Connected = 0; p.m_hStopEvent = (void*)3; p.m_pGraphConfig = &gc;
    const void* pmt = (const void*)0x44; int ok = 1;
    ok &= p.ChangeMediaType(pmt) == (long)0x80040209L;
    p.m_Connected = &pin;
    g_accept = 1; g_helperHr = 5; ok &= p.ChangeMediaType(pmt) == 0 && g_helper == 1 && g_released == 1 && g_pmt == pmt;
    g_helperHr = (long)0x80004005L; ok &= p.ChangeMediaType(pmt) == (long)0x80004005L && g_released == 2;
    g_accept = 0; ok &= p.ChangeMediaType(pmt) == 0x77 && g_reconnects == 1 && g_released == 3;
    g_hasConn = 0; ok &= p.ChangeMediaType(pmt) == 0x77 && g_reconnects == 2 && g_released == 3;
    p.m_hStopEvent = 0; ok &= p.ChangeMediaType(pmt) == (long)0x80004005L && g_reconnects == 2;
    std::printf(ok ? "00cab990_TEST PASS\n" : "00cab990_TEST FAIL\n");
    return ok ? 0 : 1;
}
