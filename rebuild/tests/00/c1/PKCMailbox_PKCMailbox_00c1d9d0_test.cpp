// Behaviour test for 00c1d9d0: PKCMailbox<SFileReply> constructor -- section, queue pointer, semaphore (0, capacity)
#include <cstdio>
static int g_sections = 0; static long g_init = -1, g_max = -1;
struct PKCCriticalSection { unsigned char m_section[0x18]; PKCCriticalSection() { ++g_sections; } };
struct PKCSemaphore { void* m_h; PKCSemaphore(long i, long m) : m_h(0) { g_init = i; g_max = m; } };
struct PKCQueue { long m_bFilled; };
struct PKCMailbox { PKCCriticalSection m_lock; PKCQueue* m_pQueue; PKCSemaphore m_available;
    PKCMailbox(PKCQueue* queue, long capacity) : m_lock(), m_pQueue(queue), m_available(0, capacity) { } };
static bool Check() { PKCQueue q; PKCMailbox m(&q, 1); return m.m_pQueue == &q && g_sections == 1 && g_init == 0 && g_max == 1 && sizeof(PKCMailbox) == 0x20; }
int main()
{
    if (!Check()) { std::printf("00c1d9d0_TEST FAIL\n"); return 1; }
    std::printf("00c1d9d0_TEST PASS\n");
    return 0;
}
