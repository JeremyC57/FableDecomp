#include <cstdio>
struct CAIBrain {}; struct CThingAICreature {}; struct CCharString {};
static int g_order[2]; static int g_n = 0; static long g_id = 0;
struct CAIStateGroupBase { virtual ~CAIStateGroupBase() {}
    virtual void Initialise(CAIBrain&, CThingAICreature&, long id, long, const CCharString&) { g_order[g_n++] = 2; g_id = id; }
    virtual void CleanupCrowd() {} virtual void InitialiseCrowd() { g_order[g_n++] = 1; } unsigned char _pad[0x10]; };
struct CAIStateGroup_MinionBehaviourCrowdBase : CAIStateGroupBase { unsigned char _pad14[0x24]; bool m_inCrowd;
    virtual void Initialise(CAIBrain& b, CThingAICreature& c, long id, long p, const CCharString& n); };
void CAIStateGroup_MinionBehaviourCrowdBase::Initialise(CAIBrain& brain, CThingAICreature& creature, long id, long priority, const CCharString& name)
{ m_inCrowd = false; InitialiseCrowd(); CAIStateGroupBase::Initialise(brain, creature, id, priority, name); }
int main()
{
    CAIStateGroup_MinionBehaviourCrowdBase g; g.m_inCrowd = true; CAIBrain b; CThingAICreature c; CCharString n;
    g.Initialise(b, c, 9, 1, n);
    if (g.m_inCrowd || g_n != 2 || g_order[0] != 1 || g_order[1] != 2 || g_id != 9) { std::printf("00957bb0_TEST FAIL\n"); return 1; }
    std::printf("00957bb0_TEST PASS\n"); return 0;
}
