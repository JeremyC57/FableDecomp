// CTCHeroRangedAttacks::FireRangedWeapon -- validate the queued projectile shot, dispatch its action and post the fire message
// Retail: 0078a390,563 bytes; private thiscall.
// Leaf: ?FireRangedWeapon@CTCHeroRangedAttacks@@AAEXXZ
// Flags: none
#define FABLE_RETAIL_FIRE_RANGED_WEAPON
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_ORIGINAL_CHAIN
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CTCWEAPON
#define FABLE_RETAIL_CTCWEAPON_MEMBERS
#define FABLE_RETAIL_CTCWEAPON_LOADING
#define FABLE_RETAIL_CTCCARRYING_MEMBERS
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CCREATUREACTION_FIREMISSILEWEAPON
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_MESSAGEEVENTMANAGER
#define FABLE_RETAIL_IS_THING_ALIVE
#include "engine/CThingCreatureBase.h"
#include "engine/CTCHeroRangedAttacks.h"
#include "engine/CTCCarrying.h"
#include "engine/CTCWeapon.h"
#include "engine/CTCTargetingBase.h"
#include "engine/CHeroCombatDef.h"
#include "engine/CCreatureAction_FireMissileWeapon.h"
#include "engine/CPlayer.h"
#include "engine/CWorld.h"
#include "engine/CMessageEventManager.h"
#include "engine/NCreatureMode.h"
#include "engine/ThingAlive.h"
void CTCHeroRangedAttacks::FireRangedWeapon()
{
 if(FireQueued&&AimingTimer>=0) {
  CTCCarrying* ptc_carrying;
  GetParentThing().GetTC(&ptc_carrying);
  CThing* pcarried_thing=ptc_carrying->GetThingInPrimarySlot();
  const CTCWeapon* ptc_weapon;
  if(IS_THING_ALIVE(pcarried_thing)&&pcarried_thing->PeekTC(&ptc_weapon)&&ptc_weapon->IsProjectileWeapon()&&ptc_weapon->IsProjectileWeaponLoaded()) {
   const CTCTargetingBase* ptc_targeting;
   GetParentThing().PeekTC(&ptc_targeting);
   CThing* ptarget=ptc_targeting->GetPTargetedThing();
   float augmented_damage_multiplier=DamageMultiplier;
   float accuracy_to_use=AccuracyOverride>=0.0f?AccuracyOverride:Accuracy;
   if(GetParentThing().GetPlayer().GetCurrentMode()==(EPlayerMode)25)accuracy_to_use=100.0f;
   CDefPointer<const CMeleeCombatAbilityDef> pdef;
   if(IS_THING_ALIVE(ptarget)&&!NCreatureMode::IsValidRangedWeaponTargetForShooter(GetParentThing(),*ptarget,PHeroCombatDef->MaxAngleToTurnToShootAtTargetInThirdPersonBowMode))ptarget=0;
   CCreatureAction_FireMissileWeapon action_bloke(*PParentCreature,ptarget,augmented_damage_multiplier,accuracy_to_use,GetShotBuildupTime(),Decapitate,pdef,100,4);
   if(!PParentCreature->SetCurrentAction(action_bloke))action_bloke.PerformAction();
   CThing& thing=GetParentThing();
   unsigned long frames=CMainGameComponent::GetConstantFPS();
   CMessageEventManager& messages=GetWorld().GetMessageEventManager();
   messages.AddMessage((EMessageEventType)67,frames,thing.GetPos(),0,0,0.0f,0.0f,0);
   FireQueued=false;
  }
 }
}