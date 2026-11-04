// CAIStateGroup_MeleeCombatNearbyBase::IsRunnable() -- latch the creature's best live enemy into PEnemy, then run while still runnable and the creature can navigate to the enemy
// Grade: DIFFER(305v295) -- dl=3: the by-value inline GetProximityForNavigation() temporary (FableWin QBEMXZ) is stored into the escaped GetTC key slot (dead store before GetPos); named/const/pointer/scope forms identical; only the PDB-contradicting const float& getter matches (not acceptable)
// Leaf: ?IsRunnable@CAIStateGroup_MeleeCombatNearbyBase@@MAE_NXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0093D270, 295 bytes; vtable slot 11 (Ego_r.pdb / FableWin.pdb
//   ?IsRunnable@CAIStateGroup_MeleeCombatNearbyBase@@MAE_NXZ, protected virtual).
// Callees: CVectorMap LowerBound const 0x004365B0 (PeekTC<const CTCTargetingBase>, type 8) and non-const 0x0040F020
//   (GetTC<CTCCreatureNavigation>, type 7), CTCTargetingBase::GetPBestEnemyCreatureForAttack() const (slot 29, +0x74), the
//   converting CIntelligentPointer<CThingCreatureBase> temporary (vtable 0x0123E7D8; 0x00A01B10 / 0x00A01B90 / 0x00A01B50 /
//   0x00A01C10) assigned through the inline operator=, own slot 14 IsStillRunnable(bool) const (+0x38, 0x0093D800),
//   CThingCreatureBase::GetCombatManager 0x00661ED0, CCombatManager::GetProximityForNavigation() const (inline, +0x28),
//   CThing::GetPos 0x004C73D0 (x2), CTCCreatureNavigation::IsAbleToNavigateFromPosToPos 0x006CB620.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CCOMBATMANAGER
#define FABLE_RETAIL_CCOMBATMANAGER_PROXIMITY
#define FABLE_RETAIL_CTCCREATURENAVIGATION
#define FABLE_RETAIL_CTCCREATURENAVIGATION_NAVIGABLE
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CTCTargetingBase.h"
#include "engine/CTCCreatureNavigation.h"
#include "engine/CCombatManager.h"

class CAIStateGroup_MeleeCombatNearbyBase : public CAIStateGroupBase {
protected:
    virtual bool IsRunnable();                                       // slot 11
    virtual bool IsStillRunnable(bool starting) const;               // slot 14 (+0x38) 0x0093D800
    CIntelligentPointer<CThingCreatureBase> PEnemy;                  // +0x14
};

bool CAIStateGroup_MeleeCombatNearbyBase::IsRunnable()
{
    const CTCTargetingBase* targeting;
    PBrain->Creature->PeekTC(&targeting);
    CThingCreatureBase* enemy = targeting->GetPBestEnemyCreatureForAttack();
    if (enemy != 0 && !enemy->IsDead()) {
        PEnemy = enemy;
        if (IsStillRunnable(false)) {
            CThingAICreature* creature = PBrain->Creature;
            CTCCreatureNavigation* navigation;
            creature->GetTC(&navigation);
            CCombatManager& manager = creature->GetCombatManager();
            CThingCreatureBase* target = PEnemy.Get();
            CThingAICreature* self = PBrain->Creature;
            if (navigation->IsAbleToNavigateFromPosToPos(self->GetPos(), target->GetPos(), manager.GetProximityForNavigation()))
                return true;
        }
    }
    return false;
}
