// Behaviour test for 00c60af0: CEaxContext::Reset -- clear, invalidate unless a resend is pending, return the queue
#include <cstdio>
struct SEaxEvent { int tag; };
static int g_clears = 0, g_invalidates = 0; static SEaxEvent g_ev;
class CEaxContext {
public:
    unsigned long m_sourceFlags; SEaxEvent* m_pEvents;
    void ClearEvents() { ++g_clears; m_pEvents = 0; }
    long InvalidateAll() { ++g_invalidates; m_sourceFlags |= 0x80000000; m_pEvents = &g_ev; return -7; }
    long Reset(SEaxEvent** ppEvents)
    {
        long result = 0;
        ClearEvents();
        if (!(m_sourceFlags & 0x80000000))
            result = InvalidateAll();
        *ppEvents = m_pEvents;
        return result;
    }
};
int main()
{
    CEaxContext c; c.m_sourceFlags = 0; c.m_pEvents = 0; SEaxEvent* out = 0;
    int ok = c.Reset(&out) == -7 && out == &g_ev && g_clears == 1 && g_invalidates == 1;
    ok = ok && c.Reset(&out) == 0 && out == 0 && g_clears == 2 && g_invalidates == 1;
    std::printf(ok ? "00c60af0_TEST PASS\n" : "00c60af0_TEST FAIL\n");
    return ok ? 0 : 1;
}
