// Behaviour test for CAIStateGroup_MoveGoodsBase::Cleanup (0x009162F0): same control flow over a mock model.
#include <cstdio>
#include <cstring>
struct CTCBase { };
struct CTCPhysicsBase { bool moving; CTCPhysicsBase() : moving(false) { } void SetAsMoving(bool m) { moving = m; } };
struct CTCGoods : CTCBase { bool Dropped; CTCGoods() : Dropped(false) { } };
struct CThing;
struct CTCSolidToThing : CTCBase {
    CThing* notSolid[4]; int n;
    CTCSolidToThing() : n(0) { }
    void AddThingNotSolidTo(CThing* t) { notSolid[n++] = t; }
};
struct CThing {
    CTCPhysicsBase* PhysicsTC;
    CTCBase* tc[0x60];
    CTCSolidToThing added; int addCalls; char addedName[32];
    CThing() : PhysicsTC(0), addCalls(0) { std::memset(tc, 0, sizeof(tc)); addedName[0] = 0; }
    bool HasTC(long k) const { return tc[k] != 0; }
    template<class T> bool GetInterface(T** pp, long k) { if (tc[k]) { *pp = (T*)tc[k]; return true; } return false; }
    template<class T> T* GetTC(long k) { T* p; if (GetInterface(&p, k)) return p; return 0; }
    CTCBase* AddTC(const char* name, bool, void*) { ++addCalls; std::strcpy(addedName, name); tc[0x4d] = &added; return &added; }
};
struct CActionBase { long ID; long GetID() const { return ID; } };
struct CThingCreatureBase : CThing {
    CActionBase* PCurrentAction; bool performing; int finished;
    CThingCreatureBase() : PCurrentAction(0), performing(false), finished(0) { }
    bool IsPerformingAnyAction() { return performing; }
    void FinishCurrentAction() { ++finished; }
};
struct CTCCarrying : CTCBase {
    CThing* slot;
    CThing* GetThingInPrimarySlot() { return slot; }
    CThing* RemoveThingFromPrimarySlot(bool) { CThing* t = slot; slot = 0; return t; }
};
struct CTCCreatureModeManager : CTCBase { unsigned long removed; void RemoveMode(unsigned long m) { removed = m; } };
struct CAIBrain { CThingCreatureBase* PCreature; };
struct CState {
    CAIBrain* PBrain; bool DroppingGoods; long PickUpGoodsActionID; CThing* hero;
    void Cleanup()
    {
        if (PBrain->PCreature->IsPerformingAnyAction() && PBrain->PCreature->PCurrentAction->GetID() == PickUpGoodsActionID)
            PBrain->PCreature->FinishCurrentAction();
        CTCCarrying* carrying;
        PBrain->PCreature->GetInterface(&carrying, 0x46);
        if (carrying->GetThingInPrimarySlot()) {
            if (carrying->GetThingInPrimarySlot()->PhysicsTC)
                carrying->GetThingInPrimarySlot()->PhysicsTC->SetAsMoving(true);
            CTCGoods* goods = carrying->GetThingInPrimarySlot()->GetTC<CTCGoods>(0x47);
            if (goods)
                goods->Dropped = true;
            CTCSolidToThing* solid;
            if (carrying->GetThingInPrimarySlot()->HasTC(0x4d))
                solid = carrying->GetThingInPrimarySlot()->GetTC<CTCSolidToThing>(0x4d);
            else
                solid = (CTCSolidToThing*)carrying->GetThingInPrimarySlot()->AddTC("CTCSolidToThing", true, 0);
            if (solid) {
                solid->AddThingNotSolidTo(PBrain->PCreature);
                solid->AddThingNotSolidTo(hero);
            }
            carrying->RemoveThingFromPrimarySlot(false);
            CTCCreatureModeManager* modes;
            PBrain->PCreature->GetInterface(&modes, 0x31);
            modes->RemoveMode(6);
        }
        DroppingGoods = false;
    }
};
int main()
{
    CThingCreatureBase creature; CThing hero; CThing crate; CTCPhysicsBase phys; CTCGoods goods;
    CTCCarrying carrying; CTCCreatureModeManager modes; CActionBase action;
    modes.removed = 0; action.ID = 7; creature.PCurrentAction = &action; creature.performing = true;
    creature.tc[0x46] = &carrying; creature.tc[0x31] = &modes;
    crate.PhysicsTC = &phys; crate.tc[0x47] = &goods; carrying.slot = &crate;
    CAIBrain brain; brain.PCreature = &creature;
    CState s; s.PBrain = &brain; s.DroppingGoods = true; s.PickUpGoodsActionID = 7; s.hero = &hero;
    s.Cleanup();
    bool ok = creature.finished == 1 && phys.moving && goods.Dropped && crate.addCalls == 1
        && std::strcmp(crate.addedName, "CTCSolidToThing") == 0 && crate.added.n == 2
        && crate.added.notSolid[0] == &creature && crate.added.notSolid[1] == &hero
        && carrying.slot == 0 && modes.removed == 6 && !s.DroppingGoods;
    // second run: nothing carried, wrong action id -> only DroppingGoods cleared
    s.DroppingGoods = true; s.PickUpGoodsActionID = 8; modes.removed = 0;
    s.Cleanup();
    ok = ok && creature.finished == 1 && modes.removed == 0 && !s.DroppingGoods;
    // third run: solid component already present, no physics, no goods
    CThing crate2; CTCSolidToThing solid; crate2.tc[0x4d] = &solid; carrying.slot = &crate2;
    s.Cleanup();
    ok = ok && crate2.addCalls == 0 && solid.n == 2 && modes.removed == 6;
    if (!ok) { std::printf("009162f0_TEST FAIL\n"); return 1; }
    std::printf("009162f0_TEST PASS\n");
    return 0;
}