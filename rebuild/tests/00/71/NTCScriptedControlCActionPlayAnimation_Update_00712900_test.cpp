// Exact target body; constructors and creature services are controlled boundaries.
#include <assert.h>
#include <stdio.h>
#include <string.h>
class CCharString{public:const char* text;};
class CAnimationEntry{};
class CSoundPair{public:long FirstSound,LastSound;CSoundPair(){} explicit CSoundPair(long sound):FirstSound(sound),LastSound(sound){}};
static char events[32];static int event_count,live_actions,constructs,destroys;
static void event(char c){assert(event_count<31);events[event_count++]=c;events[event_count]=0;}
class CThing{public:bool incapacitated;};
class CThingCreatureBase;
class CCreatureActionBase{public:
 bool looking,stay,loop,movement,physics,camera;long loops,priority;
 CThingCreatureBase* creature;const CAnimationEntry* entry;
 CCreatureActionBase(){++live_actions;}
 virtual ~CCreatureActionBase(){event('D');--live_actions;++destroys;}
};
static CCreatureActionBase observed;
class CThingCreatureBase:public CThing{public:
 bool busy,set_result;CAnimationEntry animation;const CCharString* expected_name;
 int queries,random_calls,queued_calls,current_calls;
 bool IsPerformingAnyAction()const{++const_cast<CThingCreatureBase*>(this)->queries;event('Q');return busy;}
 const CAnimationEntry& GetRandomAnimFromName(const CCharString& name,long index)const{
  assert(&name==expected_name&&index==-1);++const_cast<CThingCreatureBase*>(this)->random_calls;event('R');return animation;
 }
 void AddQueuedAction(const CCreatureActionBase& a){++queued_calls;observed=a;event('A');}
 bool SetCurrentAction(const CCreatureActionBase& a){++current_calls;observed=a;event('S');return set_result;}
};
class CCreatureAction_PlayAnimation:public CCreatureActionBase{public:
 CCreatureAction_PlayAnimation(CThingCreatureBase& c,const CAnimationEntry& a,bool st,bool lp,long n,const CSoundPair& snd,bool mv,bool ph,bool cam,long unused,long pr){
  assert(!snd.FirstSound&&!snd.LastSound&&!unused);creature=&c;entry=&a;stay=st;loop=lp;loops=n;movement=mv;physics=ph;camera=cam;priority=pr;looking=false;++constructs;event('C');
 }
};
class CCreatureAction_PlayAnimationWithLookTurning:public CCreatureAction_PlayAnimation{public:
 CCreatureAction_PlayAnimationWithLookTurning(CThingCreatureBase& c,const CAnimationEntry& a,bool st,bool lp,long n,const CSoundPair& snd,bool mv,bool ph,bool cam,long unused,long pr):CCreatureAction_PlayAnimation(c,a,st,lp,n,snd,mv,ph,cam,unused,pr){looking=true;}
};
class CTCScriptedControl{public:
 CThingCreatureBase* ParentThing;
 CThingCreatureBase& GetParentCreature(){return *ParentThing;}
 class CActionBase{public:
  CTCScriptedControl* PTCScript;bool Terminated;
  CThing& GetParentThing(){return *PTCScript->ParentThing;}
  CThingCreatureBase& GetParentCreature(){return PTCScript->GetParentCreature();}
  void Terminate(){Terminated=true;}
 };
};
namespace NCreatureMode{bool IsIncapacitated(const CThing& t){event('I');return t.incapacitated;}}
namespace NTCScriptedControl{
class CActionPlayAnimation:public CTCScriptedControl::CActionBase{public:
 bool PlayedAnim;CCharString Animation;bool StayOnLastFrame,Looping;long NumLoops;
 bool UseMovement,UsePhysics;long Priority;bool AddAsQueuedAction,WaitForAnimationToFinish,AnimMayNeedCameraPositionUpdatedInSpeakAndWait,AllowLooking;
 void Update();
};
void CActionPlayAnimation::Update()
{
    CCreatureActionBase* action=0;
    if (NCreatureMode::IsIncapacitated(GetParentCreature()))
        return;
    if (!PlayedAnim) {
        if (AllowLooking)
            action = new CCreatureAction_PlayAnimationWithLookTurning(GetParentCreature(),
                GetParentCreature().GetRandomAnimFromName(Animation, -1), StayOnLastFrame, Looping, NumLoops,
                CSoundPair(0), UseMovement, UsePhysics, AnimMayNeedCameraPositionUpdatedInSpeakAndWait, 0, Priority);
        else
            action = new CCreatureAction_PlayAnimation(GetParentCreature(),
                GetParentCreature().GetRandomAnimFromName(Animation, -1), StayOnLastFrame, Looping, NumLoops,
                CSoundPair(0), UseMovement, UsePhysics, AnimMayNeedCameraPositionUpdatedInSpeakAndWait, 0, Priority);
        if (AddAsQueuedAction) {
            if (GetParentCreature().IsPerformingAnyAction()) {
                GetParentCreature().AddQueuedAction(*action);
                PlayedAnim = true;
            } else
                PlayedAnim = GetParentCreature().SetCurrentAction(*action);
        } else
            PlayedAnim = GetParentCreature().SetCurrentAction(*action);
        if (!WaitForAnimationToFinish)
            Terminate();
    } else {
        if (!GetParentCreature().IsPerformingAnyAction())
            Terminate();
    }
    if(action)
        delete action;
}
}
int main(){
 int cases=0;const int permanent_actions=live_actions;
 for(int flags=0;flags<128;++flags)for(int params=0;params<4;++params)for(int old_term=0;old_term<2;++old_term){
  CThingCreatureBase creature;creature.incapacitated=(flags&1)!=0;creature.busy=(flags&4)!=0;creature.set_result=(flags&64)!=0;
  creature.queries=creature.random_calls=creature.queued_calls=creature.current_calls=0;
  CTCScriptedControl control;control.ParentThing=&creature;
  NTCScriptedControl::CActionPlayAnimation action;action.PTCScript=&control;action.Terminated=old_term!=0;
  action.PlayedAnim=(flags&2)!=0;action.AllowLooking=(flags&8)!=0;action.AddAsQueuedAction=(flags&16)!=0;action.WaitForAnimationToFinish=(flags&32)!=0;
  action.Animation.text="original_animation";creature.expected_name=&action.Animation;
  action.StayOnLastFrame=(params&1)!=0;action.Looping=(params&2)!=0;action.UseMovement=!(params&1);action.UsePhysics=!(params&2);action.AnimMayNeedCameraPositionUpdatedInSpeakAndWait=params==0||params==3;
  action.NumLoops=params;action.Priority=params-2;event_count=constructs=destroys=0;events[0]=0;
  action.Update();
  bool active=!creature.incapacitated;bool newly=active&&!(flags&2);bool queued=newly&&action.AddAsQueuedAction&&creature.busy;
  bool set=newly&&!queued;bool completed=active&&((flags&2)?!creature.busy:!action.WaitForAnimationToFinish);
  assert(action.Terminated==((old_term!=0)||completed));
  assert(action.PlayedAnim==((flags&2)?true:(newly?(queued||creature.set_result):false)));
  assert(constructs==(newly?1:0)&&destroys==constructs&&live_actions==permanent_actions);
  assert(creature.random_calls==(newly?1:0));assert(creature.queued_calls==(queued?1:0)&&creature.current_calls==(set?1:0));
  assert(creature.queries==(active?((flags&2)?1:(action.AddAsQueuedAction?1:0)):0));
  char expected[32]="I";if(active){if(flags&2)strcat(expected,"Q");else{strcat(expected,"RC");if(action.AddAsQueuedAction)strcat(expected,"Q");strcat(expected,queued?"AD":"SD");}}
  assert(strcmp(events,expected)==0);
  if(newly){assert(observed.creature==&creature&&observed.entry==&creature.animation);assert(observed.looking==action.AllowLooking);assert(observed.stay==action.StayOnLastFrame&&observed.loop==action.Looping&&observed.loops==action.NumLoops);assert(observed.movement==action.UseMovement&&observed.physics==action.UsePhysics&&observed.camera==action.AnimMayNeedCameraPositionUpdatedInSpeakAndWait&&observed.priority==action.Priority);}
  ++cases;
 }
 assert(cases==1024);puts("00712900_TEST PASS");return 0;
}