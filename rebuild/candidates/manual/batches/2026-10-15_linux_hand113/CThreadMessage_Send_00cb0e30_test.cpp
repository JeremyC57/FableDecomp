#include <cstdio>
// Behaviour test for CThreadMessage::Send (0x00CB0E30): E_FAIL without a worker thread (lock balanced),
// else the message is posted, the request event signalled, the reply awaited and returned.
static int g_locks = 0; static int g_sets = 0; static int g_waits = 0;
struct CS { int x; };
static void EnterCriticalSection(CS*) { ++g_locks; }
static void LeaveCriticalSection(CS*) { --g_locks; }
struct CThreadMessage;
static CThreadMessage* g_worker = 0;
static int SetEvent(void*) { ++g_sets; return 1; }
static unsigned long WaitForSingleObject(void*, unsigned long);
struct CThreadMessage {
    void* m_hRequestEvent; void* m_hReplyEvent; unsigned long m_message; long m_reply; void* m_hThread; CS m_lock;
    long Send(unsigned long message)
    {
        EnterCriticalSection(&m_lock);
        if (m_hThread == 0) {
            LeaveCriticalSection(&m_lock);
            return (long)0x80004005L;
        }
        m_message = message;
        SetEvent(m_hRequestEvent);
        WaitForSingleObject(m_hReplyEvent, 0xFFFFFFFF);
        long reply = m_reply;
        LeaveCriticalSection(&m_lock);
        return reply;
    }
};
static unsigned long WaitForSingleObject(void*, unsigned long) { ++g_waits; g_worker->m_reply = (long)g_worker->m_message * 2; return 0; }
int main()
{
    CThreadMessage m; m.m_hThread = 0; m.m_message = 0; m.m_reply = 0;
    g_worker = &m;
    if (m.Send(5) != (long)0x80004005L || g_locks != 0 || g_sets != 0) { std::printf("00cb0e30_TEST FAIL\n"); return 1; }
    m.m_hThread = (void*)1;
    if (m.Send(21) != 42 || g_locks != 0 || g_sets != 1 || g_waits != 1 || m.m_message != 21) { std::printf("00cb0e30_TEST FAIL\n"); return 1; }
    std::printf("00cb0e30_TEST PASS\n");
    return 0;
}
