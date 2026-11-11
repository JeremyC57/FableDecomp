#include <cassert>
#include <cstdio>
#include <cstring>
enum EPlayerMode { PLAYER_MODE_VIEW_HERO=47,PLAYER_MODE_USE_PROJECTILE_WEAPON=25 };
class CThing;class CTCHeroRangedAttacks;class CTCCarrying;class CTCSpecialAbilities;class CTCWeapon;
static int lookups,actionQueries,carryingQueries,aliveQueries,weaponQueries,cameraQueries,stringConstructions,stringDestructions,soundCalls;
class CCharString {public:const char* Text;CCharString(const char* text):Text(text){++stringConstructions;}~CCharString(){++stringDestructions;}};
class CTCHeroRangedAttacks {public:int Mode,Changes;void IncrementProjectileWeaponMode(){Mode=1-Mode;++Changes;}bool IsToUseFirstPersonTargetingMode()const{return Mode!=0;}};
class CTCCarrying {public:CThing* Item;CThing* GetThingInPrimarySlot()const{++carryingQueries;return Item;}};
class CTCSpecialAbilities {public:bool Locked;bool GetAbilityMutexLock()const{return Locked;}};
class CTCWeapon {public:bool Projectile;bool IsProjectileWeapon()const{++weaponQueries;return Projectile;}};
class CThing {public:bool Dead;CTCWeapon* Weapon;bool IsDead()const{return Dead;}bool PeekTC(const CTCWeapon** out)const{++lookups;if(!Weapon)return false;*out=Weapon;return true;}};
class CThingPlayerCreature:public CThing {public:
 CTCHeroRangedAttacks* Ranged;CTCCarrying* Carrying;CTCSpecialAbilities* Abilities;bool Performing;
 bool GetTC(CTCHeroRangedAttacks** out){++lookups;if(!Ranged)return false;*out=Ranged;return true;}
 bool PeekTC(const CTCCarrying** out)const{++lookups;if(!Carrying)return false;*out=Carrying;return true;}
 bool PeekTC(const CTCSpecialAbilities** out)const{++lookups;if(!Abilities)return false;*out=Abilities;return true;}
 bool IsPerformingAnyAction()const{++actionQueries;return Performing;}
};
template<class T>bool IS_THING_ALIVE(const T& thing){++aliveQueries;return thing!=0&&!thing->IsDead();}
class CPlayer {public:bool View,FirstPerson,Mode;int Added,Removed;CThingPlayerCreature* Hero;
 bool HasMode(EPlayerMode mode)const{assert(mode==PLAYER_MODE_VIEW_HERO);return View;}
 CThingPlayerCreature* GetPControlledCreature()const{return Hero;}
 bool IsFirstPersonTargeting()const{return FirstPerson;}
 void AddMode(EPlayerMode mode,bool value){assert(mode==PLAYER_MODE_USE_PROJECTILE_WEAPON&&!value);Mode=true;++Added;}
 void RemoveMode(EPlayerMode mode){assert(mode==PLAYER_MODE_USE_PROJECTILE_WEAPON);Mode=false;++Removed;}
};
class CGameCameraManager {public:void PlayCameraSoundThroughObservedThing(const CCharString& s)const{assert(std::strcmp(s.Text,"PROJECTILE_WEAPON_ZOOM_IN")==0);++soundCalls;}};
class CAIGameCameraBase {public:CGameCameraManager* Manager;bool GetCameraManager(CGameCameraManager** out){++cameraQueries;*out=Manager;return true;}};
class CGameEvent {public:signed char Player;signed char GetPlayerNumber()const{return Player;}};
class CWorld {public:CPlayer *First,*Second;CAIGameCameraBase Camera;int PlayerCalls;
 CPlayer* GetPlayer(unsigned long number){assert(number==3);return ++PlayerCalls==1?First:Second;}
 CAIGameCameraBase* GetGameCamera(){return &Camera;}
 void EAUseProjectileWeapon(const CGameEvent* event);
};
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

int main(){
 unsigned cases=0;
 for(unsigned bits=0;bits<32768;++bits){
  const bool view=(bits&1)!=0,present=(bits&2)!=0,heroPresent=(bits&4)!=0,heroDead=(bits&8)!=0;
  const bool rangedPresent=(bits&16)!=0,carryingPresent=(bits&32)!=0,firstPerson=(bits&64)!=0,performing=(bits&128)!=0;
  const bool locked=(bits&256)!=0,weaponPresent=(bits&512)!=0,weaponDead=(bits&1024)!=0,weaponTC=(bits&2048)!=0;
  const bool projectile=(bits&4096)!=0,initial=(bits&8192)!=0,abilityPresent=(bits&16384)!=0;
  lookups=actionQueries=carryingQueries=aliveQueries=weaponQueries=cameraQueries=stringConstructions=stringDestructions=soundCalls=0;
  CTCHeroRangedAttacks ranged;ranged.Mode=initial?1:0;ranged.Changes=0;
  CTCWeapon weaponComponent;weaponComponent.Projectile=projectile;
  CThing weapon;weapon.Dead=weaponDead;weapon.Weapon=weaponTC?&weaponComponent:0;
  CTCCarrying carrying;carrying.Item=weaponPresent?&weapon:0;
  CTCSpecialAbilities ability;ability.Locked=locked;
  CThingPlayerCreature hero;hero.Dead=heroDead;hero.Weapon=0;hero.Ranged=rangedPresent?&ranged:0;hero.Carrying=carryingPresent?&carrying:0;hero.Abilities=abilityPresent?&ability:0;hero.Performing=performing;
  CPlayer player;player.View=view;player.FirstPerson=firstPerson;player.Mode=initial;player.Added=player.Removed=0;player.Hero=heroPresent?&hero:0;
  CGameCameraManager manager;CWorld world;world.First=&player;world.Second=present?&player:0;world.PlayerCalls=0;world.Camera.Manager=&manager;
  CGameEvent event;event.Player=3;
  world.EAUseProjectileWeapon(&event);
  const bool reachesHero=!view&&present&&heroPresent&&!heroDead;
  const bool reachesActions=reachesHero&&rangedPresent&&carryingPresent;
  const bool reachesAbilities=reachesActions&&(firstPerson||!performing);
  const bool reachesCarried=reachesAbilities&&(!abilityPresent||!locked);
  const bool reachesWeapon=reachesCarried&&weaponPresent&&!weaponDead;
  const bool changed=reachesWeapon&&weaponTC&&projectile;
  const int expectedLookups=(reachesHero?1:0)+(reachesHero&&rangedPresent?1:0)+(reachesAbilities?1:0)+(reachesWeapon?1:0);
  assert(world.PlayerCalls==(view?1:2));assert(lookups==expectedLookups);
  assert(actionQueries==(reachesActions&&!firstPerson?1:0));assert(carryingQueries==(reachesCarried?1:0));assert(aliveQueries==(reachesCarried?1:0));
  assert(weaponQueries==(reachesWeapon&&weaponTC?1:0));assert(ranged.Changes==(changed?1:0));
  assert(ranged.Mode==(changed?!initial:initial));assert(player.Mode==(changed?!initial:initial));
  assert(player.Added==(changed&&!initial?1:0));assert(player.Removed==(changed&&initial?1:0));
  assert(cameraQueries==(changed?1:0)&&soundCalls==(changed?1:0));
  assert(stringConstructions==(changed?1:0)&&stringDestructions==stringConstructions);
  ++cases;
 }
 assert(cases==32768);std::puts("0062bef0_TEST PASS");return 0;
}