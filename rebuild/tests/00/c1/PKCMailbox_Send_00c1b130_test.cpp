// Behaviour test for 00c1b130: PKCMailbox::Send -- 1 when the queue is full, else release the semaphore, push and return 0
#include <cstdio>
static int g_locks = 0, g_releases = 0, g_pushes = 0;
struct SFileRequest { long w[7]; };
struct PKCProtectionInstance { PKCProtectionInstance(void*) { ++g_locks; } ~PKCProtectionInstance() { --g_locks; } };
struct PKCSemaphore { void Release() { ++g_releases; } };
struct PKCStaticQueue { long m_count, m_capacity; long Push(const SFileRequest*) { ++g_pushes; ++m_count; return 0; } long IsFull() { return m_count >= m_capacity; } };
struct PKCMailbox { unsigned char m_lock[0x18]; PKCStaticQueue* m_pQueue; PKCSemaphore m_available;
    long Send(const SFileRequest* item) { PKCProtectionInstance lock(m_lock); if (m_pQueue->IsFull()) return 1; m_available.Release(); m_pQueue->Push(item); return 0; } };
static bool Check() { PKCStaticQueue q; q.m_count = 0; q.m_capacity = 1; PKCMailbox m; m.m_pQueue = &q; SFileRequest r = { { 0 } };
    long a = m.Send(&r), b = m.Send(&r); return a == 0 && b == 1 && g_releases == 1 && g_pushes == 1 && g_locks == 0; }

int main()
{
    if (!Check()) { std::printf("00c1b130_TEST FAIL\n"); return 1; }
    std::printf("00c1b130_TEST PASS\n");
    return 0;
}
