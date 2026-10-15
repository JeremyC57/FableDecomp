// Behaviour test for CAIStateGroup_KrakenTentacleIdle::Update (0x009250F0): idle actions (type 0x32) repeat
// while each returns 2 and the group is not stopping; then the group terminates once.
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

struct CCreatureAction_TentacleIdle : CCombatActionBase {
    CCreatureAction_TentacleIdle(CThingCreatureBase* creature) : CCombatActionBase(creature, 0x32, CSoundPair()) { }
};
struct CAIStateGroup_KrakenTentacleIdle : CAIStateGroupBase {
    void Update()
    {
        while (!IsStopping()) {
            if (AIAction_PerformAction(Tmp(CCreatureAction_TentacleIdle(PBrain->PCreature))) != 2)
                break;
        }
        Terminate();
    }
};
int main()
{
    CThingCreatureBase c; CAIBrain brain; brain.PCreature = &c;
    CAIStateGroup_KrakenTentacleIdle g; g.PBrain = &brain;
    g_results[0] = 2; g_results[1] = 2; g_results[2] = 3;          // third action fails
    g.Update();
    bool ok = g_performs == 3 && g_terminated == 1 && g_dtors == 3 && g_lastType == 0x32 && g_soundOk;
    g_performs = 0; g_nextResult = 0; g_stopChecks = 0; g_stopAfter = 1; g_results[0] = 2; g_results[1] = 2;
    g.Update();                                                    // stops after one completed action
    ok = ok && g_performs == 1 && g_terminated == 2;
    g_performs = 0; g_nextResult = 0; g_stopChecks = 0; g_stopAfter = 0;
    g.Update();                                                    // already stopping
    ok = ok && g_performs == 0 && g_terminated == 3;
    if (!ok) { std::printf("009250f0_TEST FAIL\n"); return 1; }
    std::printf("009250f0_TEST PASS\n");
    return 0;
}