// Behaviour test for CAIStateGroup_MeleeCombatNearby2::IsCombatSequenceActionPending (0x0093DAE0).
#include <cstdio>

static int g_managerCalls = 0;
static const void* g_lastCreature = 0;

class CThingCreatureBase { public:
    bool Acting;
    bool NonInterruptable;
    bool IsPerformingAnyAction() const { return Acting; }
};
class CThingAICreature : public CThingCreatureBase { };

class CCombatManager { public:
    bool IsCreaturePerformingNonInterruptableSequence(const CThingCreatureBase& creature) const
    {
        ++g_managerCalls; g_lastCreature = &creature; return creature.NonInterruptable;
    }
};
namespace boost {
template <class T> class scoped_ptr {
public:
    T* ptr;
    T& operator*() const { return *ptr; }
};
}
class CWorld { public:
    boost::scoped_ptr<CCombatManager> PThingCombatManager;
    CCombatManager& GetCombatManager() { return *PThingCombatManager; }
};
class CAIBrain { public:
    CThingAICreature* Creature;
    CThingAICreature& GetCreature() const { return *Creature; }
};
static CWorld* g_world = 0;
class CAIStateGroupBase { public:
    CAIBrain* PBrain;
    CWorld& GetWorld() const { return *g_world; }
    CThingAICreature& GetCreature() const { return PBrain->GetCreature(); }
};
class CAIStateGroup_MeleeCombatNearby2 : public CAIStateGroupBase { public:
    CCombatManager& GetCombatManager() const { return GetWorld().GetCombatManager(); }
    bool IsCombatSequenceActionPending() const
    {
        if (GetCreature().IsPerformingAnyAction())
            return true;
        CCombatManager& manager = GetCombatManager();
        return manager.IsCreaturePerformingNonInterruptableSequence(GetCreature());
    }
};

int main()
{
    CCombatManager manager;
    CWorld world; world.PThingCombatManager.ptr = &manager; g_world = &world;
    CThingAICreature creature; creature.Acting = true; creature.NonInterruptable = false;
    CAIBrain brain; brain.Creature = &creature;
    CAIStateGroup_MeleeCombatNearby2 group; group.PBrain = &brain;
    bool ok = true;
    if (!group.IsCombatSequenceActionPending() || g_managerCalls != 0) ok = false;      // acting: short-circuit
    creature.Acting = false;
    if (group.IsCombatSequenceActionPending() || g_managerCalls != 1) ok = false;       // idle, interruptable
    if (g_lastCreature != &creature) ok = false;
    creature.NonInterruptable = true;
    if (!group.IsCombatSequenceActionPending() || g_managerCalls != 2) ok = false;      // non-interruptable sequence
    if (!ok) { std::printf("0093dae0_TEST FAIL\n"); return 1; }
    std::printf("0093dae0_TEST PASS\n");
    return 0;
}