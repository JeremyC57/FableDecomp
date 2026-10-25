// Behaviour test for 00c1b6d0: CRequestQueue constructor -- queue with the capacity, then the mailbox over it
#include <cstdio>
static long g_qcap = 0, g_mcap = 0; static void* g_mq = 0;
struct PKCStaticQueue { long m_capacity; PKCStaticQueue(long c) : m_capacity(c) { g_qcap = c; } };
struct PKCMailbox { void* m_q; PKCMailbox(PKCStaticQueue* q, long c) : m_q(q) { g_mq = q; g_mcap = c; } };
struct CRequestQueue { PKCStaticQueue m_queue; PKCMailbox m_mailbox; CRequestQueue(long capacity) : m_queue(capacity), m_mailbox(&m_queue, capacity) { } };
static bool Check() { CRequestQueue r(0x28); return g_qcap == 0x28 && g_mcap == 0x28 && g_mq == &r.m_queue; }

int main()
{
    if (!Check()) { std::printf("00c1b6d0_TEST FAIL\n"); return 1; }
    std::printf("00c1b6d0_TEST PASS\n");
    return 0;
}
