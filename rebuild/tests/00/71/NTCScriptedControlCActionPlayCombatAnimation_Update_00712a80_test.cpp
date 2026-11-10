#include <stdio.h>
static int created,destroyed,queued,current,looked;static bool capturedPhysics,movementOnDispatch;static const char* capturedName;
class CCharString{public:const char* Value;CCharString(const char* p="anim"):Value(p){}};
class CCreatureActionBase{public:virtual~CCreatureActionBase(){++destroyed;}};
class CThingCreatureBase;
class CCreatureAction_PlayCombatAnimation:public CCreatureActionBase{public:bool AlwaysUseMovementInAnim;CCreatureAction_PlayCombatAnimation(CThingCreatureBase&,const CCharString& n,bool p):AlwaysUseMovementInAnim(false){++created;capturedPhysics=p;capturedName=n.Value;}void SetAlwaysUseMovementInAnim(bool value){AlwaysUseMovementInAnim=value;}};
class CCreatureAction_PlayCombatAnimationWithLooking:public CCreatureAction_PlayCombatAnimation{public:CCreatureAction_PlayCombatAnimationWithLooking(CThingCreatureBase& c,const CCharString& n,bool p):CCreatureAction_PlayCombatAnimation(c,n,p){++looked;}};
class CThingCreatureBase{public:bool Active,Accept;bool IsPerformingAnyAction()const{return Active;}void AddQueuedAction(const CCreatureActionBase& a){++queued;movementOnDispatch=static_cast<const CCreatureAction_PlayCombatAnimation&>(a).AlwaysUseMovementInAnim;}bool SetCurrentAction(const CCreatureActionBase& a){++current;movementOnDispatch=static_cast<const CCreatureAction_PlayCombatAnimation&>(a).AlwaysUseMovementInAnim;return Accept;}};
namespace NTCScriptedControl {class CActionPlayCombatAnimation{public:CThingCreatureBase* Creature;bool PlayedAnim,UsePhysics;CCharString Animation;bool AddAsQueuedAction,WaitForAnimationToFinish,AnimMayNeedCameraPositionUpdatedInSpeakAndWait,AllowLooking,Terminated;CThingCreatureBase& GetParentCreature(){return*Creature;}void Terminate(){Terminated=true;}void Update();};}
namespace NTCScriptedControl {
void CActionPlayCombatAnimation::Update(){
 CCreatureAction_PlayCombatAnimation* action=0;
 if(!PlayedAnim){
  if(AllowLooking)action=new CCreatureAction_PlayCombatAnimationWithLooking(GetParentCreature(),Animation,UsePhysics);
  else action=new CCreatureAction_PlayCombatAnimation(GetParentCreature(),Animation,UsePhysics);
  action->SetAlwaysUseMovementInAnim(true);
  if(AddAsQueuedAction){
   if(GetParentCreature().IsPerformingAnyAction()){
    GetParentCreature().AddQueuedAction(*action);PlayedAnim=true;
   }else PlayedAnim=GetParentCreature().SetCurrentAction(*action);
  }else PlayedAnim=GetParentCreature().SetCurrentAction(*action);
  if(!WaitForAnimationToFinish)Terminate();
 }else {
  if(!GetParentCreature().IsPerformingAnyAction())Terminate();
 }
 if(action)delete action;
}
}

int main(){
 for(int mask=0;mask<256;++mask){
  CThingCreatureBase creature;creature.Active=(mask&1)!=0;creature.Accept=(mask&2)!=0;
  NTCScriptedControl::CActionPlayCombatAnimation row;row.Creature=&creature;row.PlayedAnim=(mask&4)!=0;row.AllowLooking=(mask&8)!=0;row.UsePhysics=(mask&16)!=0;row.AddAsQueuedAction=(mask&32)!=0;row.WaitForAnimationToFinish=(mask&64)!=0;row.Terminated=(mask&128)!=0;row.AnimMayNeedCameraPositionUpdatedInSpeakAndWait=false;
  bool wasPlayed=row.PlayedAnim;bool wasTerminated=row.Terminated;created=destroyed=queued=current=looked=0;movementOnDispatch=false;capturedName=0;
  row.Update();
  bool expectQueue=!wasPlayed&&row.AddAsQueuedAction&&creature.Active;
  bool expectCurrent=!wasPlayed&&!expectQueue;
  bool expectPlayed=wasPlayed||expectQueue||(expectCurrent&&creature.Accept);
  bool expectTerminated=wasTerminated||(wasPlayed?!creature.Active:!row.WaitForAnimationToFinish);
  if(row.PlayedAnim!=expectPlayed||row.Terminated!=expectTerminated||created!=(wasPlayed?0:1)||destroyed!=created||queued!=(expectQueue?1:0)||current!=(expectCurrent?1:0)||looked!=(!wasPlayed&&row.AllowLooking?1:0))return 1;
  if(!wasPlayed&&(!movementOnDispatch||capturedPhysics!=row.UsePhysics||capturedName!=row.Animation.Value))return 2;
 }
 puts("00712a80_TEST PASS");return 0;
}