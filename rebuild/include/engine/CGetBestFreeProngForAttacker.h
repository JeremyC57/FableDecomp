#ifndef FABLE_ENGINE_CGETBESTFREEPRONGFORATTACKER_H
#define FABLE_ENGINE_CGETBESTFREEPRONGFORATTACKER_H
#ifdef FABLE_RETAIL_FREE_PRONG_PASS26
#include "engine/CDefendingCombatantInfo.h"
class CWorldMap;
class CAttackingCombatantInfo;
class CThingCreatureBase;
namespace {
// Ego_d and FableWin combat_manager.obj; native 006EC297 constructs these eight members.
class CGetBestFreeProngForAttacker {
public:
    CGetBestFreeProngForAttacker(const CWorldMap& world_map,
        const CDefendingCombatantInfo::CCombatWheel& wheel,
        const CDefendingCombatantInfo::CCombatRing& ring,
        const CThingCreatureBase& attacker, const CThingCreatureBase& defender,
        const CAttackingCombatantInfo& attacker_combat_info)
        : WorldMap(world_map), Attacker(attacker), Defender(defender),
          AttackerCombatInfo(attacker_combat_info), Ring(ring), Wheel(wheel),
          BestCompareVal(-1.0f), PBestProng(0) {}
    void operator()(CDefendingCombatantInfo::CProng& prong);
    CDefendingCombatantInfo::CProng* GetPBestProng() const { return PBestProng; }
private:
    const CWorldMap& WorldMap;
    const CThingCreatureBase& Attacker;
    const CThingCreatureBase& Defender;
    const CAttackingCombatantInfo& AttackerCombatInfo;
    const CDefendingCombatantInfo::CCombatRing& Ring;
    const CDefendingCombatantInfo::CCombatWheel& Wheel;
    float BestCompareVal;
    CDefendingCombatantInfo::CProng* PBestProng;
};
}
#endif
#endif
