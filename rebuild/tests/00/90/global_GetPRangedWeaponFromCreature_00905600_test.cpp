// Behaviour test for 0x00905600 GetPRangedWeaponFromCreature: carrying component, live item, weapon component,
// projectile test; null on every miss.
#include <cstdio>

enum ETCInterfaceType { TC_CARRYING = 0x46, TC_WEAPON = 0x48 };
struct CTCBase { };
struct CInterfaceMap {
    struct SEntry { ETCInterfaceType type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type) const
    {
        SEntry* it = first;
        while (it != last && it->type < type)
            ++it;
        return it;
    }
    SEntry* Find(long type) const
    {
        SEntry* it = LowerBound((ETCInterfaceType)type);
        return (it == last || type < it->type) ? last : it;
    }
};
class CThing {
public:
    unsigned int UsingComponents[9];
    CInterfaceMap Components;
    unsigned char Dead : 1;
    bool HasTC(ETCInterfaceType type) const
    {
        if (UsingComponents[type >> 5] & (1U << (type & 31)))
            return true;
        return false;
    }
    template<class T> bool PeekTC(const T** pp) const
    {
        if (HasTC(T::StatGetInterfaceType())) {
            *pp = (const T*)Components.Find(T::StatGetInterfaceType())->component;
            return true;
        }
        return false;
    }
};
class CThingCreatureBase : public CThing { };
inline bool IsThingAlive(const CThing* thing)
{
    if (thing && !thing->Dead)
        return true;
    return false;
}
class CTCWeapon : public CTCBase {
public:
    bool projectile;
    static ETCInterfaceType StatGetInterfaceType() { return TC_WEAPON; }
    bool IsProjectileWeapon() const { return projectile; }
};
class CTCCarrying : public CTCBase {
public:
    CThing* primary;
    static ETCInterfaceType StatGetInterfaceType() { return TC_CARRYING; }
    CThing* GetThingInPrimarySlot() const { return primary; }
};

CThing* GetPRangedWeaponFromCreature(const CThingCreatureBase& creature)
{
    const CTCCarrying* carrying;
    if (creature.PeekTC(&carrying)) {
        CThing* item = carrying->GetThingInPrimarySlot();
        const CTCWeapon* weapon;
        if (IsThingAlive(item) && item->PeekTC(&weapon) && weapon->IsProjectileWeapon())
            return item;
    }
    return 0;
}

static void Init(CThing& t)
{
    for (int i = 0; i < 9; ++i)
        t.UsingComponents[i] = 0;
    t.Components.first = t.Components.last = 0;
    t.Dead = 0;
}

int main()
{
    int ok = 1;
    CTCCarrying carrying;
    CTCWeapon weapon;
    weapon.projectile = true;
    CInterfaceMap::SEntry centries[1] = { { TC_CARRYING, &carrying } };
    CInterfaceMap::SEntry wentries[1] = { { TC_WEAPON, &weapon } };
    CThingCreatureBase creature;
    Init(creature);
    CThing bow;
    Init(bow);
    carrying.primary = &bow;
    if (GetPRangedWeaponFromCreature(creature) != 0) ok = 0;          // no carrying component
    creature.UsingComponents[TC_CARRYING >> 5] |= 1U << (TC_CARRYING & 31);
    creature.Components.first = centries;
    creature.Components.last = centries + 1;
    if (GetPRangedWeaponFromCreature(creature) != 0) ok = 0;          // item has no weapon component
    bow.UsingComponents[TC_WEAPON >> 5] |= 1U << (TC_WEAPON & 31);
    bow.Components.first = wentries;
    bow.Components.last = wentries + 1;
    if (GetPRangedWeaponFromCreature(creature) != &bow) ok = 0;
    weapon.projectile = false;
    if (GetPRangedWeaponFromCreature(creature) != 0) ok = 0;          // melee weapon
    weapon.projectile = true;
    bow.Dead = 1;
    if (GetPRangedWeaponFromCreature(creature) != 0) ok = 0;          // dead item
    bow.Dead = 0;
    carrying.primary = 0;
    if (GetPRangedWeaponFromCreature(creature) != 0) ok = 0;          // empty slot
    if (!ok) { std::printf("00905600_TEST FAIL\n"); return 1; }
    std::printf("00905600_TEST PASS\n");
    return 0;
}