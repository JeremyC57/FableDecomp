// Select the best viable attacker within the attempted facing direction.
// Leaf: ?GetBestEnemyForCombatant@CCombatManager@@QBEPAVCThingCreatureBase@@ABV2@ABVC3DVector@@@Z
// Flags:
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_GETCOMBATTYPEDEF
#define FABLE_RETAIL_CTHINGCREATUREBASE_ISINMELEEASATTACKED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CCOMBATMANAGER
#define FABLE_RETAIL_CCOMBATMANAGER_GETCOMBATANTINFO_INLINE
#define FABLE_RETAIL_CCOMBATTYPEDEF
#define FABLE_RETAIL_CDEFENDINGCOMBATANTINFO_PRONG
#define FABLE_RETAIL_CTCTARGETED
#define FABLE_RETAIL_CTCTARGETED_PASS22_TARGETABLE
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_RB_INCREMENT_SIZE_OWNER_PASS27
#define FABLE_RETAIL_STLP_TREE_MFIND_NOINLINE
#define FABLE_RETAIL_STLP_TREE_CONSTFIND
#include "engine/CThingCreatureBase.h"
#include "engine/CTCTargeted.h"
#include "engine/CCombatTypeDef.h"
#include "engine/CCombatManager.h"

namespace NCreatureMode {
bool FABLE_FASTCALL IsAbleToBeAttacked(const CThing&);
}
namespace NBBBMaths {
bool FABLE_FASTCALL IsPosInFOVWithWeight(const C3DVector&,const C3DVector&,float,const C3DVector&,float&);
bool FABLE_FASTCALL IsInRange(const C3DVector&,const C3DVector&,float);
}

CThingCreatureBase* CCombatManager::GetBestEnemyForCombatant(const CThingCreatureBase& combatant, const C3DVector& attempted_facing_direction) const
{
    if (!combatant.IsInMeleeAsAttacked())
        return 0;

    float test_fov = 1.0f;
    const CCombatantInfo& combatant_info = GetCombatantInfo(combatant);
    const CCombatTypeDef& combat_def = combatant.GetCombatTypeDef();
    const CThingCreatureBase* best_enemy = 0;
    const std::map<unsigned __int64,CThingCreatureBase*>& attackers = combatant_info.DefenderInfo.Attackers;
    float best_weight = 0.0f;
    float closest_distance = 1.0e33f;
    for (std::map<unsigned __int64,CThingCreatureBase*>::const_iterator attackerit = attackers.begin(); attackerit != attackers.end(); ++attackerit) {
        const CThingCreatureBase& attacker = *attackerit->second;
        const CTCTargeted* ptc_targeted;
        if (attacker.PeekTC(&ptc_targeted) && ptc_targeted->IsTargetable() && NCreatureMode::IsAbleToBeAttacked(attacker)) {
            const C3DVector& attacker_pos = attacker.GetPos();
            float fov_weight;
            if (NBBBMaths::IsPosInFOVWithWeight(combatant.GetPos(), attempted_facing_direction, test_fov, attacker_pos, fov_weight) &&
                NBBBMaths::IsInRange(combatant.GetPos(), attacker.GetPos(), combat_def.FrontFarLimit) &&
                fov_weight > best_weight) {
                best_weight = fov_weight;
                best_enemy = &attacker;
            }
        }
    }
    if (best_enemy && !IsTargetOverriddenByAttemptedFacingDirection(combatant, *best_enemy, attempted_facing_direction)) {
        CThingCreatureBase* cast_of_greatness = const_cast<CThingCreatureBase*>(best_enemy);
        return cast_of_greatness;
    }
    return 0;
}