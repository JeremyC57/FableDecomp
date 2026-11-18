// Verbatim selected body with explicit external service test models.
#include <cassert>
#include <cstdio>
#include <climits>
#ifndef _MSC_VER
#define __fastcall

#endif
struct CCharString{};static CCharString GET_UP_FROM_FLOOR_ANIM_NAME;
#ifndef TEST_FPS
#define TEST_FPS 2
#endif
static const int MIN_TIME_TO_LEAVE_BETWEEN_SITTINGS=TEST_FPS*10;
static const int RANGE_TIME_TO_LEAVE_BETWEEN_SITTINGS=TEST_FPS*50;
static unsigned int seed_value;static int frame_value;static int world_calls,seed_calls,frame_calls,rotate_calls;
static int constructed,destructed,cloned,brain_calls,output_calls;
static bool free_to_act;
class CAIStateGroup_SitRoundCampfire;static CAIStateGroup_SitRoundCampfire* active_group;static bool mutate_sitting;
struct CWorld{static int __fastcall GetFrame(){++frame_calls;return frame_value;}};
static int __fastcall GFROR13(unsigned int x){++rotate_calls;assert(seed_value==x);return static_cast<int>((x>>13)|(x<<19));}
template<class T>class CIntelligentPointer{public:T* ptr;CIntelligentPointer():ptr(0){} T* GetPItem()const{return ptr;}T* operator->()const{return ptr;}};
struct CTCUserActionPointBase{int released;int last;CTCUserActionPointBase():released(0),last(-1){}void SetActionPointAsFree(int);};
struct CTCSingleUserActionPoints:CTCUserActionPointBase{};
struct CMode{int type;int queries;CMode():type(0),queries(0){}int GetType(){++queries;return type;}};
struct CTCCreatureModeManager{CMode mode;int gets,removed;CTCCreatureModeManager():gets(0),removed(0){}CMode& GetCurrentMode(){++gets;return mode;}void RemoveMode(int m){assert(m==10);++removed;}};
struct CTCIdleScheduler{int chosen;CTCIdleScheduler():chosen(0){}void ChooseNewIdle(){++chosen;}};
struct CThingObject{bool dead;CTCSingleUserActionPoints* points;bool IsDead()const{return dead;}bool GetTC(CTCSingleUserActionPoints** p){*p=points;return points!=0;}};
struct CCreatureActionBase{int sittingMode;bool remove;const CCharString* animation;CCreatureActionBase():sittingMode(0),remove(false),animation(0){++constructed;}CCreatureActionBase(const CCreatureActionBase& a):sittingMode(a.sittingMode),remove(a.remove),animation(a.animation){++constructed;}virtual ~CCreatureActionBase(){++destructed;}virtual CCreatureActionBase* Clone()const{++cloned;return new CCreatureActionBase(*this);}};
struct CCreatureAction_GetUpFromSitting:CCreatureActionBase{CCreatureAction_GetUpFromSitting(struct CThingAICreature&,const CCharString& a,int m,bool b){sittingMode=m;remove=b;animation=&a;}};
struct TestActionPtr{CCreatureActionBase* p;TestActionPtr():p(0){}~TestActionPtr(){delete p;}void Reset(CCreatureActionBase* a){delete p;p=a;}};
struct CBrainOutput{unsigned int Types;TestActionPtr PAction;CBrainOutput():Types(0){}void SetAsPerformAction(const CCreatureActionBase& action){Types|=8;PAction.Reset(action.Clone());}};
struct CAIBrain{void SetOutput(const CBrainOutput& b){++brain_calls;assert(b.Types==8&&b.PAction.p&&b.PAction.p->animation==&GET_UP_FROM_FLOOR_ANIM_NAME);assert(b.PAction.p->sittingMode==0&&!b.PAction.p->remove);}};
struct CThingAICreature{CTCCreatureModeManager* manager;CTCIdleScheduler* scheduler;int free_calls;CThingAICreature():manager(0),scheduler(0),free_calls(0){}bool GetTC(CTCCreatureModeManager** p){*p=manager;return manager!=0;}bool GetTC(CTCIdleScheduler** p){*p=scheduler;return scheduler!=0;}bool IsFreeToPerformAction(const CCreatureActionBase& a){++free_calls;assert(a.animation==&GET_UP_FROM_FLOOR_ANIM_NAME&&a.sittingMode==0&&!a.remove);return free_to_act;}};
class CAIStateGroup_SitRoundCampfire{public:bool GotSeatPos,Sitting;CIntelligentPointer<CThingObject> PCampfire;int SeatPosIndex;unsigned int FrameToNextSitOnSeat;CAIBrain* PBrain;CThingAICreature* creature;CWorld world;CThingAICreature& GetCreature(){return *creature;}const CWorld& PeekWorld(){++world_calls;return world;}unsigned int* GetPWorldSeed(){++seed_calls;return &seed_value;}void OnReplace();};
void CTCUserActionPointBase::SetActionPointAsFree(int i){++released;last=i;if(mutate_sitting)active_group->Sitting=true;}

