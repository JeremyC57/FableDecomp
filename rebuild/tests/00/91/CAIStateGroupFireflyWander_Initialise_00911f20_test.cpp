#include <cstdio>
// Behaviour model of CAIStateGroup_FireflyWander::Initialise @ 0x00911F20.
struct CAIBrain { int x; }; struct CCharString { int x; }; typedef long EScriptAIPriority;
struct C3DVector { float X, Y, Z; };
struct CThingAICreature { C3DVector m_pos; C3DVector* GetPosition() { return &m_pos; } };
static int g_baseCalls = 0; static long g_prio = 0;
struct CAIStateGroupBase { virtual void v00() {} CAIBrain* PBrain; long ScriptAIPriority; void* Name_PStringData; unsigned char Terminated, Priority, Enabled, _pad;
    void Initialise(CAIBrain& brain, CThingAICreature& creature, long priority, EScriptAIPriority scriptPriority, const CCharString& name) { g_baseCalls++; g_prio = priority; PBrain = &brain; } };
struct CAIStateGroup_FlyingThingBase : CAIStateGroupBase { float m_heightAboveGround, m_speed, m_minHeight, m_maxHeight; };
struct CAIStateGroup_FireflyWander : CAIStateGroup_FlyingThingBase { C3DVector m_wanderCentre; long m_frameLastChangedDirection;
    void Initialise(CAIBrain& brain, CThingAICreature& creature, long priority, EScriptAIPriority scriptPriority, const CCharString& name)
    { m_wanderCentre = *creature.GetPosition(); m_frameLastChangedDirection = 0; m_heightAboveGround = 0.7f; m_minHeight = 2.5f; m_maxHeight = 6.0f;
      CAIStateGroupBase::Initialise(brain, creature, priority, scriptPriority, name); } };
int main()
{
    CAIBrain brain; CCharString name; CThingAICreature c; c.m_pos.X = 1.0f; c.m_pos.Y = 2.0f; c.m_pos.Z = 3.0f;
    CAIStateGroup_FireflyWander g; g.m_frameLastChangedDirection = 99; g.m_speed = 42.0f;
    g.Initialise(brain, c, 5, 2, name);
    bool ok = g.m_wanderCentre.X == 1.0f && g.m_wanderCentre.Y == 2.0f && g.m_wanderCentre.Z == 3.0f && g.m_frameLastChangedDirection == 0
        && g.m_heightAboveGround == 0.7f && g.m_minHeight == 2.5f && g.m_maxHeight == 6.0f && g.m_speed == 42.0f && g_baseCalls == 1 && g_prio == 5 && g.PBrain == &brain;
    if (!ok) { std::printf("00911f20_TEST FAIL\n"); return 1; }
    std::printf("00911f20_TEST PASS\n");
    return 0;
}