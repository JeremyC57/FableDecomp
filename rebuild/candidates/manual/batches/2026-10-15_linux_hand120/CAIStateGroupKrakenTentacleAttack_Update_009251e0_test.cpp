// Behaviour test for CAIStateGroup_KrakenTentacleAttack::Update (0x009251E0): one CCreatureAction_TentacleStrikeInto (type 0x96)
// is performed and waited on, destroyed, and the group terminates.
#include <cstdio>

// Stand-ins: the combat action records its type/creature/sound; PerformAction returns a scripted sequence.
static long g_lastType = -1;
static int g_ctors = 0, g_dtors = 0, g_waitCalls = 0, g_terminated = 0, g_performs = 0;
static bool g_soundOk = true;
struct CThingCreatureBase { int dummy; };
static CThingCreatureBase* g_lastCreature = 0;
struct CSoundPair { long m_first, m_second; CSoundPair() : m_first(0), m_second(0) {} };
struct CCreatureActionBase { virtual ~CCreatureActionBase() {} };
struct CCombatActionBase : CCreatureActionBase {
    CCombatActionBase(CThingCreatureBase* creature, long type, const CSoundPair& sound)
    {
        g_lastType = type; g_lastCreature = creature; ++g_ctors;
        if (sound.m_first || sound.m_second) g_soundOk = false;
    }
    ~CCombatActionBase() { ++g_dtors; }
};
template<class T> T* Tmp(const T& t) { return const_cast<T*>(&t); }
struct CAIBrain { CThingCreatureBase* PCreature; };
static long g_results[8]; static int g_nextResult = 0; static int g_stopAfter = 100; static int g_stopChecks = 0;
struct CAIStateGroupBase {
    CAIBrain* PBrain;
    long AIAction_PerformActionAndWait(CCreatureActionBase*, bool wait) { if (!wait) ++g_waitCalls; return 2; }
    long AIAction_PerformAction(CCreatureActionBase*) { ++g_performs; return g_results[g_nextResult++]; }
    bool IsStopping() { return ++g_stopChecks > g_stopAfter; }
    void Terminate() { ++g_terminated; }
};

struct CCreatureAction_TentacleStrikeInto : CCombatActionBase {
    CCreatureAction_TentacleStrikeInto(CThingCreatureBase* creature) : CCombatActionBase(creature, 0x96, CSoundPair()) { }
};
struct CAIStateGroup_KrakenTentacleAttack : CAIStateGroupBase {
    void Update()
    {
        AIAction_PerformActionAndWait(Tmp(CCreatureAction_TentacleStrikeInto(PBrain->PCreature)), false);
        Terminate();
    }
};
int main()
{
    CThingCreatureBase c; CAIBrain brain; brain.PCreature = &c;
    CAIStateGroup_KrakenTentacleAttack g; g.PBrain = &brain;
    g.Update();
    bool ok = g_lastType == 0x96 && g_lastCreature == &c && g_soundOk && g_ctors == 1 && g_dtors == 1 && g_waitCalls == 1 && g_terminated == 1;
    if (!ok) { std::printf("009251e0_TEST FAIL\n"); return 1; }
    std::printf("009251e0_TEST PASS\n");
    return 0;
}
