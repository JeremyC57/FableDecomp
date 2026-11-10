// CCreatureAction_FireMissileWeapon::Initialise -- face the target, unload the weapon and set the transition delay.
// Retail: 0x008D55A0,486 bytes,virtual thiscall.
// Grade: VERIFIED_MATCH; exact code and original operands independently verified.
// Leaf: ?Initialise@CCreatureAction_FireMissileWeapon@@UAEXXZ
// Flags: none
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_PEEKPLAYER
#define FABLE_RETAIL_CTHING_GETTC_ORIGINAL_CHAIN
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CCREATUREACTION_FIREMISSILEWEAPON
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CTCCARRYING
#define FABLE_RETAIL_CTCWEAPON
#define FABLE_RETAIL_CTCWEAPON_SETLOADED
#define FABLE_RETAIL_CANIMATIONENTRY_ACCESSORS
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_CTOR_INLINE
#define FABLE_RETAIL_C3DVECTOR
#include "engine/CThingCreatureBase.h"
#include "engine/CCreatureAction_FireMissileWeapon.h"
#include "engine/ThingAlive.h"
#include "engine/CPlayer.h"
#include "engine/CRightHandedSet.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CTCCarrying.h"
#include "engine/CTCWeapon.h"
#include "engine/CAnimationEntry.h"
void CCreatureAction_FireMissileWeapon::Initialise()
{
 if(IS_THING_ALIVE(PTarget) && !(PCreature->PeekPlayer().IsZTargeting() || PCreature->PeekPlayer().IsFirstPersonTargeting())) {
  C3DVector line=PTarget->GetPos()-PCreature->GetPos();
  line.Z=0.0f;
  if(line.IsNonZero()) {
   line.AccurateNormalise();
   PCreature->GetPhysicsTC()->SetRHSet(CRightHandedSet(C3DVector(0.0f,0.0f,1.0f),line));
  }
 }
 CTCCarrying* ptc_carry;
 if(IS_THING_ALIVE(PCreature) && PCreature->GetTC(&ptc_carry)) {
  CThing* pcarried_thing=ptc_carry->GetThingInPrimarySlot();
  CTCWeapon* ptc_weapon;
  if(IS_THING_ALIVE(pcarried_thing) && pcarried_thing->GetTC(&ptc_weapon))
   ptc_weapon->SetProjectileWeaponLoaded(false,0);
 }
 if(!PAnimEntry.IsNull()) DelayTimer=PAnimEntry->GetTransitionTime();
 CCreatureActionBase::Initialise();
}