#ifdef _MSC_VER
#define FABLE_FASTCALL __fastcall
#else
#define FABLE_FASTCALL
#endif
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
inline unsigned int FABLE_FASTCALL GFRandom(unsigned int range,unsigned int* seed) {
 *seed=*seed*0x24a1U+0x24dfU;
 *seed=GFROR13(*seed);
 return range ? *seed%range : 0;
}
#ifdef _MSC_VER
#pragma inline_depth()
#endif
template<class T> inline bool FABLE_FASTCALL operator!=(const CIntelligentPointer<T>& p,int) {
 return p.GetPItem()!=0;
}
template<class T> inline bool FABLE_FASTCALL IS_THING_ALIVE(const T& thing) {
 return thing!=0 && !thing->IsDead();
}
void CAIStateGroup_SitRoundCampfire::OnReplace() {
 CTCCreatureModeManager* ptc_mode_manager;
 if(GotSeatPos && IS_THING_ALIVE(PCampfire)) {
  CTCSingleUserActionPoints* ptc_action_points;
  PCampfire->GetTC(&ptc_action_points);
  ptc_action_points->SetActionPointAsFree(SeatPosIndex);
 }
 GetCreature().GetTC(&ptc_mode_manager);
 if(Sitting || ptc_mode_manager->GetCurrentMode().GetType()==10) {
  FrameToNextSitOnSeat=PeekWorld().GetFrame()+MIN_TIME_TO_LEAVE_BETWEEN_SITTINGS+GFRandom(RANGE_TIME_TO_LEAVE_BETWEEN_SITTINGS,GetPWorldSeed());
  CBrainOutput output;
  const CCreatureActionBase& action=CCreatureAction_GetUpFromSitting(GetCreature(),GET_UP_FROM_FLOOR_ANIM_NAME,0,false);
  if(GetCreature().IsFreeToPerformAction(action)) {
   output.SetAsPerformAction(action);
   PBrain->SetOutput(output);
  }
  ptc_mode_manager->RemoveMode(10);
  Sitting=false;
 }
 CTCIdleScheduler* scheduler;
 if(GetCreature().GetTC(&scheduler))scheduler->ChooseNewIdle();
}

static unsigned int reference_next(unsigned int value){unsigned int x=value*0x24a1U+0x24dfU;return (x>>13)|(x<<19);}
static void one(unsigned int seed,int frame,bool got,int camp,bool sitting,bool mode,bool allowed,bool has_scheduler,bool mutation){
 world_calls=seed_calls=frame_calls=rotate_calls=constructed=destructed=cloned=brain_calls=output_calls=0;
 seed_value=seed;frame_value=frame;free_to_act=allowed;mutate_sitting=mutation;
 CAIStateGroup_SitRoundCampfire group;active_group=&group;CAIBrain brain;CTCSingleUserActionPoints points;CThingObject fire;fire.dead=camp==1;fire.points=&points;
 CTCCreatureModeManager manager;manager.mode.type=mode?10:11;CTCIdleScheduler scheduler;CThingAICreature creature;creature.manager=&manager;creature.scheduler=has_scheduler?&scheduler:0;
 group.GotSeatPos=got;group.Sitting=sitting;group.PCampfire.ptr=camp?&fire:0;group.SeatPosIndex=17;group.FrameToNextSitOnSeat=0xfedcba98U;group.PBrain=&brain;group.creature=&creature;
 bool released=got&&camp==2;bool effective_sitting=sitting||(released&&mutation);bool active=effective_sitting||mode;
 group.OnReplace();
 assert(points.released==(released?1:0));assert(points.last==(released?17:-1));
 assert(manager.gets==(effective_sitting?0:1));assert(manager.mode.queries==manager.gets);
 assert(manager.removed==(active?1:0));assert(scheduler.chosen==(has_scheduler?1:0));
 assert(world_calls==(active?1:0)&&seed_calls==world_calls&&rotate_calls==world_calls&&frame_calls==world_calls);
 assert(creature.free_calls==(active?1:0));assert(brain_calls==(active&&allowed?1:0));
 assert(constructed==(active?(allowed?2:1):0)&&destructed==constructed&&cloned==(active&&allowed?1:0));
 if(active){unsigned int next=reference_next(seed);assert(seed_value==next);assert(group.FrameToNextSitOnSeat==static_cast<unsigned int>(frame)+(RANGE_TIME_TO_LEAVE_BETWEEN_SITTINGS?next%static_cast<unsigned int>(RANGE_TIME_TO_LEAVE_BETWEEN_SITTINGS):0)+MIN_TIME_TO_LEAVE_BETWEEN_SITTINGS);assert(!group.Sitting);}
 else{assert(seed_value==seed&&group.FrameToNextSitOnSeat==0xfedcba98U);assert(group.Sitting==sitting);}
}
int main(){
 assert(sizeof(unsigned int)==4);unsigned int seeds[]={0,1,65535,0x7fffffffU,0xffffffffU};int frames[]={0,17,2147483640};int cases=0;
 for(int a=0;a<5;++a)for(int f=0;f<3;++f)for(int got=0;got<2;++got)for(int camp=0;camp<3;++camp)for(int sit=0;sit<2;++sit)for(int mode=0;mode<2;++mode)for(int allow=0;allow<2;++allow)for(int scheduler=0;scheduler<2;++scheduler){one(seeds[a],frames[f],got!=0,camp,sit!=0,mode!=0,allow!=0,scheduler!=0,false);++cases;}
 for(int a=0;a<2;++a)for(int allowed=0;allowed<2;++allowed)for(int scheduler=0;scheduler<2;++scheduler){one(seeds[a],17,true,2,false,false,allowed!=0,scheduler!=0,true);++cases;}
 assert(cases==1448);std::puts("009397e0_TEST PASS");
}
