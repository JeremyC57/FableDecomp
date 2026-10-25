// Behaviour test for 00c1dce0: CReplyMailbox destructor -- the mailbox's semaphore, then its critical section
#include <cstdio>
static int g_order = 0, g_semAt = 0, g_secAt = 0;
struct PKCCriticalSection { unsigned char m_s[0x18]; ~PKCCriticalSection() { g_secAt = ++g_order; } };
struct PKCSemaphore { void* m_h; ~PKCSemaphore() { g_semAt = ++g_order; } };
struct Mailbox { PKCCriticalSection m_lock; void* m_pQueue; PKCSemaphore m_available; };
struct CReplyMailbox { unsigned char m_queue[0x10]; Mailbox m_mailbox; ~CReplyMailbox() {} };
int main()
{
    { CReplyMailbox r; (void)r; }
    if (g_semAt != 1 || g_secAt != 2 || sizeof(CReplyMailbox) != 0x30) { std::printf("00c1dce0_TEST FAIL\n"); return 1; }
    std::printf("00c1dce0_TEST PASS\n");
    return 0;
}
