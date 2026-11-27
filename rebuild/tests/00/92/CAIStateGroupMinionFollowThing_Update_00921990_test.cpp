#include <vector>
#include <cstdio>
#include <cstring>
#include <cstdlib>
enum EAIActionReturn {AI_ACTION_FINISHED=2,AI_ACTION_IN_PROGRESS=4};
enum EMovementType {AI_MOVEMENT_NULL=0,AI_MOVEMENT_WALKING=3,AI_MOVEMENT_RUNNING=5};
enum {ACTION_REPEAT_CALLBACK=2,ESSG_FOLLOW_RANDOM=6,ESSG_FOLLOW_NEAREST=7};
struct Pos{float x,y;};float GFSqr(float x){return x*x;}float GFGet2DDistanceSquared(const Pos& a,const Pos& b){return GFSqr(b.x-a.x)+GFSqr(b.y-a.y);}
class CThing {public:bool alive,overrideRegion;Pos pos;CThing():alive(true),overrideRegion(false){pos.x=pos.y=0;}const Pos& PeekPos()const{return pos;}bool IsRegionFollowingOverriddenFromScript()const{return overrideRegion;}template<class T>bool GetTC(T** p);};
struct CTCFollowed {bool member;int removed;void RemoveFollower(CThing&){++removed;}bool IsFollower(const CThing*)const{return member;}};
struct CTCRegionFollower {int removed;void RemoveFollower(CThing&){++removed;}};
struct CTCAIScratchpad {int mode;float FollowDistance;int GetTargetName()const{return 77;}int GetScriptingStateGroup()const{return mode;}float GetFollowDistance()const{return FollowDistance;}};
static CTCFollowed followed;static CTCRegionFollower region;static CTCAIScratchpad scratch;static bool hasFollow,hasRegion;
template<>bool CThing::GetTC(CTCFollowed**p){*p=&followed;return hasFollow;}
template<>bool CThing::GetTC(CTCRegionFollower**p){*p=&region;return hasRegion;}
template<>bool CThing::GetTC(CTCAIScratchpad**p){*p=&scratch;return true;}
class ThingPtr{public:CThing* p;CThing* Get()const{return p;}CThing* operator->()const{return p;}operator CThing*()const{return p;}};
bool IS_THING_ALIVE(const ThingPtr& p){return p.Get() && p->alive;}
struct CThingFilter_HasScriptName{int name;CThingFilter_HasScriptName(int n):name(n){}};
struct CThingCompare_Nearest{Pos pos;CThingCompare_Nearest(Pos p):pos(p){}};
struct Search{int nearest,all;std::vector<CThing*> available;void GetPBestThing(CThingFilter_HasScriptName f,CThingCompare_Nearest,CThing** p){if(f.name!=77)abort();++nearest;*p=available.empty()?0:available[0];}template<class T>void GetAllThings(CThingFilter_HasScriptName f,std::vector<T*>& v){if(f.name!=77)abort();++all;v=available;}};
unsigned GFRandom(unsigned n,unsigned*){return n-1;}
class CAIStateGroup_MinionFollowThing{
public:
 ThingPtr PFollowee;CThing creature;Search search;float FollowDistance;bool AvoidDynamicObstacles,ExpressionUsed,FollowerAdded,waited,follow,stop;EMovementType CurrentMovementType;
 int avoid,term,rest,move,repeats;CThing* target;float distance;EMovementType chosen;bool avoidArg;unsigned seed;
 CAIStateGroup_MinionFollowThing():FollowDistance(11),AvoidDynamicObstacles(false),ExpressionUsed(false),FollowerAdded(false),waited(false),follow(false),stop(false),CurrentMovementType(AI_MOVEMENT_NULL),avoid(0),term(0),rest(0),move(0),repeats(0),target(0),distance(0),chosen(AI_MOVEMENT_NULL),avoidArg(false){PFollowee.p=0;search.nearest=search.all=0;}
 CThing& GetCreature(){return creature;}void SetThingToNotAvoid(CThing* p){if(p!=PFollowee.Get())abort();++avoid;}
 bool IsExpressionPerfomedTo(const char* s){return !strcmp(s,"EXPRESSION_WAIT")?waited:follow;}void Terminate(){++term;}
 bool IsStopping()const{return stop;}EAIActionReturn AIAction_DoNothing(){++rest;return rest+move<=repeats?AI_ACTION_IN_PROGRESS:AI_ACTION_FINISHED;}
 EAIActionReturn AIAction_MoveToThingProximity(int rpt,CThing* t,float dist,bool& done,EMovementType m,bool a,bool b,bool c,bool e,float f){
 if(rpt!=2||!a||b||!e||f!=0)abort();++move;target=t;distance=dist;chosen=m;avoidArg=c;done=true;
 if(!avoid)stop=true;return rest+move<=repeats?AI_ACTION_IN_PROGRESS:AI_ACTION_FINISHED;}
 Search& PeekThingSearchTools(){return search;}unsigned* GetPWorldSeed(){return &seed;}void Update();
};
void CAIStateGroup_MinionFollowThing::Update()
{
    if (IS_THING_ALIVE(PFollowee)) {
        SetThingToNotAvoid(PFollowee.Get());
        EAIActionReturn return_value;
        do {
            if (ExpressionUsed) {
                if (IsExpressionPerfomedTo("EXPRESSION_WAIT")) {
                    ExpressionUsed=false;
                    if (FollowerAdded && PFollowee!=0) {
                        CTCFollowed* ptc_followed;
                        if (PFollowee->GetTC(&ptc_followed))
                            ptc_followed->RemoveFollower(GetCreature());
                        if (!GetCreature().IsRegionFollowingOverriddenFromScript()) {
                            CTCRegionFollower* ptc_region_follower;
                            if (PFollowee->GetTC(&ptc_region_follower))
                                ptc_region_follower->RemoveFollower(GetCreature());
                        }
                    }
                    Terminate();
                    return;
                }
                CTCFollowed* ptc_followed;
                if (IS_THING_ALIVE(PFollowee) && PFollowee->GetTC(&ptc_followed) && !ptc_followed->IsFollower(&GetCreature())) {
                    ExpressionUsed=false;
                    Terminate();
                    return;
                }
            }
            if (IsExpressionPerfomedTo("EXPRESSION_FOLLOW"))
                ExpressionUsed=true;
            if (!IS_THING_ALIVE(PFollowee)) { Terminate(); return; }
            if (IsStopping()) { Terminate(); return; }
            if (GFGet2DDistanceSquared(GetCreature().PeekPos(),PFollowee->PeekPos()) < GFSqr(2.0f)) {
                CurrentMovementType=AI_MOVEMENT_NULL;
                return_value=AIAction_DoNothing();
            } else if (CurrentMovementType==AI_MOVEMENT_NULL && GFGet2DDistanceSquared(GetCreature().PeekPos(),PFollowee->PeekPos()) < GFSqr(3.5f)) {
                return_value=AIAction_DoNothing();
            } else {
                if (CurrentMovementType==AI_MOVEMENT_NULL) CurrentMovementType=AI_MOVEMENT_WALKING;
                if (CurrentMovementType==AI_MOVEMENT_WALKING) {
                    if (GFGet2DDistanceSquared(GetCreature().PeekPos(),PFollowee->PeekPos()) > GFSqr(7.5f)) CurrentMovementType=AI_MOVEMENT_RUNNING;
                } else if (GFGet2DDistanceSquared(GetCreature().PeekPos(),PFollowee->PeekPos()) < GFSqr(4.5f)) CurrentMovementType=AI_MOVEMENT_WALKING;
                bool finished_calculating_route;
                return_value=AIAction_MoveToThingProximity(ACTION_REPEAT_CALLBACK,PFollowee,FollowDistance,finished_calculating_route,CurrentMovementType,true,false,AvoidDynamicObstacles,true,0.0f);
            }
        } while (return_value==AI_ACTION_IN_PROGRESS);
    } else {
        CTCAIScratchpad* ai_scratchpad;
        GetCreature().GetTC(&ai_scratchpad);
        CThing* thing_to_follow=0;
        PeekThingSearchTools().GetPBestThing(CThingFilter_HasScriptName(ai_scratchpad->GetTargetName()), CThingCompare_Nearest(GetCreature().PeekPos()), &thing_to_follow);
        if (ai_scratchpad->GetScriptingStateGroup()==ESSG_FOLLOW_NEAREST) {
            PeekThingSearchTools().GetPBestThing(CThingFilter_HasScriptName(ai_scratchpad->GetTargetName()), CThingCompare_Nearest(GetCreature().PeekPos()), &thing_to_follow);
        } else {
            std::vector<CThing*> things;
            PeekThingSearchTools().GetAllThings<CThing>(CThingFilter_HasScriptName(ai_scratchpad->GetTargetName()),things);
            if (things.empty()) { Terminate(); return; }
            thing_to_follow=things[GFRandom(things.size(),GetPWorldSeed())];
        }
        while (ai_scratchpad->GetScriptingStateGroup()==ESSG_FOLLOW_RANDOM || ai_scratchpad->GetScriptingStateGroup()==ESSG_FOLLOW_NEAREST) {
            bool finished_calculating_route;
            AIAction_MoveToThingProximity(ACTION_REPEAT_CALLBACK,thing_to_follow,ai_scratchpad->FollowDistance,finished_calculating_route,AI_MOVEMENT_WALKING,true,false,AvoidDynamicObstacles,true,0.0f);
            if (IsStopping()) break;
        }
        Terminate();
    }
}

