// Behaviour test for 00c1b030: PKCMailbox::Receive -- wait, lock, (warn when empty), pop into *item, return item
#include <cstdio>
static int g_waits = 0, g_locks = 0, g_warns = 0;
struct Item { long a, b; };
struct PKCProtectionInstance { PKCProtectionInstance(void*) { ++g_locks; } ~PKCProtectionInstance() { --g_locks; } };
struct PKCSemaphore { void Wait() { ++g_waits; } };
struct Queue { long m_count; Item m_item; unsigned long GetCount() { return m_count; } void Pop(Item* out) { *out = m_item; --m_count; } };
struct PKCMailbox { unsigned char m_lock[0x18]; Queue* m_pQueue; PKCSemaphore m_available;
    Item* Receive(Item* item) { m_available.Wait(); PKCProtectionInstance lock(m_lock); if (!(m_pQueue->GetCount() > 0)) ++g_warns; m_pQueue->Pop(item); return item; } };
static bool Check() { Queue q; q.m_count = 1; q.m_item.a = 3; q.m_item.b = 4; PKCMailbox m; m.m_pQueue = &q; Item out = { 0, 0 };
    Item* r = m.Receive(&out); return r == &out && out.a == 3 && out.b == 4 && g_waits == 1 && g_locks == 0 && g_warns == 0 && q.m_count == 0; }
int main()
{
    if (!Check()) { std::printf("00c1b030_TEST FAIL\n"); return 1; }
    std::printf("00c1b030_TEST PASS\n");
    return 0;
}
