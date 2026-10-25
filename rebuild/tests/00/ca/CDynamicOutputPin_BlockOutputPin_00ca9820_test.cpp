// Behaviour test for 00ca9820: CDynamicOutputPin::BlockOutputPin -- reset unblock event, signal+close the caller's event, BLOCKED, event cleared
#include <cstdio>
#include <cstring>
static char g_seq[32] = "";
static void Log(char c, void* h) { std::size_t n = std::strlen(g_seq); g_seq[n] = c; g_seq[n + 1] = (char)('0' + (int)(long)h); g_seq[n + 2] = 0; }
static int ResetEvent(void* h) { Log('R', h); return 1; }
static int SetEvent(void* h) { Log('S', h); return 1; }
static int CloseHandle(void* h) { Log('C', h); return 1; }
class CDynamicOutputPin {
public:
    enum BLOCK_STATE { NOT_BLOCKED, PENDING, BLOCKED };
    void* m_hUnblockOutputPinEvent; void* m_hNotifyCallerPinBlockedEvent; BLOCK_STATE m_BlockState;
    void BlockOutputPin()
    {
        ResetEvent(m_hUnblockOutputPinEvent);
        SetEvent(m_hNotifyCallerPinBlockedEvent);
        CloseHandle(m_hNotifyCallerPinBlockedEvent);
        m_BlockState = BLOCKED;
        m_hNotifyCallerPinBlockedEvent = 0;
    }
};
int main()
{
    CDynamicOutputPin p; p.m_hUnblockOutputPinEvent = (void*)1; p.m_hNotifyCallerPinBlockedEvent = (void*)2; p.m_BlockState = CDynamicOutputPin::PENDING;
    p.BlockOutputPin();
    int ok = !std::strcmp(g_seq, "R1S2C2") && p.m_BlockState == 2 && p.m_hNotifyCallerPinBlockedEvent == 0;
    std::printf(ok ? "00ca9820_TEST PASS\n" : "00ca9820_TEST FAIL\n");
    return ok ? 0 : 1;
}
