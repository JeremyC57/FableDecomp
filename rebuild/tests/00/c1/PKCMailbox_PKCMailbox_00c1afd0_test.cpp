// Behaviour test for 00c1afd0: PKCMailbox constructor -- section, queue pointer, semaphore (0, capacity)
#include <cstdio>
static int g_sections = 0; static long g_init = -1, g_max = -1;
struct PKCCriticalSection { unsigned char m_section[0x18]; PKCCriticalSection() { ++g_sections; } };
struct PKCSemaphore { void* m_h; PKCSemaphore(long i, long m) : m_h(0) { g_init = i; g_max = m; } };
struct PKCStaticQueue { long m_capacity; };
struct PKCMailbox { PKCCriticalSection m_lock; PKCStaticQueue* m_pQueue; PKCSemaphore m_available;
    PKCMailbox(PKCStaticQueue* queue, long capacity) : m_lock(), m_pQueue(queue), m_available(0, capacity) { } };
static bool Check() { PKCStaticQueue q; PKCMailbox m(&q, 0x28); return m.m_pQueue == &q && g_sections == 1 && g_init == 0 && g_max == 0x28 && sizeof(PKCMailbox) == 0x20; }

int main()
{
    if (!Check()) { std::printf("00c1afd0_TEST FAIL\n"); return 1; }
    std::printf("00c1afd0_TEST PASS\n");
    return 0;
}
