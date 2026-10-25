// Behaviour test for 00c1b2a0: CRequestQueue destructor -- mailbox semaphore, mailbox section, then the queue
#include <cstdio>
static int g_order[4]; static int g_n = 0;
struct PKCCriticalSection { unsigned char m_section[0x18]; ~PKCCriticalSection() { g_order[g_n++] = 2; } };
struct PKCSemaphore { void* m_h; ~PKCSemaphore() { g_order[g_n++] = 1; } };
struct PKCStaticQueue { long m_capacity; ~PKCStaticQueue() { g_order[g_n++] = 3; } };
struct PKCMailbox { PKCCriticalSection m_lock; PKCStaticQueue* m_pQueue; PKCSemaphore m_available; };
struct CRequestQueue { PKCStaticQueue m_queue; PKCMailbox m_mailbox; ~CRequestQueue() { } };
static bool Check() { { CRequestQueue q; } return g_n == 3 && g_order[0] == 1 && g_order[1] == 2 && g_order[2] == 3; }

int main()
{
    if (!Check()) { std::printf("00c1b2a0_TEST FAIL\n"); return 1; }
    std::printf("00c1b2a0_TEST PASS\n");
    return 0;
}
