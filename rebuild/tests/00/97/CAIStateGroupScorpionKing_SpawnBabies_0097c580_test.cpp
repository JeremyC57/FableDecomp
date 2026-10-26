// Behaviour test for 0x0097C580 CAIStateGroup_ScorpionKing::SpawnBabies: the summon action is built against the creature
// and its enemy, performed without waiting, destroyed straight after the call, and the state terminates when it is no
// longer runnable. (The source's `&Temporary(...)` is written as a scoped local here: plain C++ has no address of a
// temporary; the destruction still happens before IsStillRunnable.)
#include <cstdio>

class CThingCreatureBase { public: int Id; };

static int g_log[8];
static int g_logCount = 0;
static void Log(int what) { if (g_logCount < 8) g_log[g_logCount++] = what; }
enum { LOG_BUILT = 1, LOG_PERFORMED = 2, LOG_DESTROYED = 3, LOG_RUNNABLE = 4, LOG_TERMINATED = 5 };

struct CBaseIntelligentPointer {
    CThingCreatureBase* m_pItem;
    CThingCreatureBase* GetPItem() const { return m_pItem; }
};
class CCreatureActionBase { public:
    CThingCreatureBase* Creature;
    CThingCreatureBase* Enemy;
};
class CCombatActionBase : public CCreatureActionBase { public:
    ~CCombatActionBase() { Log(LOG_DESTROYED); }
};
class CCreatureAction_ScorpionSummon : public CCombatActionBase { public:
    CCreatureAction_ScorpionSummon(CThingCreatureBase& creature, CThingCreatureBase& enemy)
    {
        Creature = &creature; Enemy = &enemy; Log(LOG_BUILT);
    }
};
class CAIBrain { public: CThingCreatureBase* PCreature; };
class CAIStateGroupBase { public:
    CAIBrain* PBrain;
    CCreatureActionBase* LastAction;
    bool LastWait;
    CThingCreatureBase* LastCreature;
    CThingCreatureBase* LastEnemy;
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool wait)
    {
        LastAction = action; LastWait = wait; LastCreature = action->Creature; LastEnemy = action->Enemy;
        Log(LOG_PERFORMED);
        return 0;
    }
    void Terminate() { Log(LOG_TERMINATED); }
};
class CAIStateGroup_ScorpionKing : public CAIStateGroupBase {
public:
    CBaseIntelligentPointer PEnemy;
    bool Runnable;
    bool IsStillRunnable() { Log(LOG_RUNNABLE); return Runnable; }
    void SpawnBabies();
};

void CAIStateGroup_ScorpionKing::SpawnBabies()
{
    CThingCreatureBase* creature = GetCreature();
    {
        CCreatureAction_ScorpionSummon action(*creature, *PEnemy.GetPItem());
        AIAction_PerformActionAndWait(&action, false);
    }
    if (!IsStillRunnable())
        Terminate();
}

int main()
{
    CThingCreatureBase king, hero;
    CAIBrain brain; brain.PCreature = &king;
    CAIStateGroup_ScorpionKing group;
    group.PBrain = &brain; group.PEnemy.m_pItem = &hero; group.LastWait = true;

    group.Runnable = true;
    group.SpawnBabies();
    if (g_logCount != 4 || g_log[0] != LOG_BUILT || g_log[1] != LOG_PERFORMED || g_log[2] != LOG_DESTROYED ||
        g_log[3] != LOG_RUNNABLE || group.LastWait || group.LastCreature != &king || group.LastEnemy != &hero) {
        std::printf("0097c580_TEST FAIL\n");
        return 1;
    }
    g_logCount = 0;
    group.Runnable = false;
    group.SpawnBabies();
    if (g_logCount != 5 || g_log[3] != LOG_RUNNABLE || g_log[4] != LOG_TERMINATED) {
        std::printf("0097c580_TEST FAIL\n");
        return 1;
    }
    std::printf("0097c580_TEST PASS\n");
    return 0;
}