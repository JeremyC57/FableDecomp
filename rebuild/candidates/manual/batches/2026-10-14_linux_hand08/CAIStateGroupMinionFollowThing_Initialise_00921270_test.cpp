#include <cstdio>
struct CAIBrain {}; struct CThingAICreature {}; struct CCharString {};
static int g_baseCalls = 0; static long g_id = 0;
struct CAIStateGroupBase { virtual ~CAIStateGroupBase() {} unsigned char _pad[0x10];
    void Initialise(CAIBrain&, CThingAICreature&, long id, long, const CCharString&) { ++g_baseCalls; g_id = id; } };
struct CAIStateGroup_MinionFollowThing : CAIStateGroupBase {
    unsigned char PFollowee[8]; float FollowDistance; bool AvoidDynamicObstacles, PreviousPushableByHero, ExpressionUsed, FollowerAdded; long CurrentMovementType;
    virtual void Initialise(CAIBrain& b, CThingAICreature& c, long id, long p, const CCharString& n);
};
void CAIStateGroup_MinionFollowThing::Initialise(CAIBrain& brain, CThingAICreature& creature, long id, long priority, const CCharString& name)
{ CAIStateGroupBase::Initialise(brain, creature, id, priority, name); FollowDistance = 1.0f; AvoidDynamicObstacles = true; PreviousPushableByHero = false; ExpressionUsed = false; FollowerAdded = false; CurrentMovementType = 3; }
int main()
{
    CAIStateGroup_MinionFollowThing g; CAIBrain b; CThingAICreature c; CCharString n;
    g.FollowDistance = 0; g.AvoidDynamicObstacles = false; g.PreviousPushableByHero = g.ExpressionUsed = g.FollowerAdded = true; g.CurrentMovementType = 0;
    g.Initialise(b, c, 42, 1, n);
    if (g_baseCalls != 1 || g_id != 42 || g.FollowDistance != 1.0f || !g.AvoidDynamicObstacles || g.PreviousPushableByHero || g.ExpressionUsed || g.FollowerAdded || g.CurrentMovementType != 3) { std::printf("00921270_TEST FAIL\n"); return 1; }
    std::printf("00921270_TEST PASS\n"); return 0;
}
