// CAIStateGroup_CreatureAdvanceInCover::IsRunnableLeaf() -- the target is not in melee, holds a projectile weapon, and the cover base's leaf test passes
// Leaf: ?IsRunnableLeaf@CAIStateGroup_CreatureAdvanceInCover@@MAE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00978E60, length 172 bytes (+ int3 padding).
// Identity: Ego_r.pdb 0x0099DB40 / FableWin.pdb 0x02C15700 ?IsRunnableLeaf@CAIStateGroup_CreatureAdvanceInCover@@MAE_NXZ (protected virtual;
//   the landed leaf was the QAE spelling). Name fix (NF56, round 7, queue q10460): the qualified base call 0x00970F30 binds
//   ?IsRunnableLeaf@CAIStateGroup_CreatureUseCoverBase@@MAE_NXZ (Ego_r.pdb 0x00995CE0 / FableWin.pdb 0x02B85370; was the QAE spelling of a
//   local model of the shared-header class). The row now sits on the shared headers: the retail class of
//   engine/CAIStateGroup_CreatureAdvanceInCover.h (overlay opt-in _ISRUNNABLELEAF) on engine/CAIStateGroup_CreatureUseCoverBase.h, and
//   engine/CThing.h's PeekTC<T> (FABLE_RETAIL_CTHING_GETTC_EXPANDED: the UsingComponents bit test, the const CVectorMap::LowerBound 0x004365B0)
//   in place of the local CThing / CVectorMap / CAIBrain / CTCCarrying / CTCWeapon / CThingCreatureBase models.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PTargetThing +0x14), CThingCreatureBase::IsInMelee @ 0x00661E80,
//   CTCCarrying::GetThingInPrimarySlot @ 0x006A1440, CTCWeapon::IsProjectileWeapon @ 0x005DAA40, the base IsRunnableLeaf @ 0x00970F30.
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
#define FABLE_RETAIL_CTCWEAPON
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE
#define FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE_VSLOTS
#define FABLE_RETAIL_CAISTATEGROUP_CREATUREUSECOVERBASE
#define FABLE_RETAIL_CAISTATEGROUP_CREATUREUSECOVERBASE_COVERLIST
#define FABLE_RETAIL_CAISTATEGROUP_CREATUREADVANCEINCOVER_ISRUNNABLELEAF
#include "fable_compiler.h"
#include "rebuild_abi.h"
#include <stddef.h>
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CTCCarrying.h"
#include "engine/CTCWeapon.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIStateGroup_CreatureAdvanceInCover.h"

bool CAIStateGroup_CreatureAdvanceInCover::IsRunnableLeaf()
{
    if (!PTargetThing->IsInMelee()) {
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
