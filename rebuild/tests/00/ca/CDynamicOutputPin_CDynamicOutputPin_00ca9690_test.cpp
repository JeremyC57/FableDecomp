// Behaviour test for 0x00CA9690 CDynamicOutputPin::~CDynamicOutputPin: close each held event handle (unblock, then
// notify-caller), then delete m_BlockStateLock's critical section, then ~CBasePin.
#include <cstdio>
#include <cstring>
static char g_seq[32]; static int g_n = 0;
static void Log(char c) { g_seq[g_n++] = c; g_seq[g_n] = 0; }
static int CloseHandle(void* h) { Log(h == (void*)0x10 ? 'u' : 'n'); return 1; }
struct CCritSec { int cs[6]; ~CCritSec() { Log('D'); } };
struct CBasePin { virtual ~CBasePin() { Log('P'); } };
struct CDynamicOutputPin : CBasePin {
    ~CDynamicOutputPin()
    {
        if (0 != m_hUnblockOutputPinEvent) CloseHandle(m_hUnblockOutputPinEvent);
        if (0 != m_hNotifyCallerPinBlockedEvent) CloseHandle(m_hNotifyCallerPinBlockedEvent);
    }
    CCritSec m_BlockStateLock; void* m_hUnblockOutputPinEvent; void* m_hNotifyCallerPinBlockedEvent;
};
int main()
{
    CDynamicOutputPin* p = new CDynamicOutputPin;
    p->m_hUnblockOutputPinEvent = (void*)0x10; p->m_hNotifyCallerPinBlockedEvent = (void*)0x20;
    delete p;
    if (std::strcmp(g_seq, "unDP") != 0) { std::printf("00ca9690_TEST FAIL\n"); return 1; }
    g_n = 0; g_seq[0] = 0;
    CDynamicOutputPin* q = new CDynamicOutputPin;
    q->m_hUnblockOutputPinEvent = 0; q->m_hNotifyCallerPinBlockedEvent = 0;
    delete q;
    if (std::strcmp(g_seq, "DP") != 0) { std::printf("00ca9690_TEST FAIL\n"); return 1; }
    std::printf("00ca9690_TEST PASS\n");
    return 0;
}