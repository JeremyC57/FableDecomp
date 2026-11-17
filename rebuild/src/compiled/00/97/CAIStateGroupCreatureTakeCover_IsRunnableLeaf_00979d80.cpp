// CAIStateGroup_CreatureTakeCover::IsRunnableLeaf() -- gate on the projectile-carrying module, then require a live target outside melee, a projectile weapon, and the cover-base leaf check.
// Leaf: ?IsRunnableLeaf@CAIStateGroup_CreatureTakeCover@@UAE_NXZ
// Retail: Fable.exe (TLC, VC7.1), @0x00979D80, 258 bytes.
// Identity: Ego_r.pdb 0x0099EAA0 / FableWin.pdb 0x02C1CA30 ?IsRunnableLeaf@CAIStateGroup_CreatureTakeCover@@UAE_NXZ (public virtual; the
//   landed leaf was the QAE spelling: access-only self fix). Name fix (NF56, round 7, queue q10460): the qualified base call 0x00970F30 is
//   CAIStateGroup_CreatureUseCoverBase::IsRunnableLeaf, ?IsRunnableLeaf@CAIStateGroup_CreatureUseCoverBase@@MAE_NXZ (Ego_r.pdb 0x00995CE0 /
//   FableWin.pdb 0x02B85370; retail vtable 0x0128D31C slot 15); was the made-up IsCoverBaseRunnable on a local model of the shared-header
//   class CAIStateGroup_CreatureTakeCover. The row now sits on the shared headers: the retail class of engine/CAIStateGroup_CreatureTakeCover.h
//   (overlay opt-in _ISRUNNABLELEAF) on engine/CAIStateGroup_CreatureUseCoverBase.h, engine/CAIStateGroupBase.h (GetWorld 0x008FCFC0,
//   PBrain +4), engine/CAIBrain.h (Creature +0x18), engine/CThing.h's PeekTC<T> (const CVectorMap::LowerBound 0x004365B0) and
//   engine/ThingAlive.h's IS_THING_ALIVE, in place of the local CThing / CVectorMap / CAIStateGroupBase / CTCCarrying / CTCWeapon models.
// Callees: NCreatureAI::CIsRunnableModuleCarryingProjectileWeapon @0x00982890 (temporary vtable 0x0128FFD4; the module models below
//   are kept as landed: no shared header declares them); CBaseIntelligentPointer::GetPItem @0x00A01B50; IsInMeleeWithCombatWheelAttackers
//   @0x00979910 (anonymous namespace, NF59); CTCCarrying::GetThingInPrimarySlot @0x006A1440; CTCWeapon::IsProjectileWeapon @0x005DAA40.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTCWEAPON
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_SPEECH
#define FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE
#define FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE_VSLOTS
#define FABLE_RETAIL_CAISTATEGROUP_CREATUREUSECOVERBASE
#define FABLE_RETAIL_CAISTATEGROUP_CREATUREUSECOVERBASE_COVERLIST
#define FABLE_RETAIL_CAISTATEGROUP_CREATURETAKECOVER_ISRUNNABLELEAF
#include "fable_compiler.h"
#include "rebuild_abi.h"
#include <stddef.h>
#include "stlport/fable_containers.h"
#include "engine/fable_ai_action_enums.h"
#include "engine/C3DVector.h"
#include "engine/CIntelligentPointer.h"
#include "engine/ThingAlive.h"
#include "engine/CThingAICreature.h"
#include "engine/CTCCarrying.h"
#include "engine/CTCWeapon.h"
#include "engine/CWorld.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroup_CreatureTakeCover.h"

// NF59 (round 7, queue q10527): Ego_r.pdb 0x0099E590 + FableWin.pdb 0x02C1C760 spell 0x00979910 as the file-local (anonymous-namespace)
//   __fastcall ?IsInMeleeWithCombatWheelAttackers@?A0x...@@YI_NABVCThingCreatureBase@@@Z of the TakeCover TU (the landed self row defines it
//   so); the global (CThing*) declaration bound a made-up spelling. The ?A0x hash is per TU (EXTRA_RULES: matched by prefix).
namespace {
bool __fastcall IsInMeleeWithCombatWheelAttackers(const CThingCreatureBase& creature);   // 0x00979910
}

namespace NCreatureAI {
struct __declspec(novtable) CIsRunnableModule {
    CWorld* World; CThing* Creature;
    CIsRunnableModule(CWorld* world, CThing* creature) : World(world), Creature(creature) { }
    virtual bool IsRunnable() const = 0;
};
struct CIsRunnableModuleCarryingProjectileWeapon : CIsRunnableModule {
    CIsRunnableModuleCarryingProjectileWeapon(CWorld* world, CThing* creature) : CIsRunnableModule(world, creature) { }
    virtual bool IsRunnable() const;
};
}

bool CAIStateGroup_CreatureTakeCover::IsRunnableLeaf()
{
    if (NCreatureAI::CIsRunnableModuleCarryingProjectileWeapon(&GetWorld(), (CThing*)PBrain->Creature).IsRunnable()
        && IS_THING_ALIVE(PTargetThing)
        && !IsInMeleeWithCombatWheelAttackers(*static_cast<const CThingCreatureBase*>(PTargetThing.GetPItem())))
    {
        const CTCCarrying* carrying;
        if (PTargetThing->PeekTC(&carrying)) {
            CThing* weapon = carrying->GetThingInPrimarySlot();
            if (weapon) {
                const CTCWeapon* tcWeapon;
                if (weapon->PeekTC(&tcWeapon) && tcWeapon->IsProjectileWeapon()
                    && CAIStateGroup_CreatureUseCoverBase::IsRunnableLeaf())
                    return true;
            }
        }
    }
    return false;
}
