#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
static void check(bool v){if(!v)exit(2);}
struct C3DVector {
 double x,y,z;
 C3DVector(double a=0,double b=0,double c=0):x(a),y(b),z(c){}
 C3DVector operator+(const C3DVector& b)const{return C3DVector(x+b.x,y+b.y,z+b.z);}
 C3DVector operator*(float n)const{return C3DVector(x*n,y*n,z*n);}
};
struct CRightHandedSet { C3DVector GetForward()const{return C3DVector(0,1,0);} C3DVector GetRight()const{return C3DVector(1,0,0);} };
struct Physics { CRightHandedSet GetRHSet()const{return CRightHandedSet();} };
struct Config{int present,move,look,sit,idle,story,random;} cfg;
static int moves,looks,idles,stories,rands,terms,searches,sits;
static unsigned long seed;
struct Village{bool IsStoryTime()const{return ++stories<=cfg.story;}};
struct CTCVillageMember{Village v;const Village* GetPVillage()const{return &v;}};
struct CThing{C3DVector pos;Physics physics;CTCVillageMember member;const C3DVector& GetPos()const{return pos;} Physics* GetPhysicsTC(){return &physics;} void GetTC(const CTCVillageMember** p){*p=&member;}} marker,creature;
struct CGameDefinitionManager{static CGameDefinitionManager& Get(){static CGameDefinitionManager m;return m;}long GetDefGlobalIndexFromName(const char* s){check(strcmp(s,"STORY_PLACE_MARKER")==0);return 42;}};
struct CThingFilter_HasDefGlobalIndex{CThingFilter_HasDefGlobalIndex(long n){check(n==42);}};
struct CThingCompare_Nearest{CThingCompare_Nearest(const C3DVector& v){check(&v==&creature.pos);}};
struct Search{void GetPBestThing(const CThingFilter_HasDefGlobalIndex&,const CThingCompare_Nearest&,CThing** out){++searches;*out=cfg.present?&marker:0;}};
enum EAIActionReturn {AI_ACTION_IN_PROGRESS,AI_ACTION_FINISHED,AI_ACTION_FAILED};
enum {ACTION_REPEAT_CALLBACK=2,AI_MOVEMENT_RUNNING=7};
static float GFFloatRandom(float max,unsigned long* p){check(p==&seed);++rands;check(fabs(max-(rands==1?1.5:1.5707963))<0.00001);return max*cfg.random*0.5f;}
struct CAIStateGroup_MinionListenToStory{
 Search search;
 Search& PeekThingSearchTools(){return search;}
 CThing& GetCreature(){return creature;}
 unsigned long* GetPWorldSeed(){return &seed;}
 void Terminate(){++terms;}
 EAIActionReturn AIAction_MoveToPosProximity(int call,const C3DVector& pos,float dist,bool& nav,int mode,bool a,bool b,bool c,int d,bool e,int f){
  double r=3.5+0.75*cfg.random, angle=0.7853981633974483*(cfg.random-1);
  check(call==ACTION_REPEAT_CALLBACK&&mode==AI_MOVEMENT_RUNNING&&dist==2&&a&&!b&&c&&!d&&e&&!f);
  check(fabs(pos.x-marker.pos.x-sin(angle)*r)<0.00001&&fabs(pos.y-marker.pos.y-cos(angle)*r)<0.00001&&pos.z==marker.pos.z);
  ++moves;nav=moves>1;return moves==1?AI_ACTION_IN_PROGRESS:(cfg.move?AI_ACTION_FINISHED:AI_ACTION_FAILED);
 }
 EAIActionReturn AIAction_LookAtPos(int call,const C3DVector& pos,int flags,float tol){check(moves==2&&call==ACTION_REPEAT_CALLBACK&&pos.x==marker.pos.x&&pos.y==marker.pos.y&&pos.z==marker.pos.z&&flags==17&&tol==0.0027777f);++looks;return looks==1?AI_ACTION_IN_PROGRESS:(cfg.look?AI_ACTION_FINISHED:AI_ACTION_FAILED);}
 bool SitDown(){check(looks==2);++sits;return cfg.sit!=0;}
 EAIActionReturn AIAction_DoNothing(){check(sits==1);return ++idles<=cfg.idle?AI_ACTION_IN_PROGRESS:AI_ACTION_FINISHED;}
 void Update();
};
void CAIStateGroup_MinionListenToStory::Update()
{
    long def_index = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("STORY_PLACE_MARKER");
    CThing* story_place = 0;
    PeekThingSearchTools().GetPBestThing(CThingFilter_HasDefGlobalIndex(def_index), CThingCompare_Nearest(GetCreature().GetPos()), &story_place);
    if (!story_place) {
        Terminate();
        return;
    }
    C3DVector teacher_pos = story_place->GetPos();
    CRightHandedSet teacher_orientation = story_place->GetPhysicsTC()->GetRHSet();
    static float dist = 2.0f;
    static float max_angle = 0.25f;
    float radius = GFFloatRandom(1.5f, GetPWorldSeed());
    radius += 3.5;
    float angle = GFFloatRandom(max_angle*6.2831855f,GetPWorldSeed()) - max_angle*3.1415927f;
    C3DVector sit_pos = teacher_pos + teacher_orientation.GetForward() * (radius * (float)cos(angle))
                                    + teacher_orientation.GetRight() * (radius * (float)sin(angle));
    EAIActionReturn ret = AI_ACTION_IN_PROGRESS;
    do {
        bool nav_route_calculated;
        ret = AIAction_MoveToPosProximity(ACTION_REPEAT_CALLBACK,sit_pos,dist,nav_route_calculated,AI_MOVEMENT_RUNNING,true,false,true,0,true,0);
    } while (ret==AI_ACTION_IN_PROGRESS);
    if (ret!=AI_ACTION_FINISHED) { Terminate(); return; }
    do {
        ret = AIAction_LookAtPos(ACTION_REPEAT_CALLBACK,teacher_pos,0x11,0.0027777f);
    } while (ret==AI_ACTION_IN_PROGRESS);
    if (!SitDown()) { Terminate(); return; }
    while (AIAction_DoNothing()==AI_ACTION_IN_PROGRESS) {
        const CTCVillageMember* ptc_village_mem;
        GetCreature().GetTC(&ptc_village_mem);
        if (!ptc_village_mem->GetPVillage()->IsStoryTime()) break;
    }
    Terminate();
}
int main(){
 marker.pos=C3DVector(3,-2,5);creature.pos=C3DVector(-8,3,0);
 for(int k=0;k<432;++k){
  int n=k;cfg.present=n%2;n/=2;cfg.move=n%2;n/=2;cfg.look=n%2;n/=2;cfg.sit=n%2;n/=2;cfg.idle=n%3;n/=3;cfg.story=n%3;n/=3;cfg.random=n%3;
  moves=looks=idles=stories=rands=terms=searches=sits=0;
  CAIStateGroup_MinionListenToStory state;state.Update();
  bool reached=cfg.present&&cfg.move, idle=reached&&cfg.sit;
  int queried=cfg.idle<cfg.story+1?cfg.idle:cfg.story+1;
  int calls=cfg.idle+1<cfg.story+1?cfg.idle+1:cfg.story+1;
  check(terms==1&&searches==1&&moves==(cfg.present?2:0)&&rands==moves&&looks==(reached?2:0)&&sits==(reached?1:0)&&idles==(idle?calls:0)&&stories==(idle?queried:0));
 }
 puts("009101c0_TEST PASS");return 0;
}