// Behaviour test for 0x009644f0 CAIStateGroup_StealStealableItem::OnReplace(): the row's logic over stub classes (C++03).
#include <stdio.h>
#include <stdlib.h>
#include <string>
static std::string trace;
static void check(bool v, int line) { if (!v) { printf("009644f0_TEST FAIL line %d\n", line); exit(1); } }
enum { MODE_CARRYING_WEAPON_STAND = 4, MODE_CARRYING_GENERIC_BOX = 34 };
enum { TCI_WEAPON = 72 };
class CCarryableDef { public: long ActiveCarrySlot, PassiveCarrySlot; mutable long RefCount; };
template <class T> class CDefPointer { public:
    T* Object; CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) { --Object->RefCount; trace += 'r'; } }
    T* operator->() const { return Object; } };
class CThing { public:
    bool Dead; bool Weapon; CCarryableDef* Carryable;
    bool IsDead() const { return Dead; }
    bool HasTC(int type) const { return type == TCI_WEAPON && Weapon; }
    bool GetSubDef(CDefPointer<const CCarryableDef>* def) const { def->Object = Carryable; ++Carryable->RefCount; return true; }
    long FreedPoint;
    template <class T> bool GetTC(T** out) { *out = (T*)this; return true; }
};
class CTCSingleUserActionPoints { public: void SetActionPointAsFree(long index) { ((CThing*)this)->FreedPoint = index; trace += 'F'; } };
template <class T> class CIntelligentPointer { public:
    T* Item; CIntelligentPointer(T* item = 0) : Item(item) { }
    T* Get() const { return Item; } T* operator->() const { return Item; } T& operator*() const { return *Item; } };
template <class T> static bool IS_THING_ALIVE(const CIntelligentPointer<T>& p) { if (p.Get() != 0) { if (!p->IsDead()) return true; } return false; }
class CCreatureActionBase { public: long ID; long GetActionID() const { return ID; } };
class CTCCreatureModeManager { public: void RemoveMode(unsigned long m) { trace += 'R'; check(m == MODE_CARRYING_GENERIC_BOX, __LINE__); }
    bool AddMode(unsigned long m) { trace += 'A'; check(m == MODE_CARRYING_WEAPON_STAND, __LINE__); return true; } };
class CTCCarrying { public:
    CThing* Carried; CThing* Slots[4];
    bool IsCarryingThing(const CThing& t) const { return Carried == &t; }
    void RemoveThing(CThing& t) { check(Carried == &t, __LINE__); Carried = 0; trace += 'D'; }
    CThing* GetThingInCarrySlot(long s) const { return Slots[s]; }
    CThing* RemoveThingInCarrySlot(long s, bool now) { check(!now, __LINE__); CThing* t = Slots[s]; Slots[s] = 0; trace += 'P'; return t; }
    void PutThingInCarrySlot(CThing& t, long s, bool now) { check(now, __LINE__); Slots[s] = &t; trace += 'S'; }
};
class CTCAIScratchpad { public: bool KillSelfAndStealableItemAfterReachingDestination; };
class CThingAICreature { public:
    CTCAIScratchpad scratch; bool DeathPersistent; int Killed;
    bool PeekTC(const CTCAIScratchpad** s) const { *s = &scratch; return true; }
    void SetAsDeathPersistent() { DeathPersistent = true; trace += 'd'; }
    void Kill(bool now) { check(!now, __LINE__); ++Killed; trace += 'K'; }
    CTCCreatureModeManager modes; CTCCarrying carrying; CCreatureActionBase* Current;
    bool IsPerformingAnyAction() const { return Current != 0; }
    const CCreatureActionBase* PeekPCurrentAction() const { return Current; }
    void FinishCurrentAction() { trace += 'f'; Current = 0; }
    bool GetTC(CTCCreatureModeManager** m) { *m = &modes; return true; }
    bool GetTC(CTCCarrying** c) { *c = &carrying; return true; }
};
class CAIStateGroup_StealStealableItem { public:
    CThingAICreature creature;
    CIntelligentPointer<CThing> PStealableItem; CIntelligentPointer<CThing> PPreviouslyCarriedItem;
    bool GotActionPoint; long ActionPointIndex; bool ReachedDestination; long PickUpActionID; bool NeedsCleanup;
    CThingAICreature& GetCreature() const { return const_cast<CThingAICreature&>(creature); }
    void OnReplace();
};
void CAIStateGroup_StealStealableItem::OnReplace()
{
    if (IS_THING_ALIVE(PStealableItem) && GotActionPoint) {
        CTCSingleUserActionPoints* action_points;
        PStealableItem->GetTC(&action_points);
        action_points->SetActionPointAsFree(ActionPointIndex);
        if (!ReachedDestination) {
            if (PickUpActionID != -1 && GetCreature().IsPerformingAnyAction()
                && GetCreature().PeekPCurrentAction()->GetActionID() == PickUpActionID)
                GetCreature().FinishCurrentAction();
            CThingAICreature& creature = GetCreature();
            CTCCreatureModeManager* modes;
            creature.GetTC(&modes);
            CTCCarrying* carrying;
            creature.GetTC(&carrying);
            if (carrying->IsCarryingThing(*PStealableItem)) {
                carrying->RemoveThing(*PStealableItem);
                modes->RemoveMode(MODE_CARRYING_GENERIC_BOX);
            }
            if (IS_THING_ALIVE(PPreviouslyCarriedItem)) {
                CDefPointer<const CCarryableDef> carryable_def;
                PPreviouslyCarriedItem->GetSubDef(&carryable_def);
                long active_slot = carryable_def->ActiveCarrySlot;
                long passive_slot = carryable_def->PassiveCarrySlot;
                CThing* slot_thing = carrying->GetThingInCarrySlot(passive_slot);
                if (slot_thing == PPreviouslyCarriedItem.Get())
                    carrying->RemoveThingInCarrySlot(passive_slot, false);
                carrying->PutThingInCarrySlot(*PPreviouslyCarriedItem, active_slot, true);
                if (PPreviouslyCarriedItem->HasTC(TCI_WEAPON))
                    modes->AddMode(MODE_CARRYING_WEAPON_STAND);
            }
        }
    }
    CThingAICreature& creature = GetCreature();
    const CTCAIScratchpad* scratchpad;
    creature.PeekTC(&scratchpad);
    if (ReachedDestination && scratchpad->KillSelfAndStealableItemAfterReachingDestination) {
        creature.SetAsDeathPersistent();
        GetCreature().Kill(false);
    }
    ActionPointIndex = -1;
    GotActionPoint = false;
    PickUpActionID = -1;
    PPreviouslyCarriedItem = CIntelligentPointer<CThing>(0);
    NeedsCleanup = false;
}

