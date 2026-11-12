// CCreatureAction_GenericFireProjectileWeapon::FinishAction() -- fire if the action has not yet performed; kill every live dependent thing; when the creature is attacking, ready the projectile of every attacker of its target (a live primary-slot item with a projectile weapon component); then the base FinishAction
// Grade: DIFFER(ours v retail) -- DIFFER(412v412), ported onto the shared headers; residue = the LowerBound key-address scratch register in both GetTC lookups of the attackers loop (retail lea esi,[eax+44h] then lea eax,[esp+10h] / lea ecx first in the weapon lookup; ours lea ecx / lea edx), 2 x 2 instructions. Tried: GetTC(id,&p) form, the Find-based default GetTC (+FORCEINLINE_FIND, ORIGINAL_FIND, GETTC_ORIGINAL_CHAIN), a named attacker local, hoisted carrying/weapon locals, map-before-key GetTC body order (probe header); none moves it. Next: c2prio --rotation on the two lookups.
// Leaf: ?FinishAction@CCreatureAction_GenericFireProjectileWeapon@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008BF500, 412 bytes, plain `ret` (vtable 0x0127F384 slot 4; Ego_r.pdb name above).
// Callees: CCreatureActionBase::HasPerformedAction 0x006925A0, PerformAction (vtable slot 3, 0x008BF280; the draft's "Abort" was this
//   slot), CCreatureActionBase::GetDependentThings (inline) + GetPItem 0x00A01B50 + CThing::Kill 0x004C9B80,
//   CThingCreatureBase::GetCombatManager 0x00661ED0, CCombatManager::IsCreatureAttacking 0x006E92C0 / GetCombatantInfo 0x006E9360,
//   CThing::GetTC<CTCCarrying> / <CTCWeapon> (expanded; CVectorMap::LowerBound 0x0040F020), CTCCarrying::GetThingInPrimarySlot
//   0x006A1440, CTCWeapon::IsProjectileWeapon 0x005DAA40 / ReadyProjectile 0x005DA4E0, CCombatActionBase::FinishAction 0x00859A20.
// Shape: on the shared headers (CCreatureActionBase FULL_MEMBERS, CCombatActionBase MELEE + FINISHACTION, CCombatantInfo's
//   std::map Attackers walked with the STLport iterator). The port fixed the earlier esi/edi swap and the i*8 CSE.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CCREATUREACTIONBASE_GETDEPENDENTTHINGS
#define FABLE_RETAIL_CCREATUREACTIONBASE_HASPERFORMEDACTION
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
#define FABLE_RETAIL_CCOMBATACTIONBASE_FINISHACTION
#define FABLE_RETAIL_CCREATUREACTION_GENERICFIREPROJECTILEWEAPON
#define FABLE_RETAIL_CCOMBATMANAGER_ISATTACKING
#define FABLE_RETAIL_CTCWEAPON
#define FABLE_RETAIL_CTCWEAPON_READYPROJECTILE
#include "rebuild_abi.h"
#include "engine/ETCInterfaceType.h"
#include "engine/C3DVector.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CDefPointer.h"
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"
#include "engine/ThingAlive.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CTCCarrying.h"
#include "engine/CTCWeapon.h"
#include "engine/CCombatManager.h"
#include "engine/CCombatantInfo.h"
#include "engine/CCreatureAction_GenericFireProjectileWeapon.h"

void CCreatureAction_GenericFireProjectileWeapon::FinishAction()
{
    if (!HasPerformedAction())
        PerformAction();
    for (unsigned long i = 0; i < GetDependentThings().size(); ++i) {
        if (GetDependentThings()[i] != 0)
            GetDependentThings()[i]->Kill(true);
    }
    CCombatManager& combat_manager = PCreature->GetCombatManager();
    if (combat_manager.IsCreatureAttacking(*PCreature)) {
        const CCombatantInfo& target_info = combat_manager.GetCombatantInfo(*combat_manager.GetCombatantInfo(*PCreature).AttackerInfo.Attacked);
        for (std::map<unsigned __int64, CThingCreatureBase*>::const_iterator it = target_info.DefenderInfo.Attackers.begin();
             it != target_info.DefenderInfo.Attackers.end(); ++it) {
            CTCCarrying* carrying;
            if (it->second->GetTC(&carrying)) {
                CThing* item = carrying->GetThingInPrimarySlot();
                CTCWeapon* weapon;
                if (IS_THING_ALIVE(item) && item->GetTC(&weapon) && weapon->IsProjectileWeapon())
                    weapon->ReadyProjectile();
            }
        }
    }
    CCombatActionBase::FinishAction();
}
