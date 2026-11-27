#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COPY_FORMAL
#include "engine/CThingCompare_Nearest.h"
// The production method below is exercised against deterministic search and action services.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
namespace fixture {
void check(bool value) { if (!value) abort(); }
#include "engine/EAIAction.h"
#include "engine/EMovementType.h"
enum { THING_OBJECT_PROPERTY_WELL = 0x800 };
struct C3DVector { float X,Y,Z; };
struct CTCFlammable { bool burning; mutable int reads; bool IsOnFire() const { ++reads; return burning; } };
struct CThingObject {
 bool dead,has; mutable int lookups; C3DVector pos; CTCFlammable fire;
 const C3DVector& GetPos() const {return pos;}
 bool PeekTC(const CTCFlammable** out) const {++lookups; if (has) *out=&fire; return has;}
};
template<class T> struct CIntelligentPointer {T* p; explicit CIntelligentPointer(T* arg):p(arg){} };
inline bool IS_THING_DEAD(const CThingObject* x) { return !x || x->dead; }
inline bool IS_THING_DEAD(const CIntelligentPointer<CThingObject>& x) {return IS_THING_DEAD(x.p);}
struct CThingFilter_ObjectHasProperty { int property; explicit CThingFilter_ObjectHasProperty(int x):property(x){} };
struct CThingCompare_Nearest { C3DVector pos; explicit CThingCompare_Nearest(const C3DVector& p):pos(p){} };
struct CCreatureAction_FillBucket { CThingObject& who; explicit CCreatureAction_FillBucket(CThingObject& a):who(a){} };
struct CAIStateGroup_MinionPutOutFires {
 CThingObject creature,well,building; CThingObject* PBuildingOnFire;
 bool search; int searches,moves,fills,repeats,change; EAIActionReturn final_move,fill;
 CThingObject& GetCreature(){return creature;}
 CAIStateGroup_MinionPutOutFires& PeekThingSearchTools(){return *this;}
 bool GetPBestLeafThing(const CThingFilter_ObjectHasProperty& f,const CThingCompare_Nearest& c,CThingObject** result){
   ++searches; check(*result==0 && f.property==THING_OBJECT_PROPERTY_WELL);
   check(c.pos.X==2 && c.pos.Y==-4 && c.pos.Z==8); if(search)*result=&well; return search;
 }
 EAIActionReturn AIAction_MoveToThingProximity(int cb,CIntelligentPointer<CThingObject> dest,float distance,bool& nav,int mode,bool b1,bool b2,bool b3,bool b4,float extra) {
   check(cb==ACTION_REPEAT_CALLBACK && dest.p==&well && distance==4 && mode==AI_MOVEMENT_RUNNING && b1 && !b2 && b3 && b4 && extra==0);
   check(nav==(moves!=0));nav=true;++moves;
   if(moves<=repeats) {if(change==1)well.dead=true;if(change==2)building.dead=true;if(change==3)building.has=false;if(change==4)building.fire.burning=false;return AI_ACTION_IN_PROGRESS;}
   return final_move;
 }
 EAIActionReturn AIAction_PerformAction(const CCreatureAction_FillBucket& action){check(&action.who==&creature);++fills;return fill;}
 bool GoToWell();
};
bool CAIStateGroup_MinionPutOutFires::GoToWell()
{
    EAIActionReturn ret = AI_ACTION_FAILED;
    CThingObject* pwell = 0;
    if (PeekThingSearchTools().GetPBestLeafThing(CThingFilter_ObjectHasProperty(THING_OBJECT_PROPERTY_WELL),
                                                 CThingCompare_Nearest(GetCreature().GetPos()), &pwell)) {
        CIntelligentPointer<CThingObject> ipwell(pwell);
        bool nav_route_calculated = false;
        do {
            const CTCFlammable* ptc_flammable;
            if (IS_THING_DEAD(ipwell) || IS_THING_DEAD(PBuildingOnFire) ||
                !PBuildingOnFire->PeekTC(&ptc_flammable) || !ptc_flammable->IsOnFire())
                return false;
            ret = AIAction_MoveToThingProximity(ACTION_REPEAT_CALLBACK, ipwell, 4.0f,
                nav_route_calculated, AI_MOVEMENT_RUNNING, true, false, true, true, 0.0f);
        } while (ret == AI_ACTION_IN_PROGRESS);
        if (ret == AI_ACTION_FINISHED) {
            if (AIAction_PerformAction(CCreatureAction_FillBucket(GetCreature())) == AI_ACTION_FINISHED)
                return true;
        }
    }
    return false;
}


}
struct ComparatorThing {
 ::C3DVector position;
 const ::C3DVector& GetPos() const {return position;}
};
static void CompareCases() {
 ::C3DVector origin(1.0f, 2.0f, 3.0f);
 ::CThingCompare_Nearest compare(origin);
 origin.X = 100;
 ComparatorThing a,b;
 a.position = ::C3DVector(1.0f, 2.0f, 3.0f);
 b.position = ::C3DVector(2.0f, 2.0f, 3.0f);
 ComparatorThing* empty = 0;
 fixture::check(compare(empty,empty)==empty);
 fixture::check(compare(&a,empty)==&a);
 fixture::check(compare(empty,&b)==&b);
 fixture::check(compare(&a,&b)==&a);
 fixture::check(compare(&b,&a)==&a);
 a.position = ::C3DVector(1.0f, 1.0f, 3.0f);
 fixture::check(compare(&a,&b)==&b);
 fixture::check(compare(&b,&a)==&a);
 b.position = ::C3DVector(1.0f, 2.0f, 5.0f);
 fixture::check(compare(&a,&b)==&a);
}
int main() {
 CompareCases();
 using namespace fixture;
 for(int mask=0;mask!=32;++mask)for(int n=0;n!=4;++n)for(int mod=0;mod!=5;++mod)
 for(int move=0;move!=2;++move)for(int fill=0;fill!=5;++fill) {
  CAIStateGroup_MinionPutOutFires v;memset(&v,0,sizeof(v));v.PBuildingOnFire=&v.building;
  v.creature.pos.X=2;v.creature.pos.Y=-4;v.creature.pos.Z=8;
  v.search=(mask&1)!=0;v.well.dead=(mask&2)!=0;v.building.dead=(mask&4)!=0;v.building.has=(mask&8)!=0;v.building.fire.burning=(mask&16)!=0;
  v.repeats=n;v.change=mod;v.final_move=move?AI_ACTION_FINISHED:AI_ACTION_FAILED;v.fill=(EAIActionReturn)fill;
  bool initial=v.search&&!v.well.dead&&!v.building.dead&&v.building.has&&v.building.fire.burning;
  bool stopped=n && mod;bool result=v.GoToWell();
  int calls=initial?(stopped?1:n+1):0;int fills=initial&&!stopped&&move?1:0;
  check(result==(fills && fill==AI_ACTION_FINISHED));check(v.searches==1&&v.moves==calls&&v.fills==fills);
  if(!v.search || (mask&6))check(v.building.lookups==0);
  if(!(mask&8))check(v.building.fire.reads==0);
 }
 puts("00935c40_TEST PASS");return 0;
}