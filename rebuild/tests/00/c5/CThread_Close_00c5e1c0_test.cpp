// Behaviour test for 00c5e1c0: CThread::Close -- closes an open handle once and clears it
#include <cstdio>
static int g_closed = 0; static void* g_last = 0;
static int CloseHandleStub(void* h) { ++g_closed; g_last = h; return 1; }
class CThread {
public:
    unsigned long m_0; void* m_handle; unsigned long m_threadId;
    void Close() { if (m_handle) { CloseHandleStub(m_handle); m_handle = 0; } }
};
int main()
{
    int h; CThread t; t.m_0 = 0; t.m_handle = &h; t.m_threadId = 9;
    t.Close();
    int ok = g_closed == 1 && g_last == &h && t.m_handle == 0;
    t.Close(); ok = ok && g_closed == 1;
    std::printf(ok ? "00c5e1c0_TEST PASS\n" : "00c5e1c0_TEST FAIL\n");
    return ok ? 0 : 1;
}
