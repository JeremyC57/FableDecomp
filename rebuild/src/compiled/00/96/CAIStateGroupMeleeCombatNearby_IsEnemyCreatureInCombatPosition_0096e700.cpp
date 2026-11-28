// 0096E700 CAIStateGroup_MeleeCombatNearby::IsEnemyCreatureInCombatPosition
// Leaf: ?IsEnemyCreatureInCombatPosition@CAIStateGroup_MeleeCombatNearby@@ABE_NXZ
// Flags:
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_C3DVECTOR_SQUARE_ORIGINAL_FORMAL
#define FABLE_RETAIL_C3DVECTOR_SQUARED2D_ONE_EXPRESSION_PASS23
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_DIAMETER
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_COMBATMANAGER
#define FABLE_RETAIL_CCOMBATMANAGER
#define FABLE_RETAIL_CCOMBATMANAGER_PREFERREDPOS
#define FABLE_RETAIL_CCOMBATMANAGER_OUTERRING
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_GOTOBED_WORLD
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CAISTATEGROUP_MELEECOMBATNEARBY_MOVE
#define FABLE_RETAIL_MELEECOMBAT_PREFERRED_RADIUS_PASS30
#include "engine/CThingAICreature.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CWorld.h"
#include "engine/CCombatManager.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroup_MeleeCombatNearby.h"

// Inferred: Widening each proximity result before the epsilon addition reproduces
// both x87 comparisons. The debug build does not identify the original cast spelling.
bool CAIStateGroup_MeleeCombatNearby::IsEnemyCreatureInCombatPosition() const
{
    if (GetCreature().IsInMeleeAsAttacker()) {
        const C3DVector pos = GetCombatManager().GetPreferredPos(GetCreature());
        const C3DVector to_preferred_pos = pos - GetCreature().GetPos();
        if (to_preferred_pos.GetSquaredMagnitude2D() <
            GFSqr(static_cast<double>(GetProximityToBeConsideredNearToPreferredPos()) + DELTA_NOTIONAL_ZERO))
            return true;
        const C3DVector& enemy_pos = PEnemy->GetPos();
        const C3DVector to_enemy_pos = enemy_pos - GetCreature().GetPos();
        if (!GetCombatManager().IsCreatureOnOuterRing(GetCreature()) &&
            to_enemy_pos.GetSquaredMagnitude2D() <
            GFSqr(static_cast<double>(GetProximityToBeConsideredNearToEnemyCreature()) + DELTA_NOTIONAL_ZERO))
            return true;
    }
    return false;
}
