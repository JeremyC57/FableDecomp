#include <cstdio>
#include <cstdlib>
#include <cstring>
// behaviour test for 0x00C14B20: exercises the same logic through a copy of the model (compiled separately from the source)
struct CGlobalLockTarget { int m_depth; };
static CGlobalLockTarget g_lock = { 0 }; static int g_depthWhenSet = -1; static int g_locks = 0;
struct CScopedLock { CGlobalLockTarget* m_pTarget; long m_state; CScopedLock(CGlobalLockTarget* t) : m_pTarget(t), m_state(1) { ++t->m_depth; ++g_locks; } ~CScopedLock() { --m_pTarget->m_depth; } };
struct CFlagHolder_00c14b20 { long _pad; bool m_flag; };
struct CLockedFlagOwner_00c14b20 {
    CFlagHolder_00c14b20* m_pHolder;
    void SetFlag() { CScopedLock lock(&g_lock); m_pHolder->m_flag = true; g_depthWhenSet = g_lock.m_depth; }
};
static bool Run()
{
    CFlagHolder_00c14b20 holder; holder.m_flag = false; CLockedFlagOwner_00c14b20 o; o.m_pHolder = &holder;
    o.SetFlag();
    return holder.m_flag && g_locks == 1 && g_depthWhenSet == 1 && g_lock.m_depth == 0;
}
int main()
{
    if (!Run()) { std::printf("00c14b20_TEST FAIL\n"); return 1; }
    std::printf("00c14b20_TEST PASS\n");
    return 0;
}
