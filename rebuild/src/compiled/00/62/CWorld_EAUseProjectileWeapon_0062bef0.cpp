// 0062bef0 CWorld::EAUseProjectileWeapon
// Flags: none
// Retail: 0062bef0,483 bytes.
// Leaf: ?EAUseProjectileWeapon@CWorld@@QAEXPBVCGameEvent@@@Z
#define FABLE_RETAIL_CGAMEEVENT_DATAPACKAGE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_ORIGINAL_CHAIN
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_ACTIONQUERY
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_PLAYER_REMOVEMODE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_EVENTACTIONS
#define FABLE_RETAIL_CTCCARRYING
#define FABLE_RETAIL_CTCWEAPON
#define FABLE_RETAIL_CTCSPECIALABILITIES
#define FABLE_RETAIL_CTCHERORANGEDATTACKS
#define FABLE_RETAIL_CTCHERORANGEDATTACKS_PROJECTILEMODE
#define FABLE_RETAIL_CAIGAMECAMERABASE
#define FABLE_RETAIL_CGAMECAMERAMANAGER_CAMERASOUND
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CPlayer.h"
#include "engine/CTCBase.h"
#include "engine/CTCCarrying.h"
#include "engine/CTCWeapon.h"
#include "engine/CTCHeroRangedAttacks.h"
#include "engine/CTCSpecialAbilities.h"
#include "engine/CAIGameCameraBase.h"
#include "engine/CGameCameraManager.h"
#include "engine/CGameEvent.h"
#include "engine/EPlayerMode.h"
#include "engine/CWorld.h"


#ifdef _MSC_VER
#define FABLE_FASTCALL __fastcall
#else
#define FABLE_FASTCALL
#endif
template <class T> bool FABLE_FASTCALL IS_THING_ALIVE(const T& pweapon);


void CWorld::EAUseProjectileWeapon(const CGameEvent* event)
{
 CPlayer& player=*GetPlayer(event->GetPlayerNumber());
 if(player.HasMode(PLAYER_MODE_VIEW_HERO))return;
 CPlayer* pplayer=GetPlayer(event->GetPlayerNumber());
 if(pplayer) {
  CThingPlayerCreature* phero=pplayer->GetPControlledCreature();
  if(phero && !phero->IsDead()) {
   CTCHeroRangedAttacks* ptc_ranged_attacks;
   if(phero->GetTC(&ptc_ranged_attacks)) {
    const CTCCarrying* ptc_carrying;
    if(phero->PeekTC(&ptc_carrying)) {
     if(!pplayer->IsFirstPersonTargeting() && phero->IsPerformingAnyAction())return;
     const CTCSpecialAbilities* ptc_special_abilities;
     if(phero->PeekTC(&ptc_special_abilities) && ptc_special_abilities->GetAbilityMutexLock())return;
     CThing* pweapon=ptc_carrying->GetThingInPrimarySlot();
     if(IS_THING_ALIVE(&*pweapon)) {
      const CTCWeapon* ptc_weapon;
      if(pweapon->PeekTC(&ptc_weapon) && ptc_weapon->IsProjectileWeapon()) {
       ptc_ranged_attacks->IncrementProjectileWeaponMode();
       if(ptc_ranged_attacks->IsToUseFirstPersonTargetingMode())
        pplayer->AddMode(PLAYER_MODE_USE_PROJECTILE_WEAPON,false);
       else pplayer->RemoveMode(PLAYER_MODE_USE_PROJECTILE_WEAPON);
       CGameCameraManager* pcam_man;
       GetGameCamera()->GetCameraManager(&pcam_man);
       pcam_man->PlayCameraSoundThroughObservedThing("PROJECTILE_WEAPON_ZOOM_IN");
      }
     }
    }
   }
  }
 }
}