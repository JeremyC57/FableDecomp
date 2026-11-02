// Behaviour test for CThingAICreature::OnKill (0x00830D80): a creature in melee is deregistered from its combat manager; its quest-info
// elements are always removed; a non-null brain is told; the creature-base OnKill runs last.
#include <cstdio>
static int gOrder, gDeregAt, gQuestAt, gBrainAt, gBaseAt; static bool gInMelee; static const void* gDereg; static const void* gQuestThing;
class CThing { public: virtual ~CThing() { } virtual void OnKill() { } };
class CThingCreatureBase;
class CCombatManager { public: void DeregisterDestroyedCombatant(CThingCreatureBase& c) { gDeregAt = ++gOrder; gDereg = &c; } };
static CCombatManager gCombat;
class CThingCreatureBase : public CThing { public:
    virtual void OnKill() { gBaseAt = ++gOrder; }
    bool IsInMelee() const { return gInMelee; }
    CCombatManager& GetCombatManager() { return gCombat; }
};
class CAIBrain { public: void OnKill() { gBrainAt = ++gOrder; } };
struct CDrawQuestInfo { void RemoveElementsForThing(const CThing& t) { gQuestAt = ++gOrder; gQuestThing = &t; } };
class CPlayerGui { public: CDrawQuestInfo* PDrawQuestInfo; CDrawQuestInfo& GetQuestInfo() { return *PDrawQuestInfo; } };
static CDrawQuestInfo gQuest; static CPlayerGui gGui = { &gQuest };
template <class T> struct CTBaseSingletonNoDefaultAlloc { static T& Get() { return gGui; } };
template <class T> class CCountedPointer { public: T* P; bool IsNull() const { return P == 0; } T* operator->() const { return P; } };
class CThingAICreature : public CThingCreatureBase { public:
    CCountedPointer<CAIBrain> PBrain;
    virtual void OnKill();
};
void CThingAICreature::OnKill()
{
    if (IsInMelee())
        GetCombatManager().DeregisterDestroyedCombatant(*this);
    CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().GetQuestInfo().RemoveElementsForThing(*this);
    if (!PBrain.IsNull())
        PBrain->OnKill();
    CThingCreatureBase::OnKill();
}
static void Reset() { gOrder = gDeregAt = gQuestAt = gBrainAt = gBaseAt = 0; gDereg = gQuestThing = 0; }
int main()
{
    CAIBrain brain; CThingAICreature c; CThing* t = &c;
    c.PBrain.P = &brain; gInMelee = true; Reset(); t->OnKill();
    if (gDeregAt != 1 || gDereg != &c || gQuestAt != 2 || gQuestThing != &c || gBrainAt != 3 || gBaseAt != 4) { std::printf("00830d80_TEST FAIL\n"); return 1; }
    c.PBrain.P = 0; gInMelee = false; Reset(); t->OnKill();
    if (gDeregAt != 0 || gQuestAt != 1 || gBrainAt != 0 || gBaseAt != 2) { std::printf("00830d80_TEST FAIL\n"); return 1; }
    std::printf("00830d80_TEST PASS\n");
    return 0;
}