#include <cstdio>
// Behaviour test for CThreadMessage::TryReceive (0x00CB0EB0): 0 when the request event is not signalled,
// else 1 with the message copied to the optional out pointer.
static unsigned long g_waitResult = 0;
static unsigned long WaitForSingleObject(void*, unsigned long ms) { return ms == 0 ? g_waitResult : 0xFFFFFFFF; }
struct CThreadMessage {
    void* m_hRequestEvent; void* m_hReplyEvent; unsigned long m_message; long m_reply; void* m_hThread;
    int TryReceive(unsigned long* message)
    {
        if (WaitForSingleObject(m_hRequestEvent, 0) != 0)
            return 0;
        if (message)
            *message = m_message;
        return 1;
    }
};
int main()
{
    CThreadMessage m; m.m_message = 77;
    unsigned long got = 0;
    g_waitResult = 0x102;   // WAIT_TIMEOUT
    if (m.TryReceive(&got) != 0 || got != 0) { std::printf("00cb0eb0_TEST FAIL\n"); return 1; }
    g_waitResult = 0;
    if (m.TryReceive(&got) != 1 || got != 77) { std::printf("00cb0eb0_TEST FAIL\n"); return 1; }
    if (m.TryReceive(0) != 1) { std::printf("00cb0eb0_TEST FAIL\n"); return 1; }
    std::printf("00cb0eb0_TEST PASS\n");
    return 0;
}