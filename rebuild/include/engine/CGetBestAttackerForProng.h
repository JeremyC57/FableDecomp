#ifndef FABLE_ENGINE_CGETBESTATTACKERFORPRONG_H
#define FABLE_ENGINE_CGETBESTATTACKERFORPRONG_H
#ifdef FABLE_RETAIL_COMBATRING_ASSIGN_PASS27
#include "engine/CDefendingCombatantInfo.h"
namespace {
class CGetBestAttackerForProng {
    const CDefendingCombatantInfo::CCombatRing& Ring;
    const CDefendingCombatantInfo::CCombatWheel& Wheel;
    const CDefendingCombatantInfo::CProng& Prong;
    const CThingCreatureBase& Defender;
    const CWorldMap& WorldMap;
    float BestCompareVal;
    CThingCreatureBase* PBestAttacker;
public:
    CGetBestAttackerForProng(const CWorldMap& wmap,const CDefendingCombatantInfo::CCombatWheel& wheel,
        const CDefendingCombatantInfo::CCombatRing& ring,const CDefendingCombatantInfo::CProng& prong,
        const CThingCreatureBase& defender):Ring(ring),Wheel(wheel),Prong(prong),Defender(defender),
        WorldMap(wmap),BestCompareVal(-1.0f),PBestAttacker(0){}
    void operator()(std::pair<CThingCreatureBase*,CAttackingCombatantInfo*>);
    CThingCreatureBase* GetPBestAttacker() const {return PBestAttacker;}
};
class CIsProngUsable {
public:
    // Inferred: the empty predicate passes an untouched byte at 006EBFA1. Value initialization without this constructor adds a zero store.
    CIsProngUsable() {}
    bool operator()(const CDefendingCombatantInfo::CProng&) const;
};
}
#endif
#endif
