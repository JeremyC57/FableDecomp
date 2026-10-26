// Behaviour test for 0x0097E140 GetPBestTargetToFaceForCreature: mode 25 + in melee as attacker + able to strafe -> the
// attacked creature from the combat manager; mode 25 otherwise -> the targeting component's best enemy (or null without
// one); any other mode or no mode manager -> null.
#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#endif
class CThing { public: int id; };
class CThingCreatureBase;
class CTCBase { public: int id; };

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;
};
const CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    SEntry* it = first;
    while (it != last && it->type < *type) ++it;
    return it;
}
class CModeBase {
public:
    long Type;
    virtual void v00() {}
    virtual long GetModeType() const { return Type; }
};
class CTCCreatureModeManager {
public:
    CTCBase base;
    CModeBase* Current;
    CModeBase& GetCurrentMode() const { return *Current; }
};
class CTCTargetingAI {
public:
    CThingCreatureBase* Best;
    virtual void v00() {} virtual void v01() {} virtual void v02() {} virtual void v03() {} virtual void v04() {} virtual void v05() {}
    virtual void v06() {} virtual void v07() {} virtual void v08() {} virtual void v09() {} virtual void v10() {} virtual void v11() {}
    virtual void v12() {} virtual void v13() {} virtual void v14() {} virtual void v15() {} virtual void v16() {} virtual void v17() {}
    virtual void v18() {} virtual void v19() {} virtual void v20() {} virtual void v21() {} virtual void v22() {} virtual void v23() {}
    virtual void v24() {} virtual void v25() {} virtual void v26() {} virtual void v27() {} virtual void v28() {}
    virtual CThingCreatureBase* GetPBestEnemyCreatureForAttack() const { return Best; }
};
class CAttackingCombatantInfo { public: CThingCreatureBase* Attacked; };
struct CCombatantInfo { CThingCreatureBase* Parent; CAttackingCombatantInfo AttackerInfo; };
static CCombatantInfo g_info;
static const CThingCreatureBase* g_infoQuery = 0;
class CCombatManager {
public:
    const CCombatantInfo& GetCombatantInfo(const CThingCreatureBase& creature) const { g_infoQuery = &creature; return g_info; }
};
static CCombatManager g_combatManager;
class CThingCreatureBase : public CThing {
public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    bool InMelee, InMeleeAsAttacker, AbleToStrafe;
    unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template<class T> const T* GetTC(long type) const
    {
        long key = type;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (const T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
    bool IsInMelee() const { return InMelee; }
    bool IsInMeleeAsAttacker() const { return InMeleeAsAttacker; }
    bool IsAbleToStrafe() const { return AbleToStrafe; }
    const CCombatManager& GetCombatManager() const { return g_combatManager; }
};

namespace {
CThing* __fastcall GetPBestTargetToFaceForCreature(const CThingCreatureBase& creature)
{
    if (creature.HasTC(0x31) && creature.GetTC<CTCCreatureModeManager>(0x31)->GetCurrentMode().GetModeType() == 25) {
        if (creature.IsInMelee() && creature.IsInMeleeAsAttacker() && creature.IsAbleToStrafe())
            return creature.GetCombatManager().GetCombatantInfo(creature).AttackerInfo.Attacked;
        if (creature.HasTC(8))
            return creature.GetTC<CTCTargetingAI>(8)->GetPBestEnemyCreatureForAttack();
    }
    return 0;
}
}

static int Fail() { std::printf("0097e140_TEST FAIL\n"); return 1; }

int main()
{
    CThingCreatureBase meleeTarget, bestEnemy, creature;
    CModeBase mode; mode.Type = 25;
    CTCCreatureModeManager modes; modes.Current = &mode;
    CTCTargetingAI targeting; targeting.Best = &bestEnemy;
    CInterfaceMap::SEntry entries[2];
    entries[0].type = 8;    entries[0].component = (CTCBase*)&targeting;
    entries[1].type = 0x31; entries[1].component = (CTCBase*)&modes;
    for (int i = 0; i < 9; ++i) creature.m_presentBits[i] = 0;
    creature.m_presentBits[0] = 1UL << 8;
    creature.m_presentBits[1] = 1UL << (0x31 - 32);
    creature.m_interfaces.first = entries;
    creature.m_interfaces.last = entries + 2;
    creature.InMelee = true; creature.InMeleeAsAttacker = true; creature.AbleToStrafe = true;
    g_info.Parent = &creature;
    g_info.AttackerInfo.Attacked = &meleeTarget;

    if (GetPBestTargetToFaceForCreature(creature) != &meleeTarget || g_infoQuery != &creature) return Fail();
    creature.AbleToStrafe = false;
    if (GetPBestTargetToFaceForCreature(creature) != &bestEnemy) return Fail();
    creature.AbleToStrafe = true; creature.InMeleeAsAttacker = false;
    if (GetPBestTargetToFaceForCreature(creature) != &bestEnemy) return Fail();
    creature.InMeleeAsAttacker = true; creature.InMelee = false;
    if (GetPBestTargetToFaceForCreature(creature) != &bestEnemy) return Fail();
    creature.m_presentBits[0] = 0;                       // no targeting component
    if (GetPBestTargetToFaceForCreature(creature) != 0) return Fail();
    creature.m_presentBits[0] = 1UL << 8;
    mode.Type = 24;                                      // not the combat mode
    if (GetPBestTargetToFaceForCreature(creature) != 0) return Fail();
    mode.Type = 25;
    creature.m_presentBits[1] = 0;                       // no mode manager
    if (GetPBestTargetToFaceForCreature(creature) != 0) return Fail();
    std::printf("0097e140_TEST PASS\n");
    return 0;
}