static void setup(CAIStateGroup_StealStealableItem& g, CThing& item, CThing& prev, CCarryableDef& def, CCreatureActionBase& act)
{
    item.Dead = false; item.Weapon = false; item.Carryable = &def; item.FreedPoint = 0;
    prev.Dead = false; prev.Weapon = true; prev.Carryable = &def;
    def.ActiveCarrySlot = 1; def.PassiveCarrySlot = 2; def.RefCount = 1; act.ID = 7;
    g.PStealableItem.Item = &item; g.PPreviouslyCarriedItem.Item = &prev;
    g.GotActionPoint = true; g.ActionPointIndex = 3; g.ReachedDestination = false; g.PickUpActionID = 7; g.NeedsCleanup = true;
    g.creature.Current = &act; g.creature.scratch.KillSelfAndStealableItemAfterReachingDestination = false; g.creature.DeathPersistent = false; g.creature.Killed = 0; g.creature.carrying.Carried = &item;
    for (int i = 0; i < 4; ++i) g.creature.carrying.Slots[i] = 0;
    g.creature.carrying.Slots[2] = &prev;
    trace = "";
}
int main()
{
    CThing item, prev; CCarryableDef def; CCreatureActionBase act;
    CAIStateGroup_StealStealableItem g;
    setup(g, item, prev, def, act); g.OnReplace();
    check(trace == "FfDRPSAr" && item.FreedPoint == 3 && def.RefCount == 1, __LINE__);
    check(g.creature.carrying.Slots[1] == &prev && g.creature.carrying.Slots[2] == 0, __LINE__);
    check(g.ActionPointIndex == -1 && !g.GotActionPoint && g.PickUpActionID == -1 && g.PPreviouslyCarriedItem.Item == 0, __LINE__);
    setup(g, item, prev, def, act); g.PickUpActionID = 9; prev.Weapon = false; g.creature.carrying.Slots[2] = 0; g.OnReplace();
    check(trace == "FDRSr", __LINE__);                                      // other action kept; not in the passive slot; no weapon
    setup(g, item, prev, def, act); g.GotActionPoint = false; g.OnReplace();
    check(trace == "" && g.PPreviouslyCarriedItem.Item == 0, __LINE__);      // no action point: only the reset
    setup(g, item, prev, def, act); item.Dead = true; g.OnReplace();
    check(trace == "" && g.ActionPointIndex == -1, __LINE__);
    setup(g, item, prev, def, act); g.ReachedDestination = true; g.OnReplace();
    check(trace == "F" && !g.NeedsCleanup, __LINE__);                       // destination reached: only the action point
    setup(g, item, prev, def, act); g.ReachedDestination = true; g.creature.scratch.KillSelfAndStealableItemAfterReachingDestination = true; g.OnReplace();
    check(trace == "FdK" && g.creature.DeathPersistent && g.creature.Killed == 1, __LINE__);   // reached + kill flag: dies death-persistent
    setup(g, item, prev, def, act); g.creature.scratch.KillSelfAndStealableItemAfterReachingDestination = true; g.OnReplace();
    check(trace == "FfDRPSAr" && g.creature.Killed == 0, __LINE__);         // kill flag but not reached: no kill
    printf("009644f0_TEST PASS\n");
    return 0;
}