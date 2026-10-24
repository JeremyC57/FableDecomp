// Behaviour test for 007034d0 (CThingObject::WorldUpdateWeaponTrails): with a weapon-trail component the trails update
// and the appearance refreshes (reason 4), except for a melee weapon that is carryable and not being carried.
#include <cstdio>

class CTCBase;
class CTCCarrying;
static int gTrailUpdates = 0;
static long gAppearanceReason = -1;

class CTCWeaponTrail {
public:
    void UpdateWeaponTrails() { ++gTrailUpdates; }       // 0x0076EFA0 stand-in
};
class CTCWeapon {
public:
    bool Melee;
    bool IsMeleeWeapon() const { return Melee; }        // 0x005DAA00 stand-in
};
class CTCCarryable {
public:
    CTCCarrying* PCarryingTC;
};

enum ETCInterfaceType { TC_CARRYABLE = 0x47, TC_WEAPON = 0x48, TC_WEAPON_TRAIL = 0x4c };

class CInterfaceMap {
public:
    struct SEntry {
        long type;
        CTCBase* component;
    };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long& type)                // stand-ins for 0x0040F020 / 0x004365B0
    {
        SEntry* it = first;
        while (it != last && it->type < type)
            ++it;
        return it;
    }
    const SEntry* LowerBound(const long& type) const
    {
        const SEntry* it = first;
        while (it != last && it->type < type)
            ++it;
        return it;
    }
};

class CThing {
public:
    unsigned long PresentInterfaces[9];
    CInterfaceMap Interfaces;
    bool HasTC(long type) const
    {
        if (PresentInterfaces[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    CTCBase* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* m = &Interfaces;
        CInterfaceMap::SEntry* it = m->LowerBound(key);
        if (it == m->last || type < it->type)
            it = m->last;
        return it->component;
    }
    const CTCBase* PeekTC(long type) const
    {
        long key = type;
        const CInterfaceMap* m = &Interfaces;
        const CInterfaceMap::SEntry* it = m->LowerBound(key);
        if (it == m->last || type < it->type)
            it = m->last;
        return it->component;
    }
    void UpdateGraphicAppearance(long reason) { gAppearanceReason = reason; }   // 0x004C7590 stand-in
};

class CThingObject : public CThing {
public:
    void WorldUpdateWeaponTrails()
    {
        if (HasTC(TC_WEAPON_TRAIL)) {
            CTCWeaponTrail* trail = (CTCWeaponTrail*)GetTC(TC_WEAPON_TRAIL);
            if (HasTC(TC_WEAPON) && ((const CTCWeapon*)PeekTC(TC_WEAPON))->IsMeleeWeapon() && HasTC(TC_CARRYABLE) &&
                ((const CTCCarryable*)PeekTC(TC_CARRYABLE))->PCarryingTC == 0)
                return;
            trail->UpdateWeaponTrails();
            UpdateGraphicAppearance(4);
        }
    }
};

static int Run(CThingObject& object, unsigned long bits)
{
    object.PresentInterfaces[2] = bits;
    gTrailUpdates = 0;
    gAppearanceReason = -1;
    object.WorldUpdateWeaponTrails();
    if (gTrailUpdates == 1 && gAppearanceReason == 4)
        return 1;
    if (gTrailUpdates == 0 && gAppearanceReason == -1)
        return 0;
    return -1;
}

int main()
{
    CTCWeaponTrail trail;
    CTCWeapon weapon;
    CTCCarryable carryable;
    CInterfaceMap::SEntry entries[3];
    entries[0].type = TC_CARRYABLE;
    entries[0].component = (CTCBase*)&carryable;
    entries[1].type = TC_WEAPON;
    entries[1].component = (CTCBase*)&weapon;
    entries[2].type = TC_WEAPON_TRAIL;
    entries[2].component = (CTCBase*)&trail;
    CThingObject object;
    for (int i = 0; i < 9; ++i)
        object.PresentInterfaces[i] = 0;
    object.Interfaces.first = entries;
    object.Interfaces.last = entries + 3;
    const unsigned long carryBit = 1UL << 7, weaponBit = 1UL << 8, trailBit = 1UL << 12;
    weapon.Melee = true;
    carryable.PCarryingTC = 0;
    int ok = 1;
    if (Run(object, carryBit | weaponBit) != 0)
        ok = 0;                                          // no trail component
    if (Run(object, trailBit) != 1)
        ok = 0;                                          // trail only
    if (Run(object, trailBit | weaponBit) != 1)
        ok = 0;                                          // melee weapon, not carryable
    if (Run(object, trailBit | weaponBit | carryBit) != 0)
        ok = 0;                                          // melee weapon lying free: no trails
    carryable.PCarryingTC = (CTCCarrying*)&entries[0];
    if (Run(object, trailBit | weaponBit | carryBit) != 1)
        ok = 0;                                          // melee weapon being carried
    carryable.PCarryingTC = 0;
    weapon.Melee = false;
    if (Run(object, trailBit | weaponBit | carryBit) != 1)
        ok = 0;                                          // ranged weapon
    if (!ok) {
        std::printf("007034d0_TEST FAIL\n");
        return 1;
    }
    std::printf("007034d0_TEST PASS\n");
    return 0;
}