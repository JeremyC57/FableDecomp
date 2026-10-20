// Behaviour test for 0x00848730 CCreatureAction_PickUpGoods::PerformAction: a live goods thing goes into the carry slot its
// carryable def names and a carrying-goods mode is pushed; dead or missing goods do nothing; the def reference is returned.
#include <cstdio>
struct CThing;
struct CCarryableDef { long RefCount; long CarrySlot; };
struct CDefPointer { CCarryableDef* Object; CDefPointer() : Object(0) { } ~CDefPointer() { if (Object) --Object->RefCount; } CCarryableDef* operator->() const { return Object; } };
struct CTCCarrying { CThing* thing; long slot; bool now; void PutThingInCarrySlot(CThing* t, long s, bool n) { thing = t; slot = s; now = n; } };
struct CMode { CThing* owner; CMode(CThing* o) : owner(o) { } };
struct CTCCreatureModeManager { CMode* last; int count; void AddMode(CMode* m) { delete last; last = m; ++count; } };
struct CThing {
    bool dead; CCarryableDef* def; CTCCarrying carrying; CTCCreatureModeManager modes;
    void GetSubDef(CDefPointer* out) { out->Object = def; ++def->RefCount; }
    bool GetCarrying(CTCCarrying** out) { *out = &carrying; return true; }
    bool GetModes(CTCCreatureModeManager** out) { *out = &modes; return true; }
};
struct CPtr { CThing* item; CThing* GetPItem() const { return item; } };
struct CAction {
    CPtr PCreature; CPtr PGoods;
    void PerformAction()
    {
        CThing* goods = PGoods.GetPItem();
        if (goods && !goods->dead) {
            CTCCarrying* carrying; PCreature.GetPItem()->GetCarrying(&carrying);
            CDefPointer carryDef; goods->GetSubDef(&carryDef);
            carrying->PutThingInCarrySlot(goods, carryDef->CarrySlot, true);
            CTCCreatureModeManager* modes; PCreature.GetPItem()->GetModes(&modes);
            modes->AddMode(new CMode(PCreature.GetPItem()));
        }
    }
};
int main()
{
    CCarryableDef def; def.RefCount = 1; def.CarrySlot = 3;
    CThing man; man.carrying.thing = 0; man.modes.last = 0; man.modes.count = 0;
    CThing goods; goods.dead = false; goods.def = &def;
    CAction a; a.PCreature.item = &man; a.PGoods.item = &goods;
    a.PerformAction();
    int ok = man.carrying.thing == &goods && man.carrying.slot == 3 && man.carrying.now && man.modes.count == 1 && man.modes.last->owner == &man && def.RefCount == 1;
    goods.dead = true; a.PerformAction(); ok = ok && man.modes.count == 1;
    a.PGoods.item = 0; a.PerformAction(); ok = ok && man.modes.count == 1;
    delete man.modes.last;
    if (!ok) { std::printf("00848730_TEST FAIL\n"); return 1; }
    std::printf("00848730_TEST PASS\n");
    return 0;
}
