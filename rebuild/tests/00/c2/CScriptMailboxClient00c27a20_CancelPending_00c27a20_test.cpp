#include <cstdio>
// Behaviour test for CScriptMailboxClient_00c27a20::CancelPending (0x00C27A20): same logic over a copy of the model.
static int g_depth = 0, g_maxDepthAtNotify = -1, g_notifies = 0;
static long g_a = -1, g_b = -1;
struct CGlobalLockTarget { int id; };
static CGlobalLockTarget g_lock_0143bafc;
struct CScopedLock {
    CGlobalLockTarget* m_pTarget; long m_state;
    CScopedLock(CGlobalLockTarget* target) : m_pTarget(target), m_state(0) { ++g_depth; }
    ~CScopedLock() { --g_depth; }
};
struct CMailboxTarget { unsigned long flags; };
static void NotifyTarget(CMailboxTarget* target, long a, long b)
{
    if (target->flags & 0x217) { ++g_notifies; g_a = a; g_b = b; g_maxDepthAtNotify = g_depth; }
}
struct CScriptMailboxClient_00c27a20 {
    CMailboxTarget* m_pTarget; bool m_pending;
    void CancelPending() { CScopedLock lock(&g_lock_0143bafc); NotifyTarget(m_pTarget, 0, 0); m_pending = false; }
};
int main()
{
    CMailboxTarget t; t.flags = 0x200;
    CScriptMailboxClient_00c27a20 c; c.m_pTarget = &t; c.m_pending = true;
    c.CancelPending();
    if (c.m_pending || g_notifies != 1 || g_a != 0 || g_b != 0 || g_maxDepthAtNotify != 1 || g_depth != 0) { std::printf("00c27a20_TEST FAIL\n"); return 1; }
    std::printf("00c27a20_TEST PASS\n");
    return 0;
}