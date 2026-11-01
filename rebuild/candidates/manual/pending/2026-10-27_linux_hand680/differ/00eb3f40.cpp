// 00eb3f40 NScript::CV_MurderTwistFinalScript::CMurderTwistAssassin::Main
// Grade: DIFFER(378v381) -- retail keeps a dead 4-byte zero word in the frame directly below the CIsThingAlive filter (`mov [esp+0Ch],0` then the vtable store at +4; frame 14h instead of 10h), which shifts the callee-saved colouring (retail &Me in edi and the entity handle in ebx; built &Me in ebx and the handle in edi). The `if (trusts) ... else ...` form restores retail's otherwise-dead `mov bl,al` trust copy.
// Next step: the same dead word opens the TU sibling 0x00EB55D0 (CMurderTwistAssassinGuard::Main, same 16-byte prologue), so it comes from something shared in CV_MurderTwistFinalScript.cpp (a script macro or a predicate holder). Function-scope, block-scope and ctor-zeroed struct locals are all eliminated. Find the shared construct.
// Leaf: ?Main@CMurderTwistAssassin@CV_MurderTwistFinalScript@NScript@@EAEXXZ
// Flags:
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CIsThingAlive.h"
namespace NScript {
inline CIsThingAlive::CIsThingAlive(const CScriptThing&thing):Thing(thing){}
class CQ_SunnyvaleMasterData;
// Layout: shared native level base48; donor base5c accounts for state fields5c/60 becoming48/4c.
class CV_MurderTwistFinalScript:public CLevelScriptBase<CQ_SunnyvaleMasterData>{public:enum ESheriffState{};enum EAssassinState{};ESheriffState SheriffState;EAssassinState AssassinState;bool DoesAssassinTrustHero();class CMurderTwistAssassin;};
// Layout: shared original entity prefix14, parent14, master18 and resource handle1c.
class CV_MurderTwistFinalScript::CMurderTwistAssassin:public CEntityScriptBase{
protected:CV_MurderTwistFinalScript&ParentClass;CQ_SunnyvaleMasterData*PMasterData;
private:CScriptGameResourceObjectScriptedThingBase MyEntityHandle;unsigned long NextLine;bool HasBeenThreatened;float ThreatDistSq;bool HasGivenIntroToHero,PreemptiveAttackInProgress;
void DoWatchingForTrouble();void DoRemindingHeroToKillSheriff();void AttackHero();virtual void Main();
};
void CV_MurderTwistFinalScript::CMurderTwistAssassin::Main(){
 {CIsThingAlive filter(Me);SetPredicate(filter);}
 Game->NewScriptFrame();if(IsActiveThreadTerminating())return;
 if(MyEntityHandle.IsValid())MyEntityHandle.Release();
 while(!Game->StartScriptingEntity(Me,MyEntityHandle,(EScriptAIPriority)4)){Game->NewScriptFrame();if(IsActiveThreadTerminating())return;}
 if(IsActiveThreadTerminating())return;
 bool trusts=ParentClass.DoesAssassinTrustHero();
 while(ParentClass.AssassinState==0||ParentClass.AssassinState==1){Game->NewScriptFrame();if(IsActiveThreadTerminating())return;if(IsActiveThreadTerminating())return;if(trusts)DoWatchingForTrouble();else DoWatchingForTrouble();}
 if(IsActiveThreadTerminating())return;
 while(ParentClass.AssassinState==3){Game->NewScriptFrame();if(IsActiveThreadTerminating())return;DoRemindingHeroToKillSheriff();}
 if(IsActiveThreadTerminating())return;
 if(ParentClass.AssassinState==2){if(IsActiveThreadTerminating())return;AttackHero();}
 if(IsActiveThreadTerminating())return;
 do{Game->NewScriptFrame();}while(!IsActiveThreadTerminating());
}
}
