// CAIStateGroup_CreatureSeekEnemyUnderground::OnReplace() -- when the enemy is still alive, put the burrowed creature back at its rise position, turn it to face the enemy (flat direction) and set its right-handed set (up Z, forward = that direction); then, as the idle-underground state does, make it rise up
// Leaf: ?OnReplace@CAIStateGroup_CreatureSeekEnemyUnderground@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) 0x00920DC0, 455 bytes, thiscall plain `ret` (public virtual).
// Names: Ego_r.pdb ?OnReplace@CAIStateGroup_CreatureSeekEnemyUnderground@@UAEXXZ (+ FableWin); members PEnemy (CIntelligentPointer<const
//   CThing>, +0x14) and RisePos (C3DVector, +0x1C) from struct_layouts_egor.tsv; Ego_d ai_state_group_creature_idle_underground.obj:
//   the block-scope local `direction`. The class is IdleUnderground's subclass (landed OnActivate 0x00920990 calls the base's).
//   Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, CThing::GetPos 0x004C73D0, CTCPhysicsBase virtual slots 31 ResetPosition
//   (+0x7C), 32 ResetRHSet (+0x80), 66 SetFacingAngleXY (+0x108) (slots counted in engine/CTCPhysicsBase.h's declaration order,
//   SetPosition = slot 30 as CTCPhysicsStandard::Move's +0x78), CCreatureAction_RiseUp ctor 0x008BD880,
//   CThingCreatureBase::SetCurrentAction 0x006644F0, ~CCreatureActionBase 0x0062E900.
// Same-TU body: CAIStateGroup_CreatureIdleUnderground::OnReplace (retail 0x00920960, RELOCATION_MATCH from this file), expanded at the
//   end through the qualified base call. Classes without a shared header (the two state groups) are declared here over the shared
//   CAIStateGroupBase.h.
#include <math.h>
#pragma intrinsic(atan2, fmod)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ANGLE_BODIES
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_CTOR_INLINE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_IS_THING_ALIVE
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/ThingAlive.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/CCreatureAction_RiseUp.h"

class CAIStateGroup_CreatureIdleUnderground : public CAIStateGroupBase {
public:
    virtual void OnReplace();
};
class CAIStateGroup_CreatureSeekEnemyUnderground : public CAIStateGroup_CreatureIdleUnderground {
public:
    virtual void OnReplace();
    CIntelligentPointer<const CThing> PEnemy;                    // +0x14
    C3DVector RisePos;                                           // +0x1c
};

void CAIStateGroup_CreatureSeekEnemyUnderground::OnReplace()
{
    if (IS_THING_ALIVE(PEnemy)) {
        GetCreature().GetPhysicsTC()->ResetPosition(RisePos);
        C3DVector direction = PEnemy->GetPos() - RisePos;
        direction.Z = 0.0f;
        direction.Normalise();
        GetCreature().GetPhysicsTC()->SetFacingAngleXY(direction.GetAngleXY());
        GetCreature().GetPhysicsTC()->ResetRHSet(CRightHandedSet(C3DVector(0.0f, 0.0f, 1.0f), direction));
    }
    CAIStateGroup_CreatureIdleUnderground::OnReplace();
}

void CAIStateGroup_CreatureIdleUnderground::OnReplace()
{
    GetCreature().SetCurrentAction(CCreatureAction_RiseUp(GetCreature()));
}