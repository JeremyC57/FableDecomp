#include <cstdio>
// Behaviour test for CTCHeroRangedAttacks::IsAbleToUseRangedWeapon (0x00789E60): same model and body as src/,
// with the out-of-line callees (LowerBound, GetPlayer, GetThingInPrimarySlot, IsProjectileWeapon, GetCurrentMode) stubbed.
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const
    {
        SEntry* it = first;
        while (it != last && it->type < *type) ++it;
        return it;
    }
};
struct CPlayer { bool m_controlsLocked; };
struct CThing {
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char m_flags91;
    CPlayer* player;
    CPlayer* GetPlayer() { return player; }
    bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    CTCBase* GetTC(long type) const
    {
        long key = type;
        const CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        CInterfaceMap::SEntry* end = map->last;
        return (it == end || type < it->type ? end : it)->component;
    }
    template<class T> bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CCreatureActionBase { long m_type; long m_elapsed; };
struct CThingCreatureBase { CCreatureActionBase* PCurrentAction; };
struct CTCRangedBlocker { bool m_blocking; };
struct CTCCarrying { CThing* item; CThing* GetThingInPrimarySlot() { return item; } };
struct CTCWeapon { bool projectile; bool IsProjectileWeapon() { return projectile; } };
struct CMode { long type; virtual long GetType() { return type; } };
struct CTCModeOwner { CMode* mode; CMode* GetCurrentMode() { return mode; } };

struct CTCHeroRangedAttacksMethods {
    CThing* ParentThing;
    CThingCreatureBase* PParentCreature;
    CThing* GetOwner() const { return ParentThing; }
    bool IsAbleToUseRangedWeapon();
};

bool CTCHeroRangedAttacksMethods::IsAbleToUseRangedWeapon()
{
    {
        CThing* thing = GetOwner();
        CTCRangedBlocker* blocker;
        if (thing->GetInterface(&blocker, 0xa7) && blocker->m_blocking)
            return false;
        if (thing->GetPlayer()->m_controlsLocked)
            return false;
    }
    CCreatureActionBase* action = PParentCreature->PCurrentAction;
    if (action && action->m_type == 100 && action->m_elapsed >= 250)
        return false;

    CTCCarrying* carrying;
    GetOwner()->GetInterface(&carrying, 0x46);
    CThing* item = carrying->GetThingInPrimarySlot();
    CTCWeapon* weapon;
    if (!item || (item->m_flags91 & 1) || !item->GetInterface(&weapon, 0x48) || !weapon->IsProjectileWeapon())
        return false;

    CTCModeOwner* modeOwner;
    GetOwner()->GetInterface(&modeOwner, 0x31);
    long type = modeOwner->GetCurrentMode()->GetType();
    if (type == 25 || type == 70)
        return true;
    return false;
}

static void SetBit(CThing& t, long type) { t.m_presentBits[type >> 5] |= 1UL << (type & 31); }

int main()
{
    CPlayer player = { false };
    CTCRangedBlocker blocker = { false };
    CTCWeapon weapon = { true };
    CThing item = {};
    SetBit(item, 0x48);
    CInterfaceMap::SEntry itemEntries[1] = { { 0x48, (CTCBase*)&weapon } };
    item.m_interfaces.first = itemEntries; item.m_interfaces.last = itemEntries + 1;
    CTCCarrying carrying = { &item };
    CMode mode; mode.type = 25;
    CTCModeOwner modeOwner = { &mode };
    CThing hero = {};
    hero.player = &player;
    SetBit(hero, 0x31); SetBit(hero, 0x46); SetBit(hero, 0xa7);
    CInterfaceMap::SEntry entries[3] = { { 0x31, (CTCBase*)&modeOwner }, { 0x46, (CTCBase*)&carrying },
                                         { 0xa7, (CTCBase*)&blocker } };
    hero.m_interfaces.first = entries; hero.m_interfaces.last = entries + 3;
    CCreatureActionBase action = { 100, 10 };
    CThingCreatureBase creature = { &action };
    CTCHeroRangedAttacksMethods tc = { &hero, &creature };

    int fails = 0;
    if (!tc.IsAbleToUseRangedWeapon()) ++fails;
    mode.type = 70; if (!tc.IsAbleToUseRangedWeapon()) ++fails;
    mode.type = 3; if (tc.IsAbleToUseRangedWeapon()) ++fails;
    mode.type = 25;
    blocker.m_blocking = true; if (tc.IsAbleToUseRangedWeapon()) ++fails;
    hero.m_presentBits[5] = 0; if (!tc.IsAbleToUseRangedWeapon()) ++fails;   // blocker absent: ignored
    player.m_controlsLocked = true; if (tc.IsAbleToUseRangedWeapon()) ++fails;
    player.m_controlsLocked = false;
    action.m_elapsed = 250; if (tc.IsAbleToUseRangedWeapon()) ++fails;
    action.m_type = 99; if (!tc.IsAbleToUseRangedWeapon()) ++fails;
    creature.PCurrentAction = 0; if (!tc.IsAbleToUseRangedWeapon()) ++fails;
    item.m_flags91 = 1; if (tc.IsAbleToUseRangedWeapon()) ++fails;
    item.m_flags91 = 0;
    weapon.projectile = false; if (tc.IsAbleToUseRangedWeapon()) ++fails;
    weapon.projectile = true;
    item.m_presentBits[2] = 0; if (tc.IsAbleToUseRangedWeapon()) ++fails;
    SetBit(item, 0x48);
    carrying.item = 0; if (tc.IsAbleToUseRangedWeapon()) ++fails;
    if (fails) { std::printf("00789e60_TEST FAIL\n"); return 1; }
    std::printf("00789e60_TEST PASS\n");
    return 0;
}
