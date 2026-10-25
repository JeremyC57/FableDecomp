// Behaviour test for 00c1dc40: PKCMailbox<SFileReply>::Send -- 1 when the one-shot queue is full, else release the semaphore, push and return 0
#include <cstdio>
static int g_locks = 0, g_releases = 0, g_pushes = 0;
struct SFileReply { bool m_bSuccess; long m_value; };
struct PKCProtectionInstance { PKCProtectionInstance(void*) { ++g_locks; } ~PKCProtectionInstance() { --g_locks; } };
struct PKCSemaphore { void Release() { ++g_releases; } };
struct PKCOneShotQueue { long m_bFilled; SFileReply m_item; long Push(const SFileReply* r) { ++g_pushes; m_item = *r; m_bFilled = 1; return 0; } long IsFull() { return m_bFilled; } };
struct PKCMailbox { unsigned char m_lock[0x18]; PKCOneShotQueue* m_pQueue; PKCSemaphore m_available;
    long Send(const SFileReply* item) { PKCProtectionInstance lock(m_lock); if (m_pQueue->IsFull()) return 1; m_available.Release(); m_pQueue->Push(item); return 0; } };
static bool Check() { PKCOneShotQueue q; q.m_bFilled = 0; PKCMailbox m; m.m_pQueue = &q; SFileReply r; r.m_bSuccess = false; r.m_value = 7;
    long a = m.Send(&r), b = m.Send(&r); return a == 0 && b == 1 && g_releases == 1 && g_pushes == 1 && g_locks == 0 && q.m_item.m_value == 7; }
int main()
{
    if (!Check()) { std::printf("00c1dc40_TEST FAIL\n"); return 1; }
    std::printf("00c1dc40_TEST PASS\n");
    return 0;
}