int main(){
 const float distances[]={0,1.9f,2,3.4f,3.5f,4.4f,4.5f,7.5f,7.6f};
 const EMovementType movements[]={AI_MOVEMENT_NULL,AI_MOVEMENT_WALKING,AI_MOVEMENT_RUNNING};
 for(int flags=0;flags<1024;++flags)for(int di=0;di<9;++di)for(int mt=0;mt<3;++mt){
  CAIStateGroup_MinionFollowThing c;CThing f;c.PFollowee.p=&f;f.pos.x=distances[di];c.CurrentMovementType=movements[mt];
  c.ExpressionUsed=(flags&1)!=0;c.waited=(flags&2)!=0;c.FollowerAdded=(flags&4)!=0;hasFollow=(flags&8)!=0;hasRegion=(flags&16)!=0;
  followed.member=(flags&32)!=0;c.creature.overrideRegion=(flags&64)!=0;c.stop=(flags&128)!=0;c.follow=(flags&256)!=0;c.AvoidDynamicObstacles=(flags&512)!=0;
  followed.removed=region.removed=0;bool wait=c.ExpressionUsed&&c.waited;bool lost=c.ExpressionUsed&&!wait&&hasFollow&&!followed.member;bool done=wait||lost||c.stop;
  c.Update();if(c.avoid!=1||c.term!=int(done)||followed.removed!=int(wait&&c.FollowerAdded&&hasFollow)||region.removed!=int(wait&&c.FollowerAdded&&!c.creature.overrideRegion&&hasRegion))return 1;
  if(wait||lost){if(c.ExpressionUsed)return 2;}else if(c.ExpressionUsed!=bool((flags&1)||c.follow))return 3;
  if(done){if(c.move||c.rest)return 4;continue;}
  float x=distances[di];bool rest=x<2||(mt==0&&x<3.5f);
  EMovementType expected=movements[mt];if(x<2)expected=AI_MOVEMENT_NULL;else if(!rest){if(expected==AI_MOVEMENT_NULL)expected=AI_MOVEMENT_WALKING;if(expected==AI_MOVEMENT_WALKING){if(x>7.5f)expected=AI_MOVEMENT_RUNNING;}else if(x<4.5f)expected=AI_MOVEMENT_WALKING;}
  if(c.CurrentMovementType!=expected||c.rest!=int(rest)||c.move!=int(!rest))return 5;
  if(!rest&&(c.target!=&f||c.distance!=11||c.chosen!=expected||c.avoidArg!=c.AvoidDynamicObstacles))return 6;
 }
 for(int mode=5;mode<9;++mode)for(int n=0;n<3;++n)for(int dead=0;dead<2;++dead)for(int avoid=0;avoid<2;++avoid){
  CAIStateGroup_MinionFollowThing c;CThing f,first,last;f.alive=false;c.PFollowee.p=dead?&f:0;c.AvoidDynamicObstacles=avoid!=0;scratch.mode=mode;scratch.FollowDistance=4.25f;
  if(n)c.search.available.push_back(&first);if(n==2)c.search.available.push_back(&last);c.Update();
  int moves=(mode==7||(mode==6&&n!=0));if(c.avoid||c.term!=1||c.move!=moves||c.search.nearest!=1+int(mode==7)||c.search.all!=int(mode!=7))return 7;
  if(moves&&(c.target!=(n==0?0:(mode==7||n==1?&first:&last))||c.distance!=4.25f||c.chosen!=AI_MOVEMENT_WALKING||c.avoidArg!=c.AvoidDynamicObstacles))return 8;
 }
 for(int counts=0;counts<5;++counts){CAIStateGroup_MinionFollowThing c;CThing f;c.PFollowee.p=&f;c.repeats=counts;c.Update();if(c.rest!=counts+1||c.term)return 9;}
 puts("00921990_TEST PASS");return 0;
}