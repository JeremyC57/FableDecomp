#include <cstdio>
// Behaviour of CConstraintOwner_00c1bed0::AddConstraintRepeat (0x00C1BED0): the argument is handed to the notify
// routine while the profile lock is held, and the lock is released afterwards.
static int g_lockDepth = 0; static int g_depthAtNotify = -1; static void* g_notified = 0;
struct CProfileLock {
    void* m_pSection; long m_entryTick;
    CProfileLock(void* section) : m_pSection(section), m_entryTick(0) { ++g_lockDepth; }
    ~CProfileLock() { --g_lockDepth; }
};
static unsigned char g_ProfileSection_0143bafc[1];
static void NotifyGlobalManager_00c336f0(void* constraint) { g_notified = constraint; g_depthAtNotify = g_lockDepth; }
struct CConstraintOwner_00c1bed0 {
    void AddConstraintRepeat(void* constraint)
    {
        CProfileLock lock(g_ProfileSection_0143bafc);
        NotifyGlobalManager_00c336f0(constraint);
    }
};
int main()
{
    CConstraintOwner_00c1bed0 o; int c;
    o.AddConstraintRepeat(&c);
    if (g_notified != &c || g_depthAtNotify != 1 || g_lockDepth != 0) { std::printf("00c1bed0_TEST FAIL\n"); return 1; }
    std::printf("00c1bed0_TEST PASS\n");
    return 0;
}