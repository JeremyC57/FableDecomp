#include <cstdio>
static int g_cleanups = 0;
struct CAIStateGroupBase { virtual ~CAIStateGroupBase() {} virtual void Cleanup() {} virtual void CleanupCrowd() { ++g_cleanups; } unsigned char _pad[0x10]; };
struct CAIStateGroup_MinionBehaviourCrowdBase : CAIStateGroupBase { unsigned char _pad14[0x24]; bool m_inCrowd; virtual void Cleanup(); };
void CAIStateGroup_MinionBehaviourCrowdBase::Cleanup() { m_inCrowd = false; CleanupCrowd(); }
int main()
{
    CAIStateGroup_MinionBehaviourCrowdBase g; g.m_inCrowd = true; CAIStateGroupBase* p = &g; p->Cleanup();
    if (g.m_inCrowd || g_cleanups != 1) { std::printf("00957be0_TEST FAIL\n"); return 1; }
    std::printf("00957be0_TEST PASS\n"); return 0;
}