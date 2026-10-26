// Behaviour test for CAIStateGroup_MazeTutorialBattleUseShield::Update (0x00956460): with a live target and a
// running group the will shield (3, 5) is performed and FrameOfLastShield stamped; the group always terminates.
#include <cstdio>
static int g_performed = 0, g_terminated = 0, g_dtors = 0;
static bool g_stopping = false, g_argsOk = false;
static long g_frame = 77;
class CThingCreatureBase { public: int dummy; };
class CThing { public: unsigned char m_flags91; };
struct CBaseIntelligentPointer { CThing* m_pItem; CThing* GetPItem() const { return m_pItem; } };
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
class CCreatureActionBase { public: virtual ~CCreatureActionBase() {} };
class CCombatActionBase : public CCreatureActionBase { public: ~CCombatActionBase() { ++g_dtors; } };
static CThingCreatureBase* g_creature = 0;
class CCombatAction_WillShieldInto : public CCombatActionBase { public:
    CCombatAction_WillShieldInto(CThingCreatureBase* c, float a, float b) { g_argsOk = c == g_creature && a == 3.0f && b == 5.0f; }
};
template<class T> T* Tmp(const T& t) { return const_cast<T*>(&t); }
class CWorld { public: static long GetCurrentFrame() { return g_frame; } };
static CWorld g_world;
class CAIBrain { public: CThingCreatureBase* PCreature; };
struct CAIStateGroup_MazeTutorialBattleUseShield {
    CAIBrain* PBrain;
    CBaseIntelligentPointer PTargetThing;
    long FrameOfLastShield;
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    CWorld* GetPWorld() { return &g_world; }
    bool IsStopping() { return g_stopping; }
    long AIAction_PerformActionAndWait(CCreatureActionBase*, bool wait) { if (!wait) ++g_performed; return 2; }
    void Terminate() { ++g_terminated; }
    void Update()
    {
        if (IS_THING_ALIVE(PTargetThing) && !IsStopping()) {
            AIAction_PerformActionAndWait(Tmp(CCombatAction_WillShieldInto(GetCreature(), 3.0f, 5.0f)), false);
            FrameOfLastShield = GetPWorld()->GetCurrentFrame();
        }
        Terminate();
    }
};
int main()
{
    CThingCreatureBase c; g_creature = &c; CAIBrain brain; brain.PCreature = &c;
    CThing target; target.m_flags91 = 0;
    CAIStateGroup_MazeTutorialBattleUseShield g; g.PBrain = &brain; g.PTargetThing.m_pItem = &target; g.FrameOfLastShield = 0;
    g.Update();
    bool ok = g_performed == 1 && g_argsOk && g.FrameOfLastShield == 77 && g_terminated == 1 && g_dtors == 1;
    g_frame = 99; g_stopping = true; g.Update();
    ok = ok && g_performed == 1 && g.FrameOfLastShield == 77 && g_terminated == 2;
    g_stopping = false; target.m_flags91 = 1; g.Update();
    ok = ok && g_performed == 1 && g_terminated == 3;
    g.PTargetThing.m_pItem = 0; g.Update();
    ok = ok && g_performed == 1 && g_terminated == 4;
    if (!ok) { std::printf("00956460_TEST FAIL\n"); return 1; }
    std::printf("00956460_TEST PASS\n");
    return 0;
}