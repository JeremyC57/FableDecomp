// Behaviour test for 0x00663990 CThingCreatureBase::CreateWeaponForThingFromDef(CThing& thing, long defIndex).
#include <stdio.h>
class CCarryableDef { public: long ActiveCarrySlot, SecondaryActiveCarrySlot, PassiveCarrySlot; long RefCount; };
class CThing;
class CTCInventoryItem { public: bool DefaultItem; int Added; };
class CTCInventoryWeapons { public: long ActiveWeapon; };
class CTCCarrying { public:
    CThing* Slots[8]; long RemovedSlot;
    void RemoveThingInCarrySlot(long slot, bool) { RemovedSlot = slot; Slots[slot] = 0; }
    CThing* GetThingInCarrySlot(long slot) const { return Slots[slot]; }
    void PutThingInCarrySlot(CThing& thing, long slot, bool) { Slots[slot] = &thing; }
};
class CThing { public:
    bool Dead, CreatedByOwner; unsigned short DefGlobalIndex; CTCInventoryWeapons* Weapons; CTCInventoryItem* Item; CTCCarrying* Carrying; CCarryableDef* Carryable;
};
static CThing GObject; static CTCInventoryItem GItem; static CCarryableDef GDef; static int GCreated;
static CThing* CreateObject(long defIndex) { ++GCreated; GObject.DefGlobalIndex = (unsigned short)defIndex; return &GObject; }
static void CreateWeaponForThingFromDef(CThing& thing, long defIndex)
{
    if (defIndex == 0) return;
    CThing* object = CreateObject(defIndex);
    if (thing.Weapons) {
        CTCInventoryItem* item = object->Item;
        if (item) { item->DefaultItem = true; ++item->Added; thing.Weapons->ActiveWeapon = object->DefGlobalIndex; }
    } else {
        CTCCarrying* carrying = thing.Carrying;
        CCarryableDef* def = object->Carryable; ++def->RefCount;
        carrying->RemoveThingInCarrySlot(def->PassiveCarrySlot, false);
        long slot = def->ActiveCarrySlot;
        CThing* held = carrying->GetThingInCarrySlot(slot);
        if (held && !held->Dead) slot = def->SecondaryActiveCarrySlot;
        carrying->PutThingInCarrySlot(*object, slot, false);
        object->CreatedByOwner = true;
        --def->RefCount;
    }
}
int main()
{
    GDef.ActiveCarrySlot = 1; GDef.SecondaryActiveCarrySlot = 2; GDef.PassiveCarrySlot = 3; GDef.RefCount = 1;
    GObject.Item = &GItem; GObject.Carryable = &GDef;
    CTCInventoryWeapons weapons = { 0 };
    CThing hero = { false, false, 0, &weapons, 0, 0, 0 };
    CreateWeaponForThingFromDef(hero, 0);
    int ok = GCreated == 0;
    CreateWeaponForThingFromDef(hero, 42);
    ok = ok && GCreated == 1 && GItem.DefaultItem && GItem.Added == 1 && weapons.ActiveWeapon == 42;
    CTCCarrying carrying; for (int i = 0; i < 8; ++i) carrying.Slots[i] = 0; carrying.RemovedSlot = -1;
    CThing living = { false, false, 0, 0, 0, 0, 0 };
    CThing creature = { false, false, 0, 0, 0, &carrying, 0 };
    carrying.Slots[3] = &living;
    CreateWeaponForThingFromDef(creature, 7);
    ok = ok && carrying.RemovedSlot == 3 && carrying.Slots[3] == 0 && carrying.Slots[1] == &GObject && GObject.CreatedByOwner && GDef.RefCount == 1;
    carrying.Slots[1] = &living;
    CreateWeaponForThingFromDef(creature, 7);
    ok = ok && carrying.Slots[2] == &GObject && carrying.Slots[1] == &living;
    living.Dead = true; carrying.Slots[2] = 0;
    CreateWeaponForThingFromDef(creature, 7);
    ok = ok && carrying.Slots[1] == &GObject && carrying.Slots[2] == 0;
    printf(ok ? "00663990_TEST PASS\n" : "00663990_TEST FAIL\n");
    return ok ? 0 : 1;